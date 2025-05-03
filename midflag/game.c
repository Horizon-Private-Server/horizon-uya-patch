#include <tamtypes.h>
#include <string.h>
#include <libuya/moby.h>
#include <libuya/player.h>
#include <libuya/guber.h>
#include <libuya/net.h>
#include <libuya/time.h>
#include <libuya/interop.h>
#include <libuya/stdio.h>
#include <libuya/string.h>
#include <libuya/graphics.h>
#include <libuya/color.h>
#include <libuya/random.h>
#include <libuya/math.h>
#include <libuya/game.h>
#include <libuya/map.h>
#include <libuya/team.h>
#include <libuya/spawnpoint.h>
#include <libuya/ui.h>
#include <libuya/utils.h>

#include "config.h"
#include "include/game.h"

VariableAddress_t vaFlagEventUiPopup_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00430568,
	.Hoven = 0x00431fc0,
	.OutpostX12 = 0x00428ef0,
	.KorgonOutpost = 0x00426a98,
	.Metropolis = 0x00425df0,
	.BlackwaterCity = 0x00421c58,
	.CommandCenter = 0x004263e8,
	.BlackwaterDocks = 0x00428c38,
	.AquatosSewers = 0x00427f50,
	.MarcadiaPalace = 0x004278b8,
#else
	.Lobby = 0,
	.Bakisi = 0x0042fae0,
	.Hoven = 0x00431470,
	.OutpostX12 = 0x004283b8,
	.KorgonOutpost = 0x00426008,
	.Metropolis = 0x00425368,
	.BlackwaterCity = 0x00421188,
	.CommandCenter = 0x00425a38,
	.BlackwaterDocks = 0x00428270,
	.AquatosSewers = 0x004275a0,
	.MarcadiaPalace = 0x00426ef0,
#endif
};

typedef struct flagPositions {
	int mapId;
	float x;
	float z;
	float y;
} flagPositions_t;

flagPositions_t midFlagPos[] = {
	{MAP_ID_BAKISI, 402.324, 366.073, 200.466},
	{MAP_ID_HOVEN, 259.964, 253.611, 62.598},
	{MAP_ID_OUTPOST_X12, 427.247, 204.962, 108.406},
	{MAP_ID_METROPOLIS, 755.314, 339.311, 337.703},
	{MAP_ID_BLACKWATER_CITY, 219.817, 266.732, 87.722},
	{MAP_ID_BLACKWATER_DOCKS, 211.819, 191.808, 99.281},
	{MAP_ID_AQUATOS_SEWERS, 401.181, 336.686, 940.75}
};

typedef struct midFlagInfo {
	int setup;
	Moby *pRedFlag;
	Moby *pBlueFlag;
	int flagCuboid;
	short baseCuboid[2];
} midFlagInfo_t;
midFlagInfo_t midFlag = {
	.setup = 0,
	.pRedFlag = 0,
	.pBlueFlag = 0,
	.flagCuboid = -1,
	.baseCuboid = 0,
};

extern isConfigMenuActive;
extern isCustomMap;

int findClosestSpawnPointToPosition(VECTOR position, float deadzone)
{
	// find closest spawn point
	int bestIdx = -1;
	int i;
	float bestDist = 1000000;
	float sqrDeadzone = deadzone * deadzone;
	int spCount = spawnPointGetCount();
	for (i = 0; i < spCount; ++i) {
		if (!spawnPointIsPlayer(i)) continue;

		Cuboid* sp = spawnPointGet(i);
		VECTOR delta;
		vector_subtract(delta, position, &sp->pos);
		float sqrDist = vector_sqrmag(delta);
		if (sqrDist < bestDist && sqrDist >= sqrDeadzone) {
			bestDist = sqrDist;
			bestIdx = i;
		}
	}

	return bestIdx;
}

void patchFlagUiPopup_Logic(short stringId, int seconds, int player)
{
	Moby *flag = midFlag.pRedFlag;
	flagPVars_t *flagVars = flag->pVar;
	Player *p = playerGetFromSlot(flagVars->carrierIdx);
	char buff[64];
	char *team[2] = {"Blue", "Red"};
	GameSettings *gs = gameGetSettings();
	int pIdx = p->mpIndex;
	int pTeam = p->mpTeam;
	switch (stringId) {
		case 0x1411:
		case 0x1412: {
			snprintf(buff, 64, "%s has picked up the %s Team's Flag!", gs->PlayerNames[pIdx], (char*)team[!pTeam]);
			break;
		}
		case 0x1413:
		case 0x1414: {
			snprintf(buff, 64, "%s has dropped the %s Team's Flag!", gs->PlayerNames[pIdx], (char*)team[!pTeam]);
			break;		
		}
		default: {
			strncpy(buff, uiMsgString(stringId), 64);
			break;
		}
	}
	// show mesage
	uiShowPopup(player, buff, seconds);
}

void patchFlagUiPopup(void)
{
	flagPVars_t * flag = &midFlag.pRedFlag->pVar;
	u32 *hook = GetAddress(&vaFlagEventUiPopup_Hook);

	// stop original function nulling  flag carrierId
	// *(u32*)(hook - 0x44) = 0;
	// hook our function.
	// HOOK_JAL(hook, &patchFlagUiPopup_Logic);

	// disable ui popup for flag.
	POKE_U32(hook, 0);
}

void midFlagGetFlags(void)
{
	Moby *mobyStart = mobyListGetStart();
	Moby *mobyEnd = mobyListGetEnd();
	while (mobyStart < mobyEnd) {
		switch (mobyStart->oClass) {
			case MOBY_ID_CTF_RED_FLAG:
				midFlag.pRedFlag = mobyStart;
			case MOBY_ID_CTF_BLUE_FLAG:
				midFlag.pBlueFlag = mobyStart;
		}
		++mobyStart;
	}
	midFlag.setup = 1;
}

