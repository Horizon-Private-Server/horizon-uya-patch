/* Host-authoritative crown pickup mode. The juggy receives V2 base weapons and
 * a damage cushion; clients render the synchronized crown locally. See
 * DESIGN.md for the state flow and engine-specific constraints. */

#include <tamtypes.h>
#include <string.h>

#include <libuya/stdio.h>
#include <libuya/time.h>
#include <libuya/math3d.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/player.h>
#include <libuya/moby.h>
#include <libuya/graphics.h>
#include <libuya/color.h>
#include <libuya/random.h>
#include <libuya/spawnpoint.h>
#include <libuya/ui.h>
#include <libuya/string.h>
#include <libuya/utils.h>
#include <libuya/interop.h>
#include <libuya/hud.h>
#include <libuya/net.h>
#include <libuya/team.h>
#include <libuya/weapon.h>
#include "messageid.h"
#include "module.h"
#include "config.h"

// ---- Compile-time options -------------------------------------------------
#define JUGG_DEBUG                      0

// Disabled: changing a live skin through gameSetClientSkin() crashes mid-match.
#define JUGGERNAUT_BRUISER_SKIN         0

// Cosmetic shield follows the owner-local damage cushion and is broadcast to
// remote clients; it does not provide gameplay absorption.
#define JUGGERNAUT_JUGGY_SHIELD         1

// Also render the cosmetic shield on the juggy's own screen.
#define JUGGERNAUT_JUGGY_SHIELD_LOCAL   1

// Only the current juggy's forwarded kill events are applied.
#define JUGGERNAUT_JUGGY_ONLY_KILLS     1

#define JUGGERNAUT_HEALTH_MULTIPLIER    2

// UYA's fixed base max health (15 in DM).
#define JUGGERNAUT_BASE_HEALTH          PLAYER_MAX_HEALTH
#define JUGGERNAUT_BUFFED_HEALTH        (JUGGERNAUT_BASE_HEALTH * JUGGERNAUT_HEALTH_MULTIPLIER)

// Extra survivability without writing obfuscated health/state fields.
#define JUGGERNAUT_CUSHION_HP           JUGGERNAUT_BASE_HEALTH

// A mid-life health pickup restores one absorbed hit, not the full cushion.
#define JUGGERNAUT_REFILL_ON_HEAL       1

// ---- Crown pickup tuning --------------------------------------------------
// 3D/GS colors are ABGR (not ARGB).
#define CROWN_MOBY_OCLASS               (0x1C0D)        // reused glowing-orb effect moby
#define CROWN_SPRITE_TEX                (81)
#define CROWN_SPRITE_COLOR              (0x0027D0FF)    // gold (ABGR)
#define CROWN_SPRITE_SCALE              (0.6)
#define CROWN_PICKUP_RADIUS             (3)
#define CROWN_HEIGHT_OFFSET             (1.0f)          // float the orb 1 unit up (Z is up)
#define CROWN_BEAM_HEIGHT               (100.0f)        // beacon pillar height (un-held pickup)
#define CROWN_MARKER_HEAD_OFFSET        (4.0f)          // orb height above the juggy's head

// ---- Minimap icon ---------------------------------------------------------
// When set, also show the juggy on the minimap while the crown is HELD; when 0,
// the icon only appears while the crown is on the ground (un-held pickup).
#define JUGGERNAUT_MAP_SHOW_JUGGY       1
// HUD colors are 0xAARRGGBB (ARGB) -- unlike the 3D/GS draws which are ABGR.
#define JUGG_MAP_COLOR                  (0xC0FFD027)    // gold (ARGB), ~75% alpha
#define JUGG_HUD_SPRITE                 (SPRITE_HUD_CIRCLE)
#define JUGG_HUD_CONTAINER_ID           (0x4a475f00)    // "JG_" -- unique vs KOTH's ids
#define JUGG_HUD_FRAME_ID               (JUGG_HUD_CONTAINER_ID + 1)
#define CROWN_BEAM_COLOR                (0x8027D0FF)    // gold, ~50% alpha (ABGR)
#define CROWN_HEARTBEAT_MS              (500)
#define CROWN_STALE_RELOCATE_MS         (20 * TIME_SECOND)
// Keep the crown neutral briefly after a death so the drop can be contested.
#define CROWN_DEATH_DROP_DELAY_MS       (3 * TIME_SECOND)
// Give players time to leave their spawn before the first pickup appears.
#define CROWN_FIRST_PICKUP_DELAY_MS     (10 * TIME_SECOND)

// Module-local message IDs; only one custom mode runs at a time.
#define CROWN_MSG_ID_STATE              (CUSTOM_MSG_ID_GAME_MODE_START)
// Shield state is broadcast by the juggy's own client.
#define CROWN_MSG_ID_SHIELD             (CUSTOM_MSG_ID_GAME_MODE_START + 1)

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

// Billboard frame textures require a temporary per-map getter swap.
extern VariableAddress_t vaGetFrameTex;
extern VariableAddress_t vaGetEffectTex;

// These draw.c types are mirrored here because no public header exists.
typedef enum LineStyle {
    LINE_STYLE_NONE = 0,
    LINE_STYLE_BEAM = 1,
    LINE_STYLE_SQUIGLY = 2,
    LINE_STYLE_LIGHTNING = 3,
    LINE_STYLE_NUM_STYLES = 4
} LineStyle_e;

