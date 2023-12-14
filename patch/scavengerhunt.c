#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/stdio.h>
#include <libuya/string.h>
#include <libuya/team.h>
#include <libuya/ui.h>
#include <libuya/pad.h>
#include <libuya/utils.h>
#include <libuya/graphics.h>
#include <libuya/player.h>
#include <libuya/collision.h>
#include <libuya/color.h>
#include <libuya/net.h>
#include <libuya/time.h>
#include <libuya/random.h>
#include <libuya/interop.h>
#include "messageid.h"
#include "module.h"
#include "config.h"

#define HBOLT_MOBY_OCLASS				    (0x1C0D)
#define HBOLT_PICKUP_RADIUS			    (3)
#define HBOLT_PARTICLE_COLOR        (0x0000ffff)
#define HBOLT_SPRITE_COLOR          (0x00131341)
#define HBOLT_SCALE                 (0.5)

#define HBOLT_MIN_SPAWN_DELAY_TPS   (60 * 10)
#define HBOLT_MAX_SPAWN_DELAY_TPS   (60 * 20)
#define HBOLT_SPAWN_PROBABILITY     (0.25)

VariableAddress_t vaOnPlayerKill_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004fa1ec,
	.Hoven = 0x004fc304,
	.OutpostX12 = 0x004f1bdc,
	.KorgonOutpost = 0x004ef374,
	.Metropolis = 0x004ee6c4,
	.BlackwaterCity = 0x004ebf5c,
	.CommandCenter = 0x004ebf24,
	.BlackwaterDocks = 0x004ee7a4,
	.AquatosSewers = 0x004edaa4,
	.MarcadiaPalace = 0x004ed424,
#else
	.Lobby = 0,
	.Bakisi = 0x004f7a6c,
	.Hoven = 0x004f9ac4,
	.OutpostX12 = 0x004ef3dc,
	.KorgonOutpost = 0x004ecbf4,
	.Metropolis = 0x004ebf44,
	.BlackwaterCity = 0x004e975c,
	.CommandCenter = 0x004e98e4,
	.BlackwaterDocks = 0x004ec124,
	.AquatosSewers = 0x004eb464,
	.MarcadiaPalace = 0x004eada4,
#endif
};

VariableAddress_t vaOnPlayerKill_Func = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00544488,
	.Hoven = 0x00546650,
	.OutpostX12 = 0x0053bf28,
	.KorgonOutpost = 0x00539610,
	.Metropolis = 0x00538a10,
	.BlackwaterCity = 0x005361f8,
	.CommandCenter = 0x00535a50,
	.BlackwaterDocks = 0x005382d0,
	.AquatosSewers = 0x005375d0,
	.MarcadiaPalace = 0x00536f50,
#else
	.Lobby = 0,
	.Bakisi = 0x00541b78,
	.Hoven = 0x00543c80,
	.OutpostX12 = 0x00539598,
	.KorgonOutpost = 0x00536d00,
	.Metropolis = 0x00536100,
	.BlackwaterCity = 0x00533868,
	.CommandCenter = 0x00533298,
	.BlackwaterDocks = 0x00535ad8,
	.AquatosSewers = 0x00534e18,
	.MarcadiaPalace = 0x00534758,
#endif
};

VariableAddress_t vaEmpty_TROY_IS_AMAZING = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0,
    .Hoven = 0,
    .OutpostX12 = 0,
    .KorgonOutpost = 0,
    .Metropolis = 0,
    .BlackwaterCity = 0,
    .CommandCenter = 0,
    .BlackwaterDocks = 0,
    .AquatosSewers = 0,
    .MarcadiaPalace = 0,
#else
    .Lobby = 0,
    .Bakisi = 0,
    .Hoven = 0,
    .OutpostX12 = 0,
    .KorgonOutpost = 0,
    .Metropolis = 0,
    .BlackwaterCity = 0,
    .CommandCenter = 0,
    .BlackwaterDocks = 0,
    .AquatosSewers = 0,
    .MarcadiaPalace = 0,
#endif
};

VariableAddress_t vaSpawnPart_059 = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004a0508,
    .Hoven = 0x004a2620,
    .OutpostX12 = 0x00497ef8,
    .KorgonOutpost = 0x00495690,
    .Metropolis = 0x004949e0,
    .BlackwaterCity = 0x00492278,
    .CommandCenter = 0x00492270,
    .BlackwaterDocks = 0x00494af0,
    .AquatosSewers = 0x00493df0,
    .MarcadiaPalace = 0x00493770,
