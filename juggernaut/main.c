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
 *        JUGGERNAUT_JUGGY_SHIELD      - cosmetic shield bubble on the juggy,
 *                                       drawn by every client EXCEPT the juggy's
 *                                       own screen (default on; owner-side sim is
 *                                       untouched, so the juggy stays killable).
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
#define JUGG_DEBUG                      0

// Skin swap is disabled: changing a live player's skin via gameSetClientSkin()
// fires a lobby-settings network update that crashes the client mid-match. A
// proper in-game model/skin swap needs to be REd separately before re-enabling.
#define JUGGERNAUT_BRUISER_SKIN         0

// Give the juggy a cosmetic shield bubble as a "still-buffed" tell. It shows
// while the juggy is above base max health and DROPS once they've been hurt to
// <= normal max (JUGGERNAUT_BASE_HEALTH) -- i.e. their 2x cushion is gone and
// they're now killable at normal health, a visible "vulnerable now" signal.
// Health is never networked, so the juggy's OWN client (the only one that knows
// its health) decides on/off and broadcasts the flag; every client applies the
// bubble from it. playerGiveShield() spawns a client-LOCAL moby and grants
// owner-side absorption, so we only ever apply it to REMOTE mirrors (purely
// visual, juggy stays killable) unless JUGGERNAUT_JUGGY_SHIELD_LOCAL is set.
#define JUGGERNAUT_JUGGY_SHIELD         1

// Also render the bubble on the juggy's OWN screen (unlike the crown orb, which
// is hidden locally). Off by default -- it can obscure the juggy's own view.
#define JUGGERNAUT_JUGGY_SHIELD_LOCAL   1

#define JUGGERNAUT_HEALTH_MULTIPLIER    2

// UYA hero base max health is a fixed constant (PLAYER_MAX_HEALTH == 15, stored
// as a float). DM is uniform, so we use the constant directly.
#define JUGGERNAUT_BASE_HEALTH          PLAYER_MAX_HEALTH
#define JUGGERNAUT_BUFFED_HEALTH        (JUGGERNAUT_BASE_HEALTH * JUGGERNAUT_HEALTH_MULTIPLIER)

// Damage cushion (the "2x health" buff, done WITHOUT writing health/maxHP).
// Instead of raising the juggy's max health -- which requires obfuscated writes
// to player->hitPoints/maxHP that share a self-modifying global table with
// player STATE, and corrupt it (juggy ends up invincible + unable to shoot) --
// we intercept weapon damage on the juggy's own client and absorb the first
// JUGGERNAUT_CUSHION_HP points before letting damage through normally. Net
// effect is ~2x survivability with ZERO health/state writes. Damage is in
// integer health points (base bar == 15), so a full bar of cushion == 2x.
#define JUGGERNAUT_CUSHION_HP           JUGGERNAUT_BASE_HEALTH

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
// Hold the FIRST pickup this long after match start so players spread out from
// their spawn cuboids before the crown appears -- avoids an instant "spawned on
// the pickup" crown. Applies only to the initial pickup, not mid-match respawns.
#define CROWN_FIRST_PICKUP_DELAY_MS     (10 * TIME_SECOND)

// Local (per-client) custom message id -- both sender and receiver are this
// module, and only one custom mode runs at a time, so the game-mode range is
// safe to use without editing common/messageid.h (keeps changes module-local).
#define CROWN_MSG_ID_STATE              (CUSTOM_MSG_ID_GAME_MODE_START)
// Juggy shield on/off flag, broadcast by the juggy's own client (see shield block).
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

// Damage cushion for the current juggy, tracked on the juggy's OWN client (damage
// is owner-authoritative, so the weapon-hit hook only ever fires meaningfully there).
// Refilled once per juggy life; decremented by absorbed weapon damage; shield tell
// is shown while > 0. Replaces the old obfuscated-health buff entirely.
static int    JuggyCushion = 0;

// ---- Guarded-hurt hook ----------------------------------------------------
// We patch the WEAPON-HIT `jal vaHurtPlayerFunc` inside the per-frame player
// health-reduction function. Networked PvP weapon damage did NOT go through the
// single site we tried (an in-game counter showed 0 hits), so instead of guessing
// which of the ~9 call sites carries it, we hook EVERY `jal vaHurtPlayerFunc` in
// the overlay. They all pass (a0=player, a1=damage) identically -- it's the same
// callee -- so one hook works at all of them, and the juggy check means only the
// juggy absorbs; every other call passes straight through unchanged. Catches
// whatever path PvP damage uses (weapon, collision, hazard) without site ID.
#define JUGGY_HURT_SCAN_RANGE   (0x20000)   // sites span ~HF-0x19000 .. HF+0x1f000
#define JUGGY_HURT_MAX_SITES    (24)        // ~9 expected; headroom