typedef struct LineStatic {
    int   numSegments;
    int   texture;
    float textureOffset;
    float textureRepeatDistance;
    char  billboard;
    char  rotateTexture;
    char  pad[2];
    float lineWidth;
    float widthStart;
    float widthEnd;
    int   scrollFrames;
    float pointRandomOffset;
} LineStatic_t;

typedef struct LineEndPoint {
    VECTOR      pos;
    int         color;
    char        lerpColor;
    char        pad[2];
    LineStyle_e style;
    float       fadeEnds;
} LineEndPoint_t;

extern void drawLines(LineEndPoint_t * pEndPoints, int numEndPoints, LineStatic_t * pStyle);

// HUD container creator (in libuya functions.S, not declared in hud.h).
extern void hudCreateContainer(HANDLE_ID handle_id);

// Straight, tall base-light pillar (no random jaggedness like SquiglyLineStyle).
static LineStatic_t CrownBeamStyle = {
    .numSegments = 4,
    .texture = FX_BASELIGHT,
    .textureOffset = 0,
    .textureRepeatDistance = 0,
    .billboard = 1,
    .rotateTexture = 0,
    .lineWidth = 2.0f,           // a little wider
    .widthStart = 1.0f,
    .widthEnd = 1.0f,
    .scrollFrames = 600,         // slower scroll/scintillation (larger = slower)
    .pointRandomOffset = 0.0f,   // dead straight
};

VariableAddress_t vaReplace_GetEffectTexJAL = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x2045b2f0,
    .Hoven = 0x2045ce70,
    .OutpostX12 = 0x20453c70,
    .KorgonOutpost = 0x20451830,
    .Metropolis = 0x20450b70,
    .BlackwaterCity = 0x2044e370,
    .CommandCenter = 0x2044eff0,
    .BlackwaterDocks = 0x20451870,
    .AquatosSewers = 0x20450b70,
    .MarcadiaPalace = 0x204504f0,
#else
    .Lobby = 0,
    .Bakisi = 0x2045a220,
    .Hoven = 0x2045bce0,
    .OutpostX12 = 0x20452b20,
    .KorgonOutpost = 0x20450760,
    .Metropolis = 0x2044faa0,
    .BlackwaterCity = 0x2044d220,
    .CommandCenter = 0x2044e060,
    .BlackwaterDocks = 0x204508a0,
    .AquatosSewers = 0x2044fbe0,
    .MarcadiaPalace = 0x2044f520,
#endif
};

// ---- Networked crown state (host broadcasts, clients apply) ---------------
typedef struct CrownStateMsg {
    int   Seq;
    short JuggyIdx;       // -1 = none
    short PickupActive;   // 0/1
    float PickupPos[3];
} CrownStateMsg_t;

// Juggy shield state, broadcast by the juggy's own client (health isn't synced).
typedef struct CrownShieldMsg {
    short JuggyIdx;       // sender's view of who the juggy is (for validation)
    short ShieldOn;       // 0/1
} CrownShieldMsg_t;

// ---- State ----------------------------------------------------------------
static int Initialized = 0;
static int HandlerInstalled = 0;
static int LastGameLoadTime = 0;   // detects a new match even if teardown was skipped

// Crown state (authoritative on host, mirrored from broadcast on clients).
static int    StateSeq = 0;
static int    JuggyIndex = -1;                      // current juggernaut mpIndex, -1 = none
static int    PickupActive = 0;
static VECTOR PickupPos;

// Host-only scheduling.
static int    HostDropPending = 0;                  // juggy died, waiting to drop crown
static int    HostDropAtTime = 0;
static VECTOR HostDropPos;
static int    HostDropHasPos = 0;
static int    HostStaleAtTime = 0;                  // relocate pickup if untouched by then
static int    HostNextHeartbeat = 0;
static int    HostStarted = 0;                       // host seen its first in-game tick this match?
static int    HostMatchStartTime = 0;                // gameTime of that first tick (initial-pickup grace)
static int    HostFirstPickupDone = 0;               // first pickup placed? grace applies only once

// Per-player buff bookkeeping.
static char   Buffed[GAME_MAX_PLAYERS];             // one-time cushion refill + weapon upgrades this life?

// Owner-local cushion, refilled once per juggy life and spent by absorbed damage.
static int    JuggyCushion = 0;
#if JUGGERNAUT_REFILL_ON_HEAL
static int    JuggyLastHp = 0;   // local juggy's last HP (read-only), to detect a heal
#endif

// ---- Guarded-hurt hook ----------------------------------------------------
// Hook every matching damage call site: PvP damage is not routed through one
// universal site. Non-juggy damage passes through unchanged.
#define JUGGY_HURT_SCAN_RANGE   (0x20000)   // sites span ~HF-0x19000 .. HF+0x1f000
#define JUGGY_HURT_MAX_SITES    (24)        // ~9 expected; headroom

static u32  HurtHookSites[JUGGY_HURT_MAX_SITES];  // patched call-site addresses
static u32  HurtHookSaved[JUGGY_HURT_MAX_SITES];  // their original words, for teardown
static int  HurtHookCount = 0;                    // 0 = not installed
static u32  HurtFuncAddr  = 0;                     // resolved vaHurtPlayerFunc (direct passthrough, no per-hit re-resolve)