#else
    .Lobby = 0,
    .Bakisi = 0x0049e150,
    .Hoven = 0x004a01a8,
    .OutpostX12 = 0x00495ac0,
    .KorgonOutpost = 0x004932d8,
    .Metropolis = 0x00492628,
    .BlackwaterCity = 0x0048fe40,
    .CommandCenter = 0x0048fff8,
    .BlackwaterDocks = 0x00492838,
    .AquatosSewers = 0x00491b78,
    .MarcadiaPalace = 0x004914b8,
#endif
};

VariableAddress_t vaDeletePart = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00496c58,
    .Hoven = 0x00498d70,
    .OutpostX12 = 0x0048e648,
    .KorgonOutpost = 0x0048bd18,
    .Metropolis = 0x0048b130,
    .BlackwaterCity = 0x004889c8,
    .CommandCenter = 0x004889c0,
    .BlackwaterDocks = 0x0048b240,
    .AquatosSewers = 0x0048a540,
    .MarcadiaPalace = 0x00489ec0,
#else
    .Lobby = 0,
    .Bakisi = 0x00494a60,
    .Hoven = 0x00496ab8,
    .OutpostX12 = 0x0048c3d0,
    .KorgonOutpost = 0x00489b20,
    .Metropolis = 0x00488f38,
    .BlackwaterCity = 0x00486750,
    .CommandCenter = 0x00486908,
    .BlackwaterDocks = 0x00489148,
    .AquatosSewers = 0x00488488,
    .MarcadiaPalace = 0x00487dc8,
#endif
};

VariableAddress_t vaReplace_GetEffectTexJAL = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0045b2f0,
    .Hoven = 0x0045ce70,
    .OutpostX12 = 0x00453c70,
    .KorgonOutpost = 0x00451830,
    .Metropolis = 0x00450b70,
    .BlackwaterCity = 0x0044e370,
    .CommandCenter = 0x0044eff0,
    .BlackwaterDocks = 0x00451870,
    .AquatosSewers = 0x00450b70,
    .MarcadiaPalace = 0x004504f0,
#else
    .Lobby = 0,
    .Bakisi = 0x0045a220,
    .Hoven = 0x0045bce0,
    .OutpostX12 = 0x00452b20,
    .KorgonOutpost = 0x00450760,
    .Metropolis = 0x0044faa0,
    .BlackwaterCity = 0x0044d220,
    .CommandCenter = 0x0044e060,
    .BlackwaterDocks = 0x004508a0,
    .AquatosSewers = 0x0044fbe0,
    .MarcadiaPalace = 0x0044f520,
#endif
};

extern PatchConfig_t config;
extern PatchGameConfig_t gameConfig;
extern PatchStateContainer_t patchStateContainer;
extern VariableAddress_t vaGetFrameTex;
extern VariableAddress_t vaGetEffectTex;

float scavHuntSpawnFactor = 1;
float scavHuntSpawnTimerFactor = 1;
int scavHuntShownPopup = 0;
int scavHuntHasGotSettings = 0;
int scavHuntEnabled = 0;
int scavHuntInitialized = 0;
int scavHuntBoltSpawnCooldown = 0;


struct HBoltPVar {
	int DestroyAtTime;
	struct PartInstance* Particles[4];
};

//--------------------------------------------------------------------------
void scavHuntResetBoltSpawnCooldown(void)
{
  scavHuntBoltSpawnCooldown = randRangeInt(HBOLT_MIN_SPAWN_DELAY_TPS, HBOLT_MAX_SPAWN_DELAY_TPS) / scavHuntSpawnTimerFactor;
}

//--------------------------------------------------------------------------
int scavHuntRoll(void)
{
  float roll = (randRange(0, 1) / scavHuntSpawnFactor);
  DPRINTF("scavenger hunt rolled %f => %d\n", roll, roll <= HBOLT_SPAWN_PROBABILITY);
  return roll <= HBOLT_SPAWN_PROBABILITY;
}

//--------------------------------------------------------------------------
int scavHuntOnReceiveRemoteSettings(void* connection, void* data)
{
  ScavengerHuntSettingsResponse_t response;
  memcpy(&response, data, sizeof(response));

  scavHuntEnabled = response.Enabled;
  scavHuntSpawnFactor = maxf(response.SpawnFactor, 0.1);
  scavHuntSpawnTimerFactor = clamp(scavHuntSpawnFactor, 1, 30);
  DPRINTF("received scav hunt %d %f\n", scavHuntEnabled, scavHuntSpawnFactor);
}

