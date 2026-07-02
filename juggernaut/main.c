/***************************************************
 * FILENAME :        main.c
 * DESCRIPTION :
 *      Juggernaut custom game mode.
 *
 *      DM-based mode built around a physical "crown" pickup. Whoever holds the
 *      crown is the juggernaut (juggy): they get double max health and their
 *      carried base weapons upgraded to V2. When the juggy dies the crown drops
 *      at their death location after a short delay; whoever touches it next
 *      becomes the juggy.
 *
 *      Authority is host-owned (KOTH-style). The host runs the crown state
 *      machine -- spawn point, grab detection, drop-on-death -- and broadcasts a
 *      heartbeat snapshot { seq, juggyIdx, pickupActive, pickupPos }. Every
 *      client (and any future host after migration) applies the newest snapshot,
 *      sets JuggyIndex, and renders a local cosmetic sprite. Because the full
 *      state rides the heartbeat, a reassigned host resumes seamlessly.
 *
 *      The buff layer (processPlayer) only ever reads JuggyIndex, so it is
 *      agnostic to how the crown is decided.
 *
 * NOTES :
 *      Compile-time options (promote to game rules later):
 *        JUGGERNAUT_JUGGY_SHIELD      - give the juggy a shield bubble as a
 *                                       visual marker + extra survivability
 *                                       (default on; real shield, clears on death).
 *        JUGGERNAUT_BRUISER_SKIN      - juggy's hero skin becomes Bruiser
 *                                       (default off; crashes mid-match, needs
 *                                       a proper in-game model swap).
 *      See DESIGN.md for the full specification.
 */

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
// Diagnostic: throttled on-screen status line (host?/pickup?/juggy?/spawns?).
// Set to 0 to ship.
#define JUGG_DEBUG                      1

// Skin swap is disabled: changing a live player's skin via gameSetClientSkin()
// fires a lobby-settings network update that crashes the client mid-match. A
// proper in-game model/skin swap needs to be REd separately before re-enabling.
#define JUGGERNAUT_BRUISER_SKIN         0

// Give the juggy a shield bubble. DISABLED: playerGiveShield spawns a LOCAL
// shield (only visible on the owner's screen) and grants owner-side damage
// absorption/invincibility -- the juggy became unkillable, so the crown could
// never transfer. The juggy is marked by a floating crown orb instead.
#define JUGGERNAUT_JUGGY_SHIELD         0

#define JUGGERNAUT_HEALTH_MULTIPLIER    2

// UYA hero base max health is a fixed constant (PLAYER_MAX_HEALTH == 15, stored
// as a float). DM is uniform, so we use the constant directly.
#define JUGGERNAUT_BASE_HEALTH          PLAYER_MAX_HEALTH
#define JUGGERNAUT_BUFFED_HEALTH        (JUGGERNAUT_BASE_HEALTH * JUGGERNAUT_HEALTH_MULTIPLIER)

// ---- Crown pickup tuning --------------------------------------------------
// NOTE: 3D/GS draw colors are 0xAABBGGRR (alpha, blue, green, RED-low-byte) --
// NOT 0xAARRGGBB. Gold (R=255,G=208,B=39) is therefore 0x__27D0FF.
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
#define CROWN_DEATH_DROP_DELAY_MS       (1 * TIME_SECOND)

// Local (per-client) custom message id -- both sender and receiver are this
// module, and only one custom mode runs at a time, so the game-mode range is
// safe to use without editing common/messageid.h (keeps changes module-local).
#define CROWN_MSG_ID_STATE              (CUSTOM_MSG_ID_GAME_MODE_START)

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

// ---- Billboard texture-swap plumbing (mirrors scavengerhunt.c) ------------
// gfxDrawBillboardQuad with the frame texture (81) requires temporarily
// swapping the effect-texture getter for the frame-texture getter around the
// draw call. vaGetFrameTex / vaGetEffectTex live in libuya (linked); the JAL
// call-site table is per-map and copied from patch/scavengerhunt.c.
extern VariableAddress_t vaGetFrameTex;
extern VariableAddress_t vaGetEffectTex;