// Map-resolved trampoline and per-map call-site table.
extern void internal_HurtPlayer(Player * player, int damage);
extern VariableAddress_t vaHurtPlayerFunc;

#if JUGGERNAUT_JUGGY_ONLY_KILLS
// ---- Juggy-only kill credit -----------------------------------------------
// The score handler runs on both receive and originate paths, so hook every
// matching call site to keep local and remote scoreboards consistent.
static VariableAddress_t vaOnPlayerKill_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00544488, .Hoven = 0x00546650, .OutpostX12 = 0x0053bf28,
    .KorgonOutpost = 0x00539610, .Metropolis = 0x00538a10, .BlackwaterCity = 0x005361f8,
    .CommandCenter = 0x00535a50, .BlackwaterDocks = 0x005382d0, .AquatosSewers = 0x005375d0,
    .MarcadiaPalace = 0x00536f50,
#else
    .Lobby = 0,
    .Bakisi = 0x00541b78, .Hoven = 0x00543c80, .OutpostX12 = 0x00539598,
    .KorgonOutpost = 0x00536d00, .Metropolis = 0x00536100, .BlackwaterCity = 0x00533868,
    .CommandCenter = 0x00533298, .BlackwaterDocks = 0x00535ad8, .AquatosSewers = 0x00534e18,
    .MarcadiaPalace = 0x00534758,
#endif
};
// This window brackets both known call sites while staying inside the overlay.
#define JUGGY_KILL_SCAN_LO      (0x4b000)
#define JUGGY_KILL_SCAN_HI      (0x8000)
#define JUGGY_KILL_MAX_SITES    (8)
static u32 KillHookSites[JUGGY_KILL_MAX_SITES];
static u32 KillHookSaved[JUGGY_KILL_MAX_SITES];
static int KillHookCount = 0;   // 0 = not installed
#endif

// Cosmetic shield state: owner-computed, then mirrored to other clients.
static int    JuggyShieldOn = 1;
static int    ShieldLastSent = -1;                  // last value this client broadcast (juggy only)
static int    ShieldNextResend = 0;                 // periodic re-broadcast time (late joiners / loss)

// Local cosmetic sprite.
static Moby * CrownMoby = NULL;
static int    CrownShowBeam = 0;   // draw the tall pillar (un-held pickup) vs just the orb (juggy marker)
static int    JuggMapInitialized = 0;   // minimap icon HUD widget created?

struct CrownPVar {
    int unused;   // moby needs a small pvar; the crown draws via the beam + orb
};

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

// ==========================================================================
//  Sanity validation (mirrors koth/game.c: reject NaN and bogus coordinates)
// ==========================================================================
static int floatIsSane(float v)
{
    return v == v && v > -10000.0f && v < 10000.0f;
}

static int vectorIsSane(VECTOR v)
{
    return floatIsSane(v[0]) && floatIsSane(v[1]) && floatIsSane(v[2]);
}

//--------------------------------------------------------------------------
// Pick a validated player spawn, falling back to a living player.
static int pickRandomSpawnPos(VECTOR out)
{
    int count = spawnPointGetCount();
    int tries;

    // Try random player spawn cuboids first.
    for (tries = 0; tries < 16 && count > 0; ++tries) {
        int idx = randRangeInt(0, count - 1);
        if (idx < 0 || idx >= count || !spawnPointIsPlayer(idx))
            continue;

        Cuboid * sp = spawnPointGet(idx);
        if (!sp || !vectorIsSane(sp->pos))
            continue;

        vector_copy(out, sp->pos);
        return 1;
    }

    // Fallback: a living player's position.
    Player ** players = playerGetAll();
    int i;
    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
        Player * p = players[i];
        if (p && !playerIsDead(p) && vectorIsSane(p->playerPosition)) {
            vector_copy(out, p->playerPosition);
            return 1;
        }
    }

    return 0;
}

// ==========================================================================
//  Local cosmetic crown sprite
// ==========================================================================
// Draw the beacon beam with the normal effect-texture getter.
static void crownDrawBeam(VECTOR at)
{
    LineEndPoint_t eps[2];
    memset(eps, 0, sizeof(eps));

    vector_copy(eps[0].pos, at);
    eps[0].pos[2] -= CROWN_HEIGHT_OFFSET;          // start at ground (orb floats +1)
    eps[0].color = CROWN_BEAM_COLOR;
    eps[0].style = LINE_STYLE_NONE;                // use CrownBeamStyle

    vector_copy(eps[1].pos, at);
    eps[1].pos[2] += CROWN_BEAM_HEIGHT;            // tall
    eps[1].color = CROWN_BEAM_COLOR & 0x00FFFFFF;  // fade alpha to 0 at the top
    eps[1].style = LINE_STYLE_NONE;

    drawLines(eps, 2, &CrownBeamStyle);
}