//--------------------------------------------------------------------------
void scavHuntQueryForRemoteSettings(void)
{
  void* connection = netGetLobbyServerConnection();
  if (!connection) return;

  // reset, server will update
  scavHuntEnabled = 0;
  scavHuntSpawnFactor = 1;

  netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_REQUEST_SCAVENGER_HUNT_SETTINGS, 0, NULL);
}

//--------------------------------------------------------------------------
void scavHuntSendHorizonBoltPickedUpMessage(void)
{
  void* connection = netGetLobbyServerConnection();
  if (!connection) return;

  netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_PICKED_UP_HORIZON_BOLT, 0, NULL);
}

//--------------------------------------------------------------------------
struct PartInstance * scavHuntSpawnParticle(VECTOR position, u32 color, char opacity, int idx)
{
	// u32 a3 = *(u32*)0x002218E8;
	// u32 t0 = *(u32*)0x002218E4;
	// float f12 = *(float*)0x002218DC;
	// float f1 = *(float*)0x002218E0;

	// return ((struct PartInstance* (*)(VECTOR, u32, char, u32, u32, int, int, int, float))GetAddress(&vaSpawnPart_059))(position, color, opacity, a3, t0, -1, 0, 0, f12 + (f1 * idx));

	return ((struct PartInstance* (*)(VECTOR, u32, char, u32, u32, int, int, int, float))GetAddress(&vaSpawnPart_059))(position, color, opacity, 53, 0, 2, 0, 0, 1.25 + (0.5 * idx));
}

//--------------------------------------------------------------------------
void scavHuntDestroyParticle(struct PartInstance* particle)
{
	((void (*)(struct PartInstance*))GetAddress(&vaDeletePart))(particle);
}

//--------------------------------------------------------------------------
void scavHuntHBoltDestroy(Moby* moby)
{
  if (mobyIsDestroyed(moby)) return;
	struct HBoltPVar* pvars = (struct HBoltPVar*)moby->PVar;

	// destroy particles
  int i;
	for (i = 0; i < 4; ++i) {
		if (pvars->Particles[i]) {
			scavHuntDestroyParticle(pvars->Particles[i]);
			pvars->Particles[i] = NULL;
		}
	}

  mobyDestroy(moby);
}

//--------------------------------------------------------------------------
void scavHuntHBoltPostDraw(Moby* moby)
{
	// struct QuadDef quad;
	// MATRIX m2;
	// VECTOR t;
  int opacity = 0x80;
	struct HBoltPVar* pvars = (struct HBoltPVar*)moby->PVar;
	if (!pvars)
		return;

	// determine color
	u32 color = HBOLT_SPRITE_COLOR;

	// fade as we approach destruction
	int timeUntilDestruction = (pvars->DestroyAtTime - gameGetTime()) / TIME_SECOND;
	if (timeUntilDestruction < 1)
		timeUntilDestruction = 1;

	if (timeUntilDestruction < 10) {
		float speed = timeUntilDestruction < 3 ? 20.0 : 3.0;
		float pulse = (1 + sinf(clampAngle((gameGetTime() / 1000.0) * speed))) * 0.5;
		opacity = 32 + (pulse * 96);
	}

  opacity = opacity << 24;
  color = opacity | (color & HBOLT_SPRITE_COLOR);
  moby->PrimaryColor = color;

  u32 hook = (u32)GetAddress(&vaReplace_GetEffectTexJAL) + 0x20;
  HOOK_JAL(hook, GetAddress(&vaGetFrameTex));
  gfxDrawBillboardQuad(0.55, 0, MATH_PI, moby->Position, 81, opacity, 0);
  gfxDrawBillboardQuad(0.5, 0.01, MATH_PI, moby->Position, 81, color, 0);
  HOOK_JAL(hook, GetAddress(&vaGetEffectTex));
}