// Line-beam API from libuya (draw.c). The types live in draw.c (not a header),
// so we mirror them here with the SAME field types/order so the ABI matches.
// The beam uses the normal effect-texture getter -- no frame-texture swap.
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

// ---- State ----------------------------------------------------------------
static int Initialized = 0;
static int HandlerInstalled = 0;

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

// Per-player buff bookkeeping.
static char   Buffed[GAME_MAX_PLAYERS];             // one-time heal + weapon upgrades this life?
static char   MaxHealthSet[GAME_MAX_PLAYERS];       // 2x max-health applied for this juggy?

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
// Pick a random, validated player-spawn position. Falls back to a living
// player's position if no sane spawn cuboid is found. Returns 1 on success.
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
// Vertical beacon beam -- straight base-light pillar, fades out toward the top.
// No frame-texture swap needed (drawLines uses the normal effect-texture getter).
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

    // Tall beacon pillar only for the un-held pickup; the juggy marker is just
    // the orb hovering over their head.
    if (CrownShowBeam)
        crownDrawBeam(moby->position);

    // The orb billboard uses the frame texture, which needs a per-map JAL swap.
    // Guard the addresses: on maps not in the table GetAddress returns 0, and
    // hooking at 0x20 / jumping to 0 would corrupt low memory (TLB miss).
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
    // Two visuals, driven by the synced state:
    //   - un-held pickup  -> tall pillar + orb at the pickup point
    //   - held (juggy)     -> orb hovering over the juggy's head (cross-client
    //                         marker; player positions are networked)
    // No gfxSpawnParticle here: holding PartInstance_t* across frames on a
    // persistent moby risks a double-free that corrupts the particle pool.
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

// Keep a single persistent crown moby alive for the whole match. Its update
// hides/shows itself from PickupActive, so we never spawn/destroy per pickup
// (which risked a destroy-during-deferred-draw fault). Destroyed on teardown.
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

static int crownOnReceiveState(void * connection, void * data)
{
    // The host is authoritative and never applies incoming snapshots.
    if (gameAmIHost())
        return sizeof(CrownStateMsg_t);

    CrownStateMsg_t * msg = (CrownStateMsg_t*)data;

    // Only accept newer (or equal, for idempotent heartbeats) snapshots.
    if (msg->Seq < StateSeq)
        return sizeof(CrownStateMsg_t);

    StateSeq = msg->Seq;

    int newJuggy = msg->JuggyIdx;
    if (newJuggy != JuggyIndex && newJuggy >= 0 && newJuggy < GAME_MAX_PLAYERS)
        announceJuggy(newJuggy);
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

    // 1. Crowned: watch for the juggy dying or disconnecting.
    if (JuggyIndex >= 0 && JuggyIndex < GAME_MAX_PLAYERS) {
        Player * jp = players[JuggyIndex];
        if (!jp) {
            hostBeginDeathDrop(gameTime, NULL, 0);            // disconnect -> random relocate
        } else if (playerIsDead(jp)) {
            int hasPos = vectorIsSane(jp->playerPosition);
            hostBeginDeathDrop(gameTime, jp->playerPosition, hasPos);
        }
    }

    // 2. Death drop pending: materialize the pickup once the arm delay elapses.
    if (HostDropPending && gameTime >= HostDropAtTime) {
        VECTOR pos;
        if (HostDropHasPos)
            vector_copy(pos, HostDropPos);
        else if (!pickRandomSpawnPos(pos)) {
            // No sane location yet; retry next tick.
            HostDropAtTime = gameTime + CROWN_HEARTBEAT_MS;
            return;
        }
        hostActivatePickup(pos, gameTime);
    }

    // 3. Self-heal / match start: no juggy, no pickup, nothing pending -> spawn.
    if (JuggyIndex < 0 && !PickupActive && !HostDropPending) {
        VECTOR pos;
        if (pickRandomSpawnPos(pos))
            hostActivatePickup(pos, gameTime);
    }

    // 4. Pickup active: grab detection + stale relocate.
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

    // 5. Periodic heartbeat so late joiners / a migrated host converge.
    if (gameTime >= HostNextHeartbeat) {
        crownBroadcastState();
        HostNextHeartbeat = gameTime + CROWN_HEARTBEAT_MS;
    }
}

