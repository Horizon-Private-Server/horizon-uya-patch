/***************************************************
 * FILENAME :        main.c
 * DESCRIPTION :
 *      Juggernaut custom game mode.
 *
 *      DM-based mode. The first player to get a kill becomes the "juggernaut"
 *      (juggy). While juggy they receive double max health and all of their
 *      carried weapons upgraded to V2. Whoever kills the juggy becomes the new
 *      juggy; the old juggy reverts to normal on respawn.
 *
 *      Tracking is driven by polling the synced DM kill/death stats every tick,
 *      so every client -- including the killer's own console -- derives the same
 *      juggernaut deterministically. (The frag-message hook does NOT fire on the
 *      killer's client, which is exactly the client that must apply the buffs.)
 *
 * NOTES :
 *      Compile-time options (promote to game rules later):
 *        JUGGERNAUT_SCORE_JUGGY_ONLY  - only kills made while juggy advance the
 *                                       DM frag limit (default on).
 *        JUGGERNAUT_BRUISER_SKIN      - juggy's hero skin becomes Bruiser
 *                                       (default off; crashes mid-match, needs
 *                                       a proper in-game model swap).
 */

#include <tamtypes.h>
#include <string.h>

#include <libuya/stdio.h>
#include <libuya/time.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/player.h>
#include <libuya/ui.h>
#include <libuya/string.h>
#include <libuya/utils.h>
#include <libuya/interop.h>
#include <libuya/team.h>
#include <libuya/weapon.h>
#include "module.h"
#include "config.h"

// ---- Compile-time options -------------------------------------------------
// Skin swap is disabled: changing a live player's skin via gameSetClientSkin()
// fires a lobby-settings network update that crashes the client mid-match. A
// proper in-game model/skin swap needs to be REd separately before re-enabling.
#define JUGGERNAUT_BRUISER_SKIN         0

// When set, only kills made while ALREADY the juggernaut advance the DM frag
// limit; a player's crowning/dethroning kill and any non-juggy kills are erased
// from the scoreboard. Set to 0 for standard DM scoring (every kill counts).
#define JUGGERNAUT_SCORE_JUGGY_ONLY     1

#define JUGGERNAUT_HEALTH_MULTIPLIER    2

// UYA hero base max health is a fixed constant (PLAYER_MAX_HEALTH == 15, stored
// as a float). DM is uniform, so we use the constant directly instead of trying
// to read a per-player base at runtime -- reading pMoby->pVar+0x30 returned 0
// until something wrote it, which left the buff branch permanently gated off.
#define JUGGERNAUT_BASE_HEALTH          PLAYER_MAX_HEALTH
#define JUGGERNAUT_BUFFED_HEALTH        (JUGGERNAUT_BASE_HEALTH * JUGGERNAUT_HEALTH_MULTIPLIER)

// Base weapons upgraded to V2 while juggy (no-op if the player doesn't hold them).
static const int JUGGERNAUT_WEAPONS[] = {
    GADGET_ID_N60,
    GADGET_ID_BLITZ,
    GADGET_ID_FLUX,
    GADGET_ID_ROCKET,
    GADGET_ID_GBOMB,
    GADGET_ID_MINE,
    GADGET_ID_LAVA,
    GADGET_ID_MORPH,
};
#define JUGGERNAUT_WEAPON_COUNT (sizeof(JUGGERNAUT_WEAPONS) / sizeof(JUGGERNAUT_WEAPONS[0]))

// ---- State ----------------------------------------------------------------
static int Initialized = 0;
static int JuggyIndex = -1;                         // current juggernaut mpIndex, -1 = none
static short BaseSkin[GAME_MAX_PLAYERS];            // captured original skin
static char SkinCaptured[GAME_MAX_PLAYERS];         // original skin recorded yet?
static char Buffed[GAME_MAX_PLAYERS];               // one-time full-heal + weapon upgrades applied this life?
static char MaxHealthSet[GAME_MAX_PLAYERS];         // 2x max-health applied for current juggy role?

// Crown detection is driven by polling the synced DM kill/death stats every
// tick (not the frag-message hook -- that hook only fires on clients that
// RECEIVE the broadcast frag message, never on the killer's own client, which
// is exactly the client that must apply the local buffs). Polling the synced
// stats runs identically on every client, including the killer's.
static short LastKills[GAME_MAX_PLAYERS];
static short LastDeaths[GAME_MAX_PLAYERS];

static const char * JuggyPopupFormat = "%s is the Juggernaut!\0";

//--------------------------------------------------------------------------
static void announceJuggy(int idx)
{
    GameSettings * gameSettings = gameGetSettings();
    Player ** players = playerGetAll();
    if (!gameSettings || !players || idx < 0 || idx >= GAME_MAX_PLAYERS)
        return;

    char buf[64];
    sprintf(buf, JuggyPopupFormat, gameSettings->PlayerNames[idx]);

    int i;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        if (players[i] && players[i]->isLocal)
            uiShowPopup(players[i], buf, 3);
    }
}