static void crownDraw(Moby * moby)
{
    if (!moby)
        return;

    // The beam marks an unheld pickup; a held crown is an orb over the juggy.
    if (CrownShowBeam)
        crownDrawBeam(moby->position);

    // Frame-texture drawing needs a guarded per-map JAL swap.
    u32 site = (u32)GetAddress(&vaReplace_GetEffectTexJAL);
    u32 frameTex = (u32)GetAddress(&vaGetFrameTex);
    u32 effectTex = (u32)GetAddress(&vaGetEffectTex);
    if (!site || !frameTex || !effectTex)
        return;

    int opacity = 0x80 << 24;                                   // alpha only
    u32 color = opacity | (CROWN_SPRITE_COLOR & 0x00FFFFFF);
    moby->primaryColor = color;

    u32 hook = site + 0x20;
    HOOK_JAL(hook, frameTex);
    gfxDrawBillboardQuad(0.55, 0, MATH_PI, moby->position, CROWN_SPRITE_TEX, opacity, 0);
    gfxDrawBillboardQuad(CROWN_SPRITE_SCALE, 0.01, MATH_PI, moby->position, CROWN_SPRITE_TEX, color, 0);
    HOOK_JAL(hook, effectTex);
}

static void crownUpdate(Moby * moby)
{
    // Unheld: beam plus orb at the pickup. Held: orb over the juggy.
    // Keep the moby persistent; retaining a particle instance across frames can
    // corrupt the particle pool.
    if (PickupActive) {
        vector_copy(moby->position, PickupPos);
        CrownShowBeam = 1;
    } else if (JuggyIndex >= 0 && JuggyIndex < GAME_MAX_PLAYERS) {
        Player ** players = playerGetAll();
        Player * jp = players ? players[JuggyIndex] : NULL;
        if (!jp || playerIsDead(jp))
            return;                             // nothing to draw
        if (jp->isLocal)
            return;                             // don't obscure the juggy's own view
        vector_copy(moby->position, jp->playerPosition);
        moby->position[2] += CROWN_MARKER_HEAD_OFFSET;
        CrownShowBeam = 0;
    } else {
        return;                                 // no pickup, no juggy
    }

    gfxRegisterDrawFunction(&crownDraw, moby);
}

static void crownSpriteDestroy(void)
{
    if (!CrownMoby)
        return;

    if (!mobyIsDestroyed(CrownMoby))
        mobyDestroy(CrownMoby);
    CrownMoby = NULL;
}

// Keep one crown moby for the match; update hides or shows it as needed.
static void crownSpriteSync(void)
{
    if (CrownMoby && !mobyIsDestroyed(CrownMoby))
        return;

    CrownMoby = mobySpawn(CROWN_MOBY_OCLASS, sizeof(struct CrownPVar));
    if (CrownMoby) {
        CrownMoby->pUpdate = &crownUpdate;
        CrownMoby->updateDist = -1;
        CrownMoby->drawn = 1;
        CrownMoby->alpha = 0x00;
        CrownMoby->drawDist = 0x00;
    }
}

// ==========================================================================
//  Minimap icon (same widget pattern as koth's radar marker)
// ==========================================================================
static int juggMapSetVisible(int visible)
{
    int hasFrame = hudSetFlags(JUGG_HUD_FRAME_ID, 1, visible);
    int hasContainer = hudSetFlags(JUGG_HUD_CONTAINER_ID, 1, visible);
    return hasFrame || hasContainer;
}

static void juggMapInit(void)
{
    if (JuggMapInitialized) {
        if (juggMapSetVisible(1))
            return;
        JuggMapInitialized = 0;   // HUD torn down under us; rebuild
    }

    hudCreateContainer(JUGG_HUD_CONTAINER_ID);
    hudSetFlags(JUGG_HUD_CONTAINER_ID, 1, true);
    hudAddToContainer(HUD_RADAR_ROOT, JUGG_HUD_CONTAINER_ID);
    if (hudCreateRectangle(0.05, 0.05, 0.05, 0.05, JUGG_HUD_FRAME_ID, JUGG_MAP_COLOR, JUGG_HUD_SPRITE))
        hudAddToContainer(JUGG_HUD_CONTAINER_ID, JUGG_HUD_FRAME_ID);
    hudSetFlags(JUGG_HUD_FRAME_ID, 1, true);
    JuggMapInitialized = 1;
}

static void juggMapUpdate(void)
{
    VECTOR target;
    int show = 0;

    if (PickupActive) {
        vector_copy(target, PickupPos);         // crown on the ground
        show = 1;
    }
#if JUGGERNAUT_MAP_SHOW_JUGGY
    else if (JuggyIndex >= 0 && JuggyIndex < GAME_MAX_PLAYERS) {
        Player ** players = playerGetAll();
        Player * jp = players ? players[JuggyIndex] : NULL;
        if (jp && !playerIsDead(jp) && vectorIsSane(jp->playerPosition)) {
            vector_copy(target, jp->playerPosition);
            show = 1;
        }
    }
#endif

    if (!show || !vectorIsSane(target)) {
        juggMapSetVisible(0);
        return;
    }

    float x, y;
    gfxWStoMapSpace(target, &x, &y);
    if (!floatIsSane(x) || !floatIsSane(y)) {
        juggMapSetVisible(0);
        return;
    }
    if (x < 0.0f) x = 0.0f; else if (x > 1.0f) x = 1.0f;
    if (y < 0.0f) y = 0.0f; else if (y > 1.0f) y = 1.0f;

    juggMapSetVisible(1);
    hudSetPosition(x, y, JUGG_HUD_FRAME_ID);
    hudSetColor(JUGG_HUD_FRAME_ID, JUGG_MAP_COLOR);
}