// ==========================================================================
//  Per-player buffs (unchanged from v1 -- only reads JuggyIndex)
// ==========================================================================
// libuya's playerSetMaxHealth() POKEs a single global respawn instruction
// shared by every player; using it for a per-player buff corrupts everyone's
// respawn health. We only touch this player's own obfuscated cap + pVar bar.
static void juggySetMaxHealth(Player * player, int health)
{
    // Never write to a dead player's moby: during death the hero moby/pVar can
    // be torn down (dangling pointer), and writing pVar+0x30 there corrupts
    // memory -> later null load in game code (the pc=0x49e1fc TLB miss on juggy
    // death). Respawn re-initializes max health anyway, so deferring is safe.
    if (!player || playerIsDead(player) || !player->pMoby || !player->pMoby->pVar)
        return;

    playerObfuscate((u32)&player->maxHP, health, OBFUSCATE_MODE_HEALTH);
    *(float*)((u32)player->pMoby->pVar + 0x30) = (float)health;
}

static void processPlayer(Player * player)
{
    if (!player)
        return;

    int idx = player->mpIndex;
    if (idx < 0 || idx >= GAME_MAX_PLAYERS)
        return;

    if (idx == JuggyIndex) {
        // Raise max health once per juggy role (per-player, no global POKE).
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

#if JUGGERNAUT_JUGGY_SHIELD
            // Only the owning client gives the shield (it spawns a replicated
            // moby). Guard against double-giving within a life.
            if (!playerHasShield(player))
                playerGiveShield(player);
#endif

            Buffed[idx] = 1;
        }
    } else {
        // Not (or no longer) the juggy: restore base max health once. V2 weapons
        // clear naturally on the next respawn.
        if (MaxHealthSet[idx]) {
            juggySetMaxHealth(player, JUGGERNAUT_BASE_HEALTH);
            MaxHealthSet[idx] = 0;
        }
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

    gameSettings->GameType = GAMETYPE_DM;
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

    memset(Buffed, 0, sizeof(Buffed));
    memset(MaxHealthSet, 0, sizeof(MaxHealthSet));

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
        HandlerInstalled = 1;
    }

    Initialized = 1;
}

//--------------------------------------------------------------------------
static void teardown(void)
{
    resetState();
    Initialized = 0;
}

//--------------------------------------------------------------------------
void gameStart(struct GameModule * module, PatchConfig_t * config, PatchGameConfig_t * gameConfig, PatchStateContainer_t * gameState)
{
    GameSettings * gameSettings = gameGetSettings();

    // Ensure in game
    if (!gameSettings || !isInGame()) {
        if (Initialized)
            teardown();
        return;
    }

    if (!Initialized)
        initialize();

    if (!gameHasEnded()) {
        int gameTime = gameGetTime();

        // Host advances the authoritative crown state; clients get it via broadcast.
        if (gameAmIHost())
            juggyHostTick(gameTime);

        // Render the local sprite from current state, then apply per-player buffs.
        crownSpriteSync();
        // Init runs every tick (idempotent): the radar HUD root may not exist on
        // the first in-game ticks, so we retry until the widget sticks -- same
        // as koth calling radarInit() each frame.
        juggMapInit();
        juggMapUpdate();

        Player ** players = playerGetAll();
        int i;
        for (i = 0; i < GAME_MAX_PLAYERS; ++i)
            processPlayer(players[i]);

#if JUGG_DEBUG
        // Live status on each local screen: is this client host, is a pickup
        // active, who's juggy, how many spawn cuboids exist. Reveals exactly
        // where the crown chain stalls.
        {
            static int nextDbg = 0;
            if (gameTime >= nextDbg) {
                nextDbg = gameTime + (2 * TIME_SECOND);
                char b[80];
                sprintf(b, "JUGGv2 h=%d act=%d jug=%d spn=%d seq=%d mob=%d",
                        gameAmIHost(), PickupActive, JuggyIndex, spawnPointGetCount(), StateSeq,
                        (CrownMoby != NULL));
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