//--------------------------------------------------------------------------
void scavHuntHBoltUpdate(Moby* moby)
{
	const float rotSpeeds[] = { 0.05, 0.02, -0.03, -0.1 };
	const int opacities[] = { 64, 32, 44, 51 };
	VECTOR t;
	int i;
	struct HBoltPVar* pvars = (struct HBoltPVar*)moby->PVar;
	if (!pvars)
		return;

  gfxStickyFX(&scavHuntHBoltPostDraw, moby);

	// handle particles
	u32 color = colorLerp(0, HBOLT_PARTICLE_COLOR, 1.0 / 4);
	color |= 0x80000000;
	for (i = 0; i < 4; ++i) {
		struct PartInstance * particle = pvars->Particles[i];
		if (!particle) {
			particle = scavHuntSpawnParticle(moby->Position, color, 100, i);
		}

		// update
		if (particle) {
			particle->rot = (int)((gameGetTime() + (i * 100)) / (TIME_SECOND * rotSpeeds[i])) & 0xFF;
		}
	}

  // handle pickup
  for (i = 0; i < GAME_MAX_LOCALS; ++i) {
    Player* p = playerGetFromSlot(i);
    if (!p || playerIsDead(p)) continue;

    vector_subtract(t, p->PlayerPosition, moby->Position);
    if (vector_sqrmag(t) < (HBOLT_PICKUP_RADIUS * HBOLT_PICKUP_RADIUS)) {
      uiShowPopup(0, "You found a Horizon Bolt!", 3);
      // mobyPlaySoundByClass(1, 0, moby, MOBY_ID_PICKUP_PAD);
      // scavHuntSendHorizonBoltPickedUpMessage();
      scavHuntHBoltDestroy(moby);
      break;
    }
  }

	// handle auto destruct
	if (pvars->DestroyAtTime && gameGetTime() > pvars->DestroyAtTime) {
		scavHuntHBoltDestroy(moby);
	}
}

//--------------------------------------------------------------------------
void scavHuntSpawn(VECTOR position)
{
  Moby* moby = mobySpawn(HBOLT_MOBY_OCLASS, sizeof(struct HBoltPVar));
  if (!moby) return;

  moby->PUpdate = &scavHuntHBoltUpdate;
  vector_copy(moby->Position, position);
  moby->UpdateDist = -1;
  moby->Drawn = 1;
  moby->Opacity = 0x00;
  moby->DrawDist = 0x00;

  // update pvars
  struct HBoltPVar* pvars = (struct HBoltPVar*)moby->PVar;
  // pvars->DestroyAtTime = gameGetTime() + (TIME_SECOND * 30);
  memset(pvars->Particles, 0, sizeof(pvars->Particles));

  // mobySetState(moby, 0, -1);
  scavHuntResetBoltSpawnCooldown();
  // mobyPlaySoundByClass(0, 0, moby, MOBY_ID_NODE_BASE);
  DPRINTF("hbolt spawned at %08X destroyAt:%d %04X\n", (u32)moby, pvars->DestroyAtTime, moby->ModeBits);
}

//--------------------------------------------------------------------------
void scavHuntSpawnRandomNearPosition(VECTOR position)
{
  // try 4 times to generate a random point near given position
  int i = 0;
  while (i < 4)
  {
    // generate random position
    VECTOR from = {0,0,0,0}, to = {0,0,-6,0}, p = {0,0,1,0};
    float theta = randRadian();
    float radius = randRange(5, 10);
    vector_fromyaw(from, theta);
    vector_scale(from, from, radius);
    from[2] = 3;
    vector_add(from, from,  position);
    vector_add(to, to, from);

	//vector_add(p, p, CollLine_Fix_GetHitPosition());
	scavHuntSpawn(from);
  break;

    // snap to ground
    // and check if ground is walkable
    // if (CollLine_Fix(from, to, 0, NULL, NULL)) {
    //   int colId = CollLine_Fix_GetHitCollisionId() & 0x0f;
    //   if (colId == 0xF || colId == 0x7 || colId == 0x9 || colId == 0xA) {
    //     vector_add(p, p, CollLine_Fix_GetHitPosition());
    //     scavHuntSpawn(p);
    //     break;
    //   }
    // }

    ++i;
  }
}

//--------------------------------------------------------------------------
void scavHuntSpawnRandomNearPlayer(int pIdx)
{
	Player** players = playerGetAll();
	Player* player = players[pIdx];
	if (!player) return;

	scavHuntSpawnRandomNearPosition(player->PlayerPosition);
	// scavHuntSpawn(player->PlayerPosition);
}