// ==========================================================================
//  Crown state broadcast / receive
// ==========================================================================
static void crownBroadcastState(void)
{
    void * connection = netGetDmeServerConnection();
    if (!connection)
        return;

    CrownStateMsg_t msg;
    msg.Seq = StateSeq;
    msg.JuggyIdx = (short)JuggyIndex;
    msg.PickupActive = (short)PickupActive;
    msg.PickupPos[0] = PickupPos[0];
    msg.PickupPos[1] = PickupPos[1];
    msg.PickupPos[2] = PickupPos[2];

    netBroadcastCustomAppMessage(connection, CROWN_MSG_ID_STATE, sizeof(msg), &msg);
}

// The juggy's own client broadcasts this flag.
static void juggyBroadcastShield(int on)
{
    void * connection = netGetDmeServerConnection();
    if (!connection)
        return;

    CrownShieldMsg_t msg;
    msg.JuggyIdx = (short)JuggyIndex;
    msg.ShieldOn = (short)(on ? 1 : 0);
    netBroadcastCustomAppMessage(connection, CROWN_MSG_ID_SHIELD, sizeof(msg), &msg);
}

// Broadcast on change and periodically for late joiners or dropped packets.
static void juggyUpdateShieldState(int want)
{
    int now = gameGetTime();
    JuggyShieldOn = want ? 1 : 0;
    if (JuggyShieldOn != ShieldLastSent || now >= ShieldNextResend) {
        juggyBroadcastShield(JuggyShieldOn);
        ShieldLastSent = JuggyShieldOn;
        ShieldNextResend = now + CROWN_HEARTBEAT_MS;
    }
}

// A new juggy starts shielded; force an immediate broadcast.
static void juggyResetShieldForNewJuggy(void)
{
    JuggyShieldOn = 1;
    ShieldLastSent = -1;
    ShieldNextResend = 0;
}

static int crownOnReceiveShield(void * connection, void * data)
{
    CrownShieldMsg_t * msg = (CrownShieldMsg_t*)data;
    // Ignore stale or foreign flags.
    if (msg->JuggyIdx == JuggyIndex)
        JuggyShieldOn = msg->ShieldOn ? 1 : 0;
    return sizeof(CrownShieldMsg_t);
}

static int crownOnReceiveState(void * connection, void * data)
{
    // The host is authoritative.
    if (gameAmIHost())
        return sizeof(CrownStateMsg_t);

    CrownStateMsg_t * msg = (CrownStateMsg_t*)data;

    // Equal snapshots are harmless and keep heartbeats idempotent.
    if (msg->Seq < StateSeq)
        return sizeof(CrownStateMsg_t);

    StateSeq = msg->Seq;

    int newJuggy = msg->JuggyIdx;
    if (newJuggy != JuggyIndex && newJuggy >= 0 && newJuggy < GAME_MAX_PLAYERS) {
        announceJuggy(newJuggy);
        juggyResetShieldForNewJuggy();   // default shielded until the juggy says otherwise
    }
    JuggyIndex = (newJuggy >= 0 && newJuggy < GAME_MAX_PLAYERS) ? newJuggy : -1;

    PickupActive = msg->PickupActive ? 1 : 0;
    PickupPos[0] = msg->PickupPos[0];
    PickupPos[1] = msg->PickupPos[1];
    PickupPos[2] = msg->PickupPos[2];

    return sizeof(CrownStateMsg_t);
}

// ==========================================================================
//  Host-authoritative crown state machine
// ==========================================================================
static void hostCommitAndBroadcast(void)
{
    ++StateSeq;
    crownBroadcastState();
    HostNextHeartbeat = gameGetTime() + CROWN_HEARTBEAT_MS;
}

static void hostActivatePickup(VECTOR pos, int gameTime)
{
    vector_copy(PickupPos, pos);
    PickupPos[2] += CROWN_HEIGHT_OFFSET;   // float the orb up for visibility (Z is up)
    PickupActive = 1;
    JuggyIndex = -1;
    HostStaleAtTime = gameTime + CROWN_STALE_RELOCATE_MS;
    HostDropPending = 0;
    hostCommitAndBroadcast();
}

static void hostCrownPlayer(int idx, int gameTime)
{
    JuggyIndex = idx;
    PickupActive = 0;
    announceJuggy(idx);
    juggyResetShieldForNewJuggy();
    hostCommitAndBroadcast();
}

static void hostBeginDeathDrop(int gameTime, VECTOR deathPos, int hasPos)
{
    // Drop the crown from the (now former) juggy immediately; the pickup
    // materializes after a short arm delay so nearby players can contest it.
    JuggyIndex = -1;
    HostDropPending = 1;
    HostDropAtTime = gameTime + CROWN_DEATH_DROP_DELAY_MS;
    HostDropHasPos = hasPos;
    if (hasPos)
        vector_copy(HostDropPos, deathPos);
    hostCommitAndBroadcast();
}