//--------------------------------------------------------------------------
// Per-player max-health setter.
//
// libuya's playerSetMaxHealth() ALSO POKEs a single global instruction inside
// the respawn function (vaPlayerRespawnFunc + 0x140). That instruction is
// shared by every player, so using it for a per-player buff corrupts the
// respawn health of everyone in the match (the "killed player dies instantly
// on respawn" symptom). We only touch this player's own obfuscated max-HP cap
// and pVar health-bar value -- never the global respawn POKE.
static void juggySetMaxHealth(Player * player, int health)
{
    if (!player || !player->pMoby || !player->pMoby->pVar)
        return;

    playerObfuscate((u32)&player->maxHP, health, OBFUSCATE_MODE_HEALTH);
    *(float*)((u32)player->pMoby->pVar + 0x30) = (float)health;
}

//--------------------------------------------------------------------------
static void setJuggy(int idx)
{
    if (idx == JuggyIndex)
        return;

    // Immediately restore the outgoing juggy back to base stats.
    if (JuggyIndex >= 0 && JuggyIndex < GAME_MAX_PLAYERS) {
        if (MaxHealthSet[JuggyIndex]) {
            Player ** players = playerGetAll();
            Player * oldJuggyPlayer = players ? players[JuggyIndex] : NULL;
            juggySetMaxHealth(oldJuggyPlayer, JUGGERNAUT_BASE_HEALTH);
        }
        Buffed[JuggyIndex] = 0;
        MaxHealthSet[JuggyIndex] = 0;
        // Skin restoration is handled in processPlayer() (owning client only).
    }

    JuggyIndex = idx;
    announceJuggy(idx);
}

//--------------------------------------------------------------------------
// Snapshot current kill/death stats without acting on them. Used at init so a
// mid-match load (or pre-existing stats) doesn't trigger a false crown.
static void snapshotStats(void)
{
    GameData * gd = gameGetData();
    int i;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        if (gd) {
            LastKills[i] = gd->playerStats.frag[i].kills;
            LastDeaths[i] = gd->playerStats.frag[i].deaths;
        } else {
            LastKills[i] = 0;
            LastDeaths[i] = 0;
        }
    }
}

//--------------------------------------------------------------------------
// Derive the juggernaut from the synced DM kill/death stats. Runs every tick
// on every client, so the killer's own client (which must apply the local
// buffs) crowns the same juggy as everyone else.
//
//   - First blood: the first player to record a kill becomes juggy.
//   - Dethrone: when the current juggy's death count rises, whoever recorded a
//     kill that tick becomes the new juggy.
static void updateJuggyFromStats(void)
{
    GameData * gd = gameGetData();
    if (!gd)
        return;

    int i;
    int killer = -1;        // a player whose kill count rose this tick
    int juggyDied = 0;

#if JUGGERNAUT_SCORE_JUGGY_ONLY
    // Only the host owns the authoritative scoreboard/frag limit, so only the
    // host erases non-juggy kills. i != JuggyIndex is evaluated before setJuggy()
    // runs below, so a player's own crowning/dethroning kill does not count.
    int amHost = gameAmIHost();
#endif

    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        short kills = gd->playerStats.frag[i].kills;
        short deaths = gd->playerStats.frag[i].deaths;

        if (kills > LastKills[i]) {
            killer = i;

#if JUGGERNAUT_SCORE_JUGGY_ONLY
            if (amHost && i != JuggyIndex) {
                // Erase this non-juggy kill from the frag total. Hold our baseline
                // at the old value so we don't advance past the erased kill.
                gd->playerStats.frag[i].kills = LastKills[i];
                kills = LastKills[i];
            }
#endif
        }

        if (i == JuggyIndex && deaths > LastDeaths[i])
            juggyDied = 1;

        LastKills[i] = kills;
        LastDeaths[i] = deaths;
    }

    if (killer >= 0) {
        if (JuggyIndex < 0)
            setJuggy(killer);           // first blood
        else if (juggyDied)
            setJuggy(killer);           // killed the juggy -> new juggy
    }
}

//--------------------------------------------------------------------------
// Record the player's original skin once, so we can restore it on dethrone.
// Only reads gameSettings->PlayerSkins (always valid) -- no dependency on the
// player moby/pVar being populated.
static void captureSkin(int idx, GameSettings * gameSettings)
{
    if (idx < 0 || idx >= GAME_MAX_PLAYERS || SkinCaptured[idx] || !gameSettings)
        return;

    BaseSkin[idx] = gameSettings->PlayerSkins[idx];
    SkinCaptured[idx] = 1;
}