static u32  HurtHookSites[JUGGY_HURT_MAX_SITES];  // patched call-site addresses
static u32  HurtHookSaved[JUGGY_HURT_MAX_SITES];  // their original words, for teardown
static int  HurtHookCount = 0;                    // 0 = not installed
static u32  HurtFuncAddr  = 0;                     // resolved vaHurtPlayerFunc (direct passthrough, no per-hit re-resolve)

// libuya's map-resolved trampoline to the real vaHurtPlayerFunc (see functions.S),
// plus the per-map address table itself (used to build the jal we scan for).
extern void internal_HurtPlayer(Player * player, int damage);
extern VariableAddress_t vaHurtPlayerFunc;

// Shield tell: is the current juggy above base max health? Computed on the
// juggy's own client, mirrored from broadcast elsewhere. Default on (a freshly
// crowned juggy is at full buffed health until they take damage).
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

// Broadcast the juggy shield flag. Sent by the juggy's own client only.
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

// Update the local shield flag and broadcast it on change (plus a periodic
// re-send so late joiners / dropped packets converge). Called on the juggy's
// own client, which is the sole authority for its own health.
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

// A freshly crowned juggy starts at full buffed health -> shielded. Force the
// next tick to (re)broadcast so remote clients converge immediately.
static void juggyResetShieldForNewJuggy(void)
{
    JuggyShieldOn = 1;
    ShieldLastSent = -1;
    ShieldNextResend = 0;
}