static void juggyHostTick(int gameTime)
{
    Player ** players = playerGetAll();

    // Stamp match start for the initial-pickup delay.
    if (!HostStarted) {
        HostStarted = 1;
        HostMatchStartTime = gameTime;
    }

    // 1. Watch the juggy for death or disconnect.
    if (JuggyIndex >= 0 && JuggyIndex < GAME_MAX_PLAYERS) {
        Player * jp = players[JuggyIndex];
        if (!jp) {
            hostBeginDeathDrop(gameTime, NULL, 0);            // disconnect -> random relocate
        } else if (playerIsDead(jp)) {
            int hasPos = vectorIsSane(jp->playerPosition);
            hostBeginDeathDrop(gameTime, jp->playerPosition, hasPos);
        }
    }

    // 2. Materialize a pending death drop.
    if (HostDropPending && gameTime >= HostDropAtTime) {
        VECTOR pos;
        if (HostDropHasPos)
            vector_copy(pos, HostDropPos);
        else if (!pickRandomSpawnPos(pos)) {
            // Retry if no valid location is available yet.
            HostDropAtTime = gameTime + CROWN_HEARTBEAT_MS;
            return;
        }
        hostActivatePickup(pos, gameTime);
    }

    // 3. Spawn the initial or a mid-match pickup.
    if (JuggyIndex < 0 && !PickupActive && !HostDropPending) {
        if (HostFirstPickupDone || gameTime >= HostMatchStartTime + CROWN_FIRST_PICKUP_DELAY_MS) {
            VECTOR pos;
            if (pickRandomSpawnPos(pos)) {
                hostActivatePickup(pos, gameTime);
                HostFirstPickupDone = 1;
            }
        }
    }

    // 4. Detect pickup overlap and relocate stale pickups.
    if (PickupActive) {
        int i;
        for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
            Player * p = players[i];
            if (!p || playerIsDead(p))
                continue;

            VECTOR d;
            vector_subtract(d, p->playerPosition, PickupPos);
            if (vector_sqrmag(d) < (CROWN_PICKUP_RADIUS * CROWN_PICKUP_RADIUS)) {
                hostCrownPlayer(i, gameTime);
                break;
            }
        }

        if (PickupActive && gameTime >= HostStaleAtTime) {
            VECTOR pos;
            if (pickRandomSpawnPos(pos))
                hostActivatePickup(pos, gameTime);
            else
                HostStaleAtTime = gameTime + CROWN_STALE_RELOCATE_MS;
        }
    }

    // 5. Heartbeat for late joiners and host migration.
    if (gameTime >= HostNextHeartbeat) {
        crownBroadcastState();
        HostNextHeartbeat = gameTime + CROWN_HEARTBEAT_MS;
    }
}

// ==========================================================================
//  Guarded-hurt hook -- the "2x health" buff, without touching health/state
// ==========================================================================
// Absorb damage into the cushion without writing health/state. Knockback is
// applied separately by the projectile impact.
#if JUGG_DEBUG
static int HookCalls   = 0;   // times the hooked site fired (any player)
static int HookAbsorbs = 0;   // times we actually absorbed for the juggy
static int HookLastIdx = -1;  // mpIndex of the last hurt player seen here
static int HookLastDmg = 0;   // last damage value seen here
#endif

static void juggyHurtWeaponHit_Hook(Player * player, int damage)
{
#if JUGG_DEBUG
    HookCalls++;
    HookLastIdx = player ? player->mpIndex : -2;
    HookLastDmg = damage;
#endif
    if (player && player->mpIndex == JuggyIndex && JuggyCushion > 0 && damage > 0) {
#if JUGG_DEBUG
        HookAbsorbs++;
#endif
        JuggyCushion -= damage;     // crossing-hit overflow is fully absorbed, NOT passed through -- shield then drops on cu<=0 for a "last block, now you take damage" feel
        return;                     // damage NOT applied
    }

    // Use the cached address; retain the trampoline as a fallback.
    if (HurtFuncAddr)
        ((void (*)(Player *, int))HurtFuncAddr)(player, damage);
    else
        internal_HurtPlayer(player, damage);
}

static void juggyUninstallHurtHook(void)
{
    // Restore only slots that still contain our hook; map reloads may invalidate
    // the saved addresses.
    u32 ourJal = ADDR2JAL((u32)&juggyHurtWeaponHit_Hook);
    int i;
    for (i = 0; i < HurtHookCount; ++i)
        if (*(u32*)HurtHookSites[i] == ourJal)
            *(u32*)HurtHookSites[i] = HurtHookSaved[i];
    HurtHookCount = 0;
    HurtFuncAddr = 0;
}

static void juggyInstallHurtHook(void)
{
    juggyUninstallHurtHook();                // clear any stale state, then scan fresh

    u32 hurtFunc = GetAddress(&vaHurtPlayerFunc);
    if (!hurtFunc)
        return;

    HurtFuncAddr = hurtFunc;
    u32 jalWord = ADDR2JAL(hurtFunc);        // `jal vaHurtPlayerFunc` for this map

    // Match the exact call word. If no sites are found, the cushion stays off.
    u32 lo = hurtFunc - JUGGY_HURT_SCAN_RANGE;
    u32 hi = hurtFunc + JUGGY_HURT_SCAN_RANGE;
    u32 a;
    for (a = lo; a < hi && HurtHookCount < JUGGY_HURT_MAX_SITES; a += 4) {
        if (*(u32*)a == jalWord) {
            HurtHookSites[HurtHookCount] = a;
            HurtHookSaved[HurtHookCount] = *(u32*)a;
            HOOK_JAL(a, &juggyHurtWeaponHit_Hook);
            HurtHookCount++;
        }
    }
}