//--------------------------------------------------------------------------
static void processPlayer(Player * player, GameSettings * gameSettings)
{
    if (!player)
        return;

    int idx = player->mpIndex;
    if (idx < 0 || idx >= GAME_MAX_PLAYERS)
        return;

    captureSkin(idx, gameSettings);

    if (idx == JuggyIndex) {
        // Raise max health once per juggy role. juggySetMaxHealth is per-player
        // (no global respawn POKE).
        if (!MaxHealthSet[idx] && player->pMoby && player->pMoby->pVar) {
            juggySetMaxHealth(player, JUGGERNAUT_BUFFED_HEALTH);
            MaxHealthSet[idx] = 1;
        }

        // Reset Buffed on death so heal + weapons re-apply after each respawn.
        if (playerIsDead(player))
            Buffed[idx] = 0;

        // Once per life: full heal + weapon upgrades. Only the owning client
        // writes authoritative health/weapons for its local player.
        if (!Buffed[idx] && !playerIsDead(player) && player->isLocal) {
            playerSetHealth(player, JUGGERNAUT_BUFFED_HEALTH);

            int w;
            for (w = 0; w < (int)JUGGERNAUT_WEAPON_COUNT; ++w)
                playerGiveWeaponUpgrade(player, JUGGERNAUT_WEAPONS[w]);

            Buffed[idx] = 1;
        }

#if JUGGERNAUT_BRUISER_SKIN
        // Only the owning client changes its own skin. gameSetClientSkin fires
        // the network update that actually applies/broadcasts the change -- a
        // bare PlayerSkins[] write does nothing. Applied on next (re)spawn.
        if (player->isLocal && gameSettings && gameSettings->PlayerSkins[idx] != SKIN_BRUISER)
            gameSetClientSkin(idx, SKIN_BRUISER);
#endif
    } else {
        // Non-juggy: V2 weapons clear on next respawn naturally. Max health is
        // restored in setJuggy() when the role transfers.
        Buffed[idx] = 0;

#if JUGGERNAUT_BRUISER_SKIN
        if (player->isLocal && gameSettings && SkinCaptured[idx] && gameSettings->PlayerSkins[idx] != BaseSkin[idx])
            gameSetClientSkin(idx, BaseSkin[idx]);
#endif
    }
}

//--------------------------------------------------------------------------
static void setLobbyGameOptions(void)
{
    GameOptions * gameOptions = gameGetOptions();
    GameSettings * gameSettings = gameGetSettings();
    if (!gameOptions || !gameSettings || gameSettings->GameLoadStartTime <= 0)
        return;

    gameSettings->GameType = GAMETYPE_DM;
    gameOptions->GameFlags.MultiplayerGameFlags.Chargeboots = 1;
    gameOptions->GameFlags.MultiplayerGameFlags.PlayerNames = 1;
}

//--------------------------------------------------------------------------
static void resetState(void)
{
    JuggyIndex = -1;
    memset(BaseSkin, 0, sizeof(BaseSkin));
    memset(SkinCaptured, 0, sizeof(SkinCaptured));
    memset(Buffed, 0, sizeof(Buffed));
    memset(MaxHealthSet, 0, sizeof(MaxHealthSet));
    memset(LastKills, 0, sizeof(LastKills));
    memset(LastDeaths, 0, sizeof(LastDeaths));
}

//--------------------------------------------------------------------------
static void initialize(void)
{
    setLobbyGameOptions();
    resetState();

    // Seed the kill/death baseline from current stats so we don't fire a false
    // crown on the first tick (e.g. on a mid-match load).
    snapshotStats();

    Initialized = 1;
}

//--------------------------------------------------------------------------
void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
    GameSettings * gameSettings = gameGetSettings();

    // Ensure in game
    if (!gameSettings || !isInGame()) {
        // Dropped out of a match: clear state so the next game starts fresh.
        if (Initialized) {
            resetState();
            Initialized = 0;
        }
        return;
    }

    if (!Initialized)
        initialize();

    if (!gameHasEnded()) {
        // Update the crown from synced stats first, then apply per-player state.
        updateJuggyFromStats();

        Player ** players = playerGetAll();
        int i;
        for (i = 0; i < GAME_MAX_PLAYERS; ++i)
            processPlayer(players[i], gameSettings);
    }
}

//--------------------------------------------------------------------------
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
    UiMenu_t * menu;

    if (menu = uiGetActiveMenu(UI_MENU_STAGING, 0), menu != NULL) {
        // Clean state when entering staging for a new match.
        if (Initialized) {
            resetState();
            Initialized = 0;
        }
        setLobbyGameOptions();
    }
}

//--------------------------------------------------------------------------
void loadStart(struct GameModule * module, PatchStateContainer_t * gameState)
{
    setLobbyGameOptions();
}