static int crownOnReceiveShield(void * connection, void * data)
{
    CrownShieldMsg_t * msg = (CrownShieldMsg_t*)data;
    // Trust the flag only for the current juggy (ignore stale/foreign senders).
    if (msg->JuggyIdx == JuggyIndex)
        JuggyShieldOn = msg->ShieldOn ? 1 : 0;
    return sizeof(CrownShieldMsg_t);
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

    // Stamp match start on the host's first tick (drives the initial-pickup grace).
    if (!HostStarted) {
        HostStarted = 1;
        HostMatchStartTime = gameTime;
    }

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
    // The FIRST pickup is held for CROWN_FIRST_PICKUP_DELAY_MS after match start so
    // players move off their spawn cuboids before the crown appears (no instant
    // spawn-on-pickup crown). Mid-match respawns (HostFirstPickupDone) skip the wait.
    if (JuggyIndex < 0 && !PickupActive && !HostDropPending) {
        if (HostFirstPickupDone || gameTime >= HostMatchStartTime + CROWN_FIRST_PICKUP_DELAY_MS) {
            VECTOR pos;
            if (pickRandomSpawnPos(pos)) {
                hostActivatePickup(pos, gameTime);
                HostFirstPickupDone = 1;
            }
        }
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
//  Guarded-hurt hook -- the "2x health" buff, without touching health/state
// ==========================================================================
// Replaces the WEAPON-HIT `jal vaHurtPlayerFunc` in the player damage
// dispatcher. Args arrive exactly as the engine set them up: a0 = player,
// a1 = damage. For the juggy with cushion remaining, we swallow the damage into
// JuggyCushion and return WITHOUT calling the real hurt -> no health/state write,
// so nothing corrupts. Knockback is applied by the projectile impact separately
// (not by this call), so it is preserved -- the juggy still visibly reacts.
// Everyone else (and the juggy once the cushion is spent) passes straight through.
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

    // Normal damage. Call the resolved address directly (cached at install) to
    // avoid re-resolving the map address on every hit; fall back to the trampoline
    // only if somehow uncached.
    if (HurtFuncAddr)
        ((void (*)(Player *, int))HurtFuncAddr)(player, damage);
    else
        internal_HurtPlayer(player, damage);
}

static void juggyUninstallHurtHook(void)
{
    // Only restore a site that STILL holds our hook. If the overlay was reloaded
    // (map change) under stale addresses, those slots now hold unrelated code --
    // writing the old jal back would corrupt it, so skip any that aren't ours.
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

    // Hook EVERY jal to vaHurtPlayerFunc in a window around it. Matching only the
    // exact jal word means we never patch anything but a real call to that
    // function. If none are found (unexpected build), nothing is patched and the
    // cushion is simply disabled -- never a crash.
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

#if JUGGERNAUT_JUGGY_SHIELD
// Destroy this player's cosmetic shield moby, if present. Mirrors the scan in
// playerHasShield (shield moby stores its owner at pVar+0x40).
static void juggyRemoveShield(Player * player)
{
    Moby * shield = mobyListGetStart();
    while ((shield = mobyFindNextByOClass(shield, MOBY_ID_OMNI_SHIELD))) {
        if (shield->pVar && *(u32*)((u32)shield->pVar + 0x40) == (u32)player)
            mobyDestroy(shield);
        ++shield;
    }
}

// Disconnect / crown-transfer safety. The engine's shield update
// (shieldEffectUpdateSourceJoint) reads the OWNER player's moby joint every frame,
// and its draw path calls the HUD-quad / vec-lerp / sine-motion helpers. An
// ex-juggy or a disconnected player leaves a shield whose owner pointer
// (pVar+0x40) now dangles -> that per-frame read hits freed/unmapped memory -> the
// repeating TLB miss seen on disconnect. Fix: each frame, destroy any shield not
// owned by the current LIVE juggy. We only COMPARE the stored owner pointer, never
// dereference it, so the sweep is safe even when the owner has been freed. Cheap:
// one oclass-list walk (normally 0-1 shields), same cost as the give/remove scans.
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
        // Reset Buffed on death so the cushion + weapons re-apply after respawn.
        if (playerIsDead(player))
            Buffed[idx] = 0;

        // Once per life (owner only): refill the damage cushion + upgrade weapons.
        // No health/maxHP writes -- the cushion (guarded weapon-hit hook) provides
        // the effective 2x survivability without touching the obfuscated fields
        // that share a table with player STATE. V2 weapon upgrades don't touch
        // that table, so they stay.
        if (!Buffed[idx] && !playerIsDead(player) && player->isLocal) {
            JuggyCushion = JUGGERNAUT_CUSHION_HP;

            int w;
            for (w = 0; w < (int)JUGGERNAUT_WEAPON_COUNT; ++w)
                playerGiveWeaponUpgrade(player, JUGGERNAUT_WEAPONS[w]);

            Buffed[idx] = 1;
        }

#if JUGGERNAUT_JUGGY_SHIELD
        // Cosmetic shield "still-buffed" tell, driven off the remaining cushion
        // (a truthful "still cushioned / vulnerable-soon" signal). The juggy's own
        // client owns the cushion, decides on/off, and broadcasts the flag; every
        // client renders the bubble from JuggyShieldOn.
        // Owner computes on/off from its cushion and broadcasts JuggyShieldOn.
        if (player->isLocal)
            juggyUpdateShieldState(!playerIsDead(player) && JuggyCushion > 0);

        // Draw the cosmetic bubble on every client via playerGiveShield -- the
        // omni-shield moby (0x1b37) has no gameplay callbacks, so it is purely
        // visual on local AND remote (NOT shieldTrigger, which does a real
        // networked equip and desynced player state). Because the cushion now
        // absorbs all damage, the juggy can't die while shielded, so the bubble is
        // removed on cushion-spent while ALIVE -- not torn down mid-loop on death
        // (the old "sound loops on death" case, which only happened back when the
        // cushion didn't work and the juggy died shielded).
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
        // Not (or no longer) the juggy: clear the per-life latch. No health to
        // restore -- we never changed max health. Zero a stale cushion on our own
        // ex-juggy (harmless otherwise: the hook only absorbs while idx==JuggyIndex).
        // V2 weapons clear naturally on the next respawn.
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
    HostStarted = 0;
    HostMatchStartTime = 0;
    HostFirstPickupDone = 0;

    memset(Buffed, 0, sizeof(Buffed));
    JuggyCushion = 0;

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

    Initialized = 1;
}

//--------------------------------------------------------------------------
static void teardown(void)
{
    // Restore the original hurt call site before we leave (belt-and-suspenders:
    // the level overlay is reloaded per match anyway, but our handler lives in
    // this overlay, so don't leave a live jump into it during the lobby window).
    juggyUninstallHurtHook();
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

    // Fresh init on first entry, OR when the match changed but teardown never
    // fired (e.g. a map transition that kept isInGame() true) -- GameLoadStartTime
    // changes per match load, so a mismatch means a new game and we must clear
    // stale crown state (otherwise last match's JuggyIndex carries over).
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

#if JUGGERNAUT_JUGGY_SHIELD
        // Reap any shield left orphaned by a crown transfer, death, or disconnect
        // before the engine's shield update dereferences its freed owner.
        juggySweepStrayShields();
#endif

#if JUGG_DEBUG
        // Live status on each local screen. "JUGGv3" marker confirms THIS build is
        // running (vs a stale module). hook=1 means the weapon-hit scan installed;
        // @=low16 of the site it patched; cush=current cushion. If the popup is
        // absent or says an older marker, the game is running an old .bin.
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