#if JUGGERNAUT_JUGGY_ONLY_KILLS
// Apply forwarded score events only when fragMsg[0] is the juggy's mpIndex.
static void juggyOnPlayerKill(char * fragMsg)
{
    if (fragMsg && fragMsg[0] >= 0 && fragMsg[0] == JuggyIndex)
        ((void (*)(char *))GetAddress(&vaOnPlayerKill_Func))(fragMsg);
    // else: non-juggy kill -> not applied (no credit / death / kill-feed)
}

static void juggyUninstallKillHook(void)
{
    // Restore only sites that still contain our hook.
    u32 ourJal = ADDR2JAL((u32)&juggyOnPlayerKill);
    int i;
    for (i = 0; i < KillHookCount; ++i)
        if (*(u32*)KillHookSites[i] == ourJal)
            *(u32*)KillHookSites[i] = KillHookSaved[i];
    KillHookCount = 0;
}

static void juggyInstallKillHook(void)
{
    juggyUninstallKillHook();                 // clear stale state, then scan fresh

    u32 func = GetAddress(&vaOnPlayerKill_Func);
    if (!func)
        return;

    u32 jalWord = ADDR2JAL(func);
    u32 lo = func - JUGGY_KILL_SCAN_LO;
    u32 hi = func + JUGGY_KILL_SCAN_HI;
    u32 a;
    for (a = lo; a < hi && KillHookCount < JUGGY_KILL_MAX_SITES; a += 4) {
        if (*(u32*)a == jalWord) {
            KillHookSites[KillHookCount] = a;
            KillHookSaved[KillHookCount] = *(u32*)a;
            HOOK_JAL(a, &juggyOnPlayerKill);
            KillHookCount++;
        }
    }
}
#endif

#if JUGGERNAUT_JUGGY_SHIELD
// Destroy this player's cosmetic shield moby, if present.
static void juggyRemoveShield(Player * player)
{
    Moby * shield = mobyListGetStart();
    while ((shield = mobyFindNextByOClass(shield, MOBY_ID_OMNI_SHIELD))) {
        if (shield->pVar && *(u32*)((u32)shield->pVar + 0x40) == (u32)player)
            mobyDestroy(shield);
        ++shield;
    }
}

// Remove shields whose owner is no longer the live juggy. Compare the stored
// owner pointer without dereferencing it; this avoids stale-owner TLB faults.
static void juggySweepStrayShields(void)
{
    Player * juggy = NULL;
    if (JuggyIndex >= 0 && JuggyIndex < GAME_MAX_PLAYERS) {
        Player ** players = playerGetAll();
        Player * jp = players ? players[JuggyIndex] : NULL;
        if (jp && !playerIsDead(jp))
            juggy = jp;
    }

    Moby * shield = mobyListGetStart();
    while ((shield = mobyFindNextByOClass(shield, MOBY_ID_OMNI_SHIELD))) {
        if (shield->pVar && *(u32*)((u32)shield->pVar + 0x40) != (u32)juggy)
            mobyDestroy(shield);
        ++shield;
    }
}
#endif

static void processPlayer(Player * player)
{
    if (!player)
        return;

    int idx = player->mpIndex;
    if (idx < 0 || idx >= GAME_MAX_PLAYERS)
        return;

    if (idx == JuggyIndex) {
        // Re-apply the cushion and weapons after respawn.
        if (playerIsDead(player))
            Buffed[idx] = 0;

        // Apply the owner-local cushion and weapon upgrades once per life.
        if (!Buffed[idx] && !playerIsDead(player) && player->isLocal) {
            JuggyCushion = JUGGERNAUT_CUSHION_HP;

            int w;
            for (w = 0; w < (int)JUGGERNAUT_WEAPON_COUNT; ++w)
                playerGiveWeaponUpgrade(player, JUGGERNAUT_WEAPONS[w]);

            Buffed[idx] = 1;
#if JUGGERNAUT_REFILL_ON_HEAL
            JuggyLastHp = playerGetHealth(player);  // seed so respawn/crown HP isn't seen as a "heal"
#endif
        }

#if JUGGERNAUT_REFILL_ON_HEAL
        // A mid-life health pickup restores one absorbed hit.
        if (Buffed[idx] && !playerIsDead(player) && player->isLocal) {
            int hp = playerGetHealth(player);
            if (hp > JuggyLastHp && JuggyCushion < 1)
                JuggyCushion = 1;
            JuggyLastHp = hp;
        }
#endif

#if JUGGERNAUT_JUGGY_SHIELD
        // The owner derives shield state from its cushion; all clients render it.
        if (player->isLocal)
            juggyUpdateShieldState(!playerIsDead(player) && JuggyCushion > 0);

        // playerGiveShield() creates a cosmetic moby; shieldTrigger() would alter
        // networked player state.
        {
            int show = JuggyShieldOn && !playerIsDead(player)
                       && player->pMoby && player->pMoby->pVar;
#if !JUGGERNAUT_JUGGY_SHIELD_LOCAL
            if (player->isLocal)
                show = 0;   // keep the bubble off the juggy's own view
#endif
            if (show) {
                if (!playerHasShield(player))
                    playerGiveShield(player);
            } else if (playerHasShield(player)) {
                juggyRemoveShield(player);
            }
        }
#endif
    } else {
        // Clear state when this player is no longer the juggy.
        if (player->isLocal)
            JuggyCushion = 0;
        Buffed[idx] = 0;
    }
}