//--------------------------------------------------------------------------
void scavHuntOnKillDeathMessage(void* killDeathMsg)
{
  ((void (*)(void*))GetAddress(&vaOnPlayerKill_Func))(killDeathMsg);

  if (scavHuntBoltSpawnCooldown || !scavHuntRoll()) return;

  Player* localPlayer = playerGetFromSlot(0);
  if (!localPlayer) return;
  char killer = *(char*)(killDeathMsg + 0);
  char killed = *(char*)(killDeathMsg + 2);
  if (killed >= 0 && killed < GAME_MAX_PLAYERS && killer == localPlayer->mpIndex) {
    scavHuntSpawnRandomNearPlayer(killed);
  }
}

int scavCheckClients(void)
{
    GameSettings* gs = gameGetSettings();
    int i;
    int uniqueClients = 0;
    char clientIds[GAME_MAX_PLAYERS] = {0,0,0,0,0,0,0,0};

    for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
      int clientId = gs->PlayerClients[i];
      if (clientId < 0) continue;

      if (clientIds[clientId]++ == 0) {
        uniqueClients += 1;
      }
    }

    return uniqueClients;
}

//--------------------------------------------------------------------------
void scavHuntRun(void)
{
  GameSettings* gs = gameGetSettings();
  GameData* gameData = gameGetData();
  netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_RESPONSE_SCAVENGER_HUNT_SETTINGS, &scavHuntOnReceiveRemoteSettings);

  // if the hunt is live then show a first time popup on the online lobby
  if (isInMenus() && uiGetActivePointer(UIP_ONLINE_LOBBY) && !scavHuntShownPopup && !config.disableScavengerHunt && scavHuntEnabled) {
    //uiShowOkDialog("Scavenger Hunt", "The Horizon Scavenger Hunt is live! Hunt for Horizon Bolts for a chance to win prizes! Join our discord for more info: discord.gg/horizonps");
    scavHuntShownPopup = 1;
  }

  // request settings on first download of the patch
  if (!scavHuntHasGotSettings) {
    scavHuntHasGotSettings = 1;
    scavHuntQueryForRemoteSettings();
  }

  // only continue if enabled and in game
  if (config.disableScavengerHunt || !isInGame() || !gs || !gameData) {
    scavHuntInitialized = 0;
    return;
  }

  // disabled
  if (!scavHuntEnabled) { scavHuntShownPopup = 0; return; }
  if (scavHuntSpawnFactor <= 0) { scavHuntShownPopup = 0; return; }

  // reset cooldown on beginning of game
  if (!scavHuntInitialized) {
    scavHuntResetBoltSpawnCooldown();
    scavHuntInitialized = 1;
  }

  Player* localPlayer = playerGetFromSlot(0);
  if (!localPlayer) return;

#if DEBUG
  if (padGetButtonDown(0, PAD_DOWN | PAD_L1) > 0) {
    scavHuntSpawnRandomNearPlayer(0);
  }
#endif

  // we need at least 3 unique clients
#if !DEBUG
  if (scavCheckClients() < 3) return;
#endif

  // hooks
  HOOK_JAL(GetAddress(&vaOnPlayerKill_Hook), &scavHuntOnKillDeathMessage);

  // use stats to detect when objective event happens
  static int lastFlagsCaptured = 0;
  static int lastNodesCaptured = 0;
  static int lastNodesSaved = 0;
  int flagsCaptured = gameData->PlayerStats.FlagsCaptured[localPlayer->mpIndex];
  int nodesCaptured = gameData->PlayerStats.NodesCaptured[localPlayer->mpIndex];
  int nodesSaved = gameData->PlayerStats.NodesSaved[localPlayer->mpIndex];

  // decrement cooldown or check for event
  if (scavHuntBoltSpawnCooldown) {
    --scavHuntBoltSpawnCooldown;
  } else if (flagsCaptured > lastFlagsCaptured && scavHuntRoll()) {
    scavHuntSpawnRandomNearPlayer(localPlayer->mpIndex);
  } else if (nodesCaptured > lastNodesCaptured && scavHuntRoll()) {
    scavHuntSpawnRandomNearPlayer(localPlayer->mpIndex);
  } else if (nodesSaved > lastNodesSaved && scavHuntRoll()) {
    scavHuntSpawnRandomNearPlayer(localPlayer->mpIndex);
  }

  lastFlagsCaptured = flagsCaptured;
  lastNodesCaptured = nodesCaptured;
  lastNodesSaved = nodesSaved;
}