void midflagConfigBasesAndSpawn(Moby *redFlag, Moby *blueFlag)
{
	flagPVars_t *red = (Moby *)redFlag->pVar;
	flagPVars_t *blue = (Moby *)blueFlag->pVar;
	VECTOR spMedianPosition = {0, 0, 0, 0};
	int centerSpawn = 0;
	// save capture cuboids
	midFlag.baseCuboid[0] = red->captureCuboid;
	midFlag.baseCuboid[1] = blue->captureCuboid;
	// swap unk_28
	red->unk_28 = blue->unk_28;
	// make red flag glow
	redFlag->primaryColor = 0xffffffff;
	// disable and hide blue flag
	blueFlag->modeBits |= MOBY_MODE_BIT_DISABLED | MOBY_MODE_BIT_HIDDEN | MOBY_MODE_BIT_NO_UPDATE;
	// check if flag spawn override.
	int mapId = gameGetSettings()->GameLevel;
	int forceFlagPos = 0;
	int i = 0;
	if (isCustomMap == 0) {
		for (i; i < COUNT_OF(midFlagPos); ++i) {
			if (mapId == midFlagPos[i].mapId) {
				spMedianPosition[0] = midFlagPos[i].x;
				spMedianPosition[1] = midFlagPos[i].z;
				spMedianPosition[2] = midFlagPos[i].y;
				forceFlagPos = 1;
				break;
			}
		}
	}
	if (!forceFlagPos) {
		vector_add(spMedianPosition, spMedianPosition, red->basePos);
		vector_add(spMedianPosition, spMedianPosition, blue->basePos);
		vector_scale(spMedianPosition, spMedianPosition, 0.5);
		midFlag.flagCuboid = findClosestSpawnPointToPosition(spMedianPosition, 0);
		vector_copy(spMedianPosition, spawnPointGet(midFlag.flagCuboid)->pos);
	}
	// set flag spawn
	vector_copy(red->basePos, spMedianPosition);
	vector_copy(redFlag->position, spMedianPosition);
	redFlag->position[2] += .25;

	// hook flag event ui popup so we can use our own strings.
	patchFlagUiPopup();

	midFlag.setup = 2;
}

void gameTick(int isCustomMap)
{
	// find and store flag moby address
	if (midFlag.setup == 0)
		midFlagGetFlags();

	Moby *redFlag = midFlag.pRedFlag;
	Moby *blueFlag = midFlag.pBlueFlag;
	if ((Moby *)redFlag == 0)
		return;
	
	// get flag pVars
	flagPVars_t *red = (Moby *)redFlag->pVar;
	flagPVars_t *blue = (Moby *)((u32)redFlag->pVar + 0x30);
	// setup bases and find center spawn for flag
	if (midFlag.setup == 1)
		midflagConfigBasesAndSpawn(redFlag, blueFlag);

	// Flag Logic
	// if flag not carried, set team to 3 so all teams can pick it up.
	if (red->carrierIdx == -1 && red->team != 2) {
		red->team = 2;
		red->captureCuboid = -1;
	}
	// if carried, set red flag team to opposite of player team.
	else if (red->carrierIdx > -1) {
		Player *player = playerGetFromSlot(red->carrierIdx);
		// blue team captures: 1, red team captures: 0
		int mpTeam = !player->mpTeam;
		red->team = mpTeam;
		red->captureCuboid = midFlag.baseCuboid[mpTeam];
	}

	// Sprite Logic
	Player *player = playerGetFromSlot(0);
	int isPaused = player->pauseOn;
	// draw sprite for non-flag holders.
	if (!player->flagMoby) {
		int i = red->carrierIdx;
		u32 color = i > -1 ? TEAM_COLORS[!red->team] : 0x00ffffff;
		VECTOR t;
		// if not carried, set sprite to flag position.
		if (i == -1) {
			vector_copy(t, redFlag->position);
			t[2] += 3;
			t[0] += .07;
		}
		// if carried, set sprite to player position.
		else if (i > -1) {
			Player *p = playerGetFromSlot(i);
			vector_copy(t, p->playerPosition);
			t[2] += 2;
			t[0] += .07;	
		}
		if (!isPaused)
			gfxHelperDrawSprite_WS(t, 24, 24, SPRITE_FLAG, 0x80000000 | color, TEXT_ALIGN_MIDDLECENTER);
	}

	#ifdef DEBUG
	player = playerGetFromSlot(0);
	if (player->pauseOn == 0 && playerPadGetButtonDown(player, PAD_CIRCLE | PAD_CROSS) > 0) {
		printf("\n================");
		printf("\nflagCube: %d", midFlag.flagCuboid);
		printf("\nbaseCube (r, b): %d, %d", midFlag.baseCuboid[0], midFlag.baseCuboid[1]);
		printf("\nr+b: %08x, %08x", midFlag.pRedFlag, midFlag.pBlueFlag);
		printf("\ngd: %08x", gameGetData()->CTFGameData);
		printf("\n");
	}
	#endif
}

void initialize(PatchGameConfig_t* gameConfig)
{
	// hook messages
	// netHookMessages();

	memset(&midFlag, 0, sizeof(midFlagInfo_t));

	// give a 1 second delay before finalizing the initialization.
	// this helps prevent the slow loaders from desyncing
	static int startDelay = 60 * 2;
	if (startDelay > 0) {
		--startDelay;
		return;
	}

	// finalize initialization
	State.Initialized = 1;
}