//--------------------------------------------------------------------------
static void setLobbyGameOptions(void)
{
    GameOptions * gameOptions = gameGetOptions();
    GameSettings * gameSettings = gameGetSettings();
    if (!gameOptions || !gameSettings || gameSettings->GameLoadStartTime <= 0)
        return;

    // Preserve DM/CTF/Siege; scoring is gated separately for non-DM modes.
    gameOptions->GameFlags.MultiplayerGameFlags.Chargeboots = 1;
    gameOptions->GameFlags.MultiplayerGameFlags.PlayerNames = 1;
}

//--------------------------------------------------------------------------
static void resetState(void)
{
    StateSeq = 0;
    JuggyIndex = -1;
    PickupActive = 0;
    memset(PickupPos, 0, sizeof(PickupPos));

    HostDropPending = 0;
    HostDropAtTime = 0;
    HostDropHasPos = 0;
    memset(HostDropPos, 0, sizeof(HostDropPos));
    HostStaleAtTime = 0;
    HostNextHeartbeat = 0;
    HostStarted = 0;
    HostMatchStartTime = 0;
    HostFirstPickupDone = 0;

    memset(Buffed, 0, sizeof(Buffed));
    JuggyCushion = 0;
#if JUGGERNAUT_REFILL_ON_HEAL
    JuggyLastHp = 0;
#endif

    JuggyShieldOn = 1;
    ShieldLastSent = -1;
    ShieldNextResend = 0;

    crownSpriteDestroy();
    juggMapSetVisible(0);   // hide the minimap icon between matches (widget kept)
}

//--------------------------------------------------------------------------
static void initialize(void)
{
    setLobbyGameOptions();
    resetState();

    if (!HandlerInstalled) {
        netInstallCustomMsgHandler(CROWN_MSG_ID_STATE, &crownOnReceiveState);
        netInstallCustomMsgHandler(CROWN_MSG_ID_SHIELD, &crownOnReceiveShield);
        HandlerInstalled = 1;
    }

    // Patch the weapon-hit hurt call so the juggy's cushion can absorb damage.
    juggyInstallHurtHook();

#if JUGGERNAUT_JUGGY_ONLY_KILLS
    // Gate frag scoring to the juggy -- but only in DM, where frags are the
    // objective. In CTF/Siege the objective is flags/nodes, so leave kills scoring
    // normally and let the crown be a pure buff (hook simply not installed).
    {
        GameSettings * gs = gameGetSettings();
        if (gs && gs->GameType == GAMETYPE_DM)
            juggyInstallKillHook();
    }
#endif

    Initialized = 1;
}

//--------------------------------------------------------------------------
static void teardown(void)
{
    // Restore hooks before returning to the lobby.
    juggyUninstallHurtHook();
#if JUGGERNAUT_JUGGY_ONLY_KILLS
    juggyUninstallKillHook();
#endif
    resetState();
    Initialized = 0;
}

//--------------------------------------------------------------------------
void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
    GameSettings * gameSettings = gameGetSettings();

    // Tear down when leaving the match.
    if (!gameSettings || !isInGame()) {
        if (Initialized)
            teardown();
        return;
    }

    // Reinitialize if a new match loaded without teardown.
    if (!Initialized) {
        initialize();
        LastGameLoadTime = gameSettings->GameLoadStartTime;
    } else if (gameSettings->GameLoadStartTime != LastGameLoadTime) {
        teardown();
        initialize();
        LastGameLoadTime = gameSettings->GameLoadStartTime;
    }

    if (!gameHasEnded()) {
        int gameTime = gameGetTime();

        // The host advances state; clients receive it by broadcast.
        if (gameAmIHost())
            juggyHostTick(gameTime);

        crownSpriteSync();
        // Retry until the HUD root exists.
        juggMapInit();
        juggMapUpdate();

        Player ** players = playerGetAll();
        int i;
        for (i = 0; i < GAME_MAX_PLAYERS; ++i)
            processPlayer(players[i]);

#if JUGGERNAUT_JUGGY_SHIELD
        // Remove shields orphaned by transfer, death, or disconnect.
        juggySweepStrayShields();
#endif

#if JUGG_DEBUG
        // Optional live diagnostic for confirming the loaded module.
        {
            static int nextDbg = 0;
            if (gameTime >= nextDbg) {
                nextDbg = gameTime + (2 * TIME_SECOND);
                char b[96];
                sprintf(b, "JUGGv5 jug=%d hk=%d cu=%d cl=%d ab=%d ld=%d",
                        JuggyIndex, HurtHookCount, JuggyCushion, HookCalls, HookAbsorbs, HookLastDmg);
                for (i = 0; i < GAME_MAX_PLAYERS; ++i)
                    if (players[i] && players[i]->isLocal)
                        uiShowPopup(players[i], b, 2);
            }
        }
#endif
    }
}

//--------------------------------------------------------------------------
void lobbyStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
    UiMenu_t * menu;

    if (menu = uiGetActiveMenu(UI_MENU_STAGING, 0), menu != NULL) {
        if (Initialized)
            teardown();
        setLobbyGameOptions();
    }
}

//--------------------------------------------------------------------------
void loadStart(struct GameModule * module, PatchStateContainer_t * gameState)
{
    setLobbyGameOptions();
}
