#include <tamtypes.h>

#include <libuya/stdio.h>
#include <libuya/string.h>
#include <libuya/player.h>
#include <libuya/utils.h>
#include <libuya/game.h>
#include <libuya/pad.h>
#include <libuya/uya.h>
#include <libuya/weapon.h>
#include <libuya/interop.h>
#include <libuya/moby.h>
#include <libuya/graphics.h>
#include <libuya/gamesettings.h>
#include <libuya/spawnpoint.h>
#include <libuya/team.h>
#include <libuya/ui.h>
#include <libuya/time.h>
#include <libuya/camera.h>
#include <libuya/gameplay.h>
#include <libuya/map.h>
#include <libuya/collision.h>
#include <libuya/guber.h>
#include <libuya/sound.h>

#define TestMoby	(*(Moby**)0x00091004)

VECTOR position;
VECTOR rotation;

static int SPRITE_ME = 0;
static int EFFECT_ME = 21;
static int SOUND_ME = 0;
static int SOUND_ME_FLAG = 3;

void DebugInGame(Player* player)
{
    if (playerPadGetButtonDown(player, PAD_LEFT) > 0) {
		// Swap Teams
		int SetTeam = (player->mpTeam < 7) ? player->mpTeam + 1 : 0;
		playerSetTeam(player, SetTeam);
	} else if (playerPadGetButtonDown(player, PAD_RIGHT) > 0) {
        // Hurt Player
        playerDecHealth(player, 1);
	} else if (playerPadGetButtonDown(player, PAD_UP) > 0) {
		// static int Occlusion = (Occlusion == 2) ? 0 : 2;
		// gfxOcclusion(Occlusion);
		// spawnPointGetRandom(player, &position, &rotation);
		Player ** ps = playerGetAll();
		Player * p = ps[1];
		playerSetPosRot(player, &p->PlayerPosition, &p->PlayerRotation);
	} else if(playerPadGetButtonDown(player, PAD_DOWN) > 0) {
		// Set Gattling Turret Health to 1.
		DEBUGsetGattlingTurretHealth();
	} else if (playerPadGetButtonDown(player, PAD_L3) > 0) {
		// Nothing Yet!
	} else if (playerPadGetButtonDown(player, PAD_R3) > 0) {
		// Nothing Yet!
	}
}

void DebugInMenus(void)
{
    if (padGetButtonDown(0, PAD_LEFT) > 0) {
		// Nothing Yet!
	} else if (padGetButtonDown(0, PAD_RIGHT) > 0) {
		// Nothing Yet!
	} else if (padGetButtonDown(0, PAD_UP) > 0) {
		// Nothing Yet!
	} else if(padGetButtonDown(0, PAD_DOWN) > 0) {
		// Nothing Yet!
	} else if (padGetButtonDown(0, PAD_L3) > 0) {
		// Nothing Yet!
	} else if (padGetButtonDown(0, PAD_R3) > 0) {
		// Nothing Yet!
	}
}

void InfiniteChargeboot(void)
{
	int i;
	Player ** players = playerGetAll();
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
		Player * player = players[i];
		if (!player)
			continue;

		if (player->timers.IsChargebooting == 1 && playerPadGetButton(player, PAD_R2) > 0 && player->timers.state > 55)
			player->timers.state = 55;
	}
}

void InfiniteHealthMoonjump(void)
{
    static int _InfiniteHealthMoonjump_Init = 0;
    int Joker = *(u16*)0x00225982;
    if (Joker == 0xFDFB)
        _InfiniteHealthMoonjump_Init = 1;
    else if (Joker == 0xFFFD)
        _InfiniteHealthMoonjump_Init = 0;

    if (_InfiniteHealthMoonjump_Init)
    {
        Player * player = (Player*)PLAYER_STRUCT;
		if (playerGetHealth(player) < 15)
        	playerSetHealth(player, 15);

        if (Joker == 0xBFFF)
            (float)player->move.behavior[2] = 0.125;
    }
}

void DEBUGsetGattlingTurretHealth(void)
{
    Moby* moby = mobyListGetStart();
    // Iterate through mobys and change health
    while ((moby = mobyFindNextByOClass(moby, MOBY_ID_GATTLING_TURRET))) {
        if (moby->PVar) {
			*(float*)((u32)moby->PVar + 0x30) = 0;
        }
        ++moby;
    }
}


void Test_Sprites(float x, float y, float scale)
{
	float small = scale * 0.75;
	float delta = (scale - small) / 2;

	gfxSetupGifPaging(0);
	u64 dreadzoneSprite = gfxGetFrameTex(SPRITE_ME);
	// gfxDrawSprite(x+2, y+2, scale, scale, 0, 0, 32, 32, 0x40000000, dreadzoneSprite);
	gfxDrawSprite(x,   y,   scale, scale, 0, 0, 32, 32, 0x80C0C0C0, dreadzoneSprite);
	gfxDrawSprite(x+delta, y+delta, small, small, 0, 0, 32, 32, 0x80000040, dreadzoneSprite);
	gfxDoGifPaging();
}

void drawEffectQuad(VECTOR position, int texId, float scale)
{
	struct QuadDef quad;
	MATRIX m2;
	VECTOR t;
	VECTOR pTL = {0.5,0,0.5,1};
	VECTOR pTR = {-0.5,0,0.5,1};
	VECTOR pBL = {0.5,0,-0.5,1};
	VECTOR pBR = {-0.5,0,-0.5,1};

	// determine color
	u32 color = 0x80FFFFFF;

	// set draw args
	matrix_unit(m2);

	// init
	// gfxResetQuad(&quad);

	// color of each corner?
	vector_copy(quad.VertexPositions[0], pTL);
	vector_copy(quad.VertexPositions[1], pTR);
	vector_copy(quad.VertexPositions[2], pBL);
	vector_copy(quad.VertexPositions[3], pBR);
	quad.VertexColors[0] = quad.VertexColors[1] = quad.VertexColors[2] = quad.VertexColors[3] = color;
	quad.VertexUVs[0] = (struct UV){0,0};
	quad.VertexUVs[1] = (struct UV){1,0};
	quad.VertexUVs[2] = (struct UV){0,1};
	quad.VertexUVs[3] = (struct UV){1,1};
	quad.Clamp = 0;
	quad.Tex0 = gfxGetEffectTex(texId, 1);
	quad.Tex1 = 0xFF9000000260;
	quad.Alpha = 0x8000000044;

	GameCamera* camera = cameraGetGameCamera(0);
	if (!camera)
		return;

	// get forward vector
	vector_subtract(t, camera->pos, position);
	t[2] = 0;
	vector_normalize(&m2[4], t);

	// up vector
	m2[8 + 2] = 1;

	// right vector
	vector_outerproduct(&m2[0], &m2[4], &m2[8]);

	t[0] = t[1] = t[2] = scale;
	t[3] = 1;
	matrix_scale(m2, m2, t);

	// position
	memcpy(&m2[12], position, sizeof(VECTOR));

	// draw
	gfxDrawQuad(&quad, m2, 1);
}

int ping(void)
{
	static int ping_old = 0;
	static int myPing = 0;
	int ping_new = *(int*)0x001d3b5c;
	if (ping_old != ping_new) {
		myPing = ping_new - ping_old;
		ping_old = ping_new;
	}
	return myPing;
}

VECTOR b6ExplosionPositions[64];
int b6ExplosionPositionCount = 0;
void drawB6Visualizer(void)
{
	int i;

	for (i = 0; i < b6ExplosionPositionCount; ++i) {
		drawEffectQuad(b6ExplosionPositions[i], 4, 1);
	}
}

void renderB6Visualizer(Moby* m)
{
	gfxStickyFX(&drawB6Visualizer, m);
}
void runB6HitVisualizer(void)
{
	VECTOR off = {0,0,2,0};
	if (!TestMoby) {
		Moby *m = TestMoby = mobySpawn(MOBY_ID_TEST, 0);
		m->Scale *= 0.1;
		m->PUpdate = &renderB6Visualizer;
		vector_add(m->Position, playerGetFromSlot(0)->PlayerPosition, off);
	}

  // 
  TestMoby->DrawDist = 0xFF;
  TestMoby->UpdateDist = 0xFF;

	// check for b6
	Moby* b = mobyListGetStart();
	Moby* mEnd = mobyListGetEnd();
	while (b < mEnd) {
		if (!mobyIsDestroyed(b) && b->OClass == MOBY_ID_SHOT_GRAVITY_BOMB1 && b->State == 2) {
			DPRINTF("%08X\n", (u32)b->PUpdate);
			vector_copy(b6ExplosionPositions[b6ExplosionPositionCount], b->Position);
			b6ExplosionPositionCount = (b6ExplosionPositionCount + 1) % 64;
		}
		++b;
	}
}

void drawCollider(Moby* moby)
{
	VECTOR pos,t,o = {0,0,0.7,0};
	int i,j = 0;
	int x,y;
	char buf[12];
	Player * p = playerGetFromSlot(0);

	const int steps = 10 * 2;
	const float radius = 2;

	for (i = 0; i < steps; ++i) {
		for (j = 0; j < steps; ++j) {
			float theta = (i / (float)steps) * MATH_TAU;
			float omega = (j / (float)steps) * MATH_TAU;

			vector_copy(pos, p->PlayerPosition);
			pos[0] += radius * sinf(theta) * cosf(omega);
			pos[1] += radius * sinf(theta) * sinf(omega);
			pos[2] += radius * cosf(theta);

			vector_add(t, p->PlayerPosition, o);

			if (CollLine_Fix(pos, t, 1, NULL, 0)) {
				vector_copy(t, CollLine_Fix_GetHitPosition());
				drawEffectQuad(t, EFFECT_ME, 0.1);
			}
		}
	}
}

void drawSomething(Moby* moby)
{
	VECTOR pos,t,o = {0,0,0.7,0};
	int i,j = 0;
	int x,y;
	char buf[12];
	Player * p = playerGetFromSlot(0);

	const int steps = 10 * 2;
	const float radius = 2;

	vector_copy(pos, p->PlayerPosition);
	for (i = 0; i < steps; ++i) {
		for (j = 0; j < steps; ++j) {
			float theta = (i / (float)steps) * MATH_TAU;
			float omega = (j / (float)steps) * MATH_TAU;

			vector_copy(pos, p->PlayerPosition);
			pos[0] += radius * sinf(theta) * cosf(omega);
			pos[1] += radius * sinf(theta) * sinf(omega);
			pos[2] += radius * cosf(theta);

			vector_add(t, p->PlayerPosition, o);

			if (CollLine_Fix(pos, t, 1, NULL, 0)) {
				vector_copy(t, CollLine_Fix_GetHitPosition());
				// drawEffectQuad(t, EFFECT_ME, 0.1);
			}
		}
	}
	drawEffectQuad(pos, EFFECT_ME, 2);
}

void Test()
{
	GameSettings * gameSettings = gameGetSettings();
	GameOptions * gameOptions = gameGetOptions();
	if (gameOptions || gameSettings || gameSettings->GameLoadStartTime > 0) {
		gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_SmallTurrets = 0;
		gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_Bots = 0;
		gameOptions->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets = 0;
	}

    if (isInGame()) {
		Player * p = playerGetFromSlot(0);
		if (!p)
			return 0;

		// Force Normal Up/Down Controls
		*(u32*)0x001A5A70 = 0;

		// gfxStickyFX(&PostDraw, p->PlayerMoby);
		// drawEffectQuad(p->PlayerMoby->Position, 1, .5);
		// drawSomething(p->PlayerMoby);
		
		// base light follow player
		// ((void (*)(Moby*))0x003F6670)(p->PlayerMoby);

		// Set 1k kills
		// *(u32*)0x004A8F6C = 0x240703E8;
		// *(u32*)0x00539258 = 0x240203E8;
		// *(u32*)0x005392D8 = 0x240203E8;

		// Test_Sprites(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * .50, 100);

		// printf("\nState: %d", playerDeobfuscate(&p->State, 0, 0));
		// printf("\nPrevious State: %d", playerDeobfuscate(&p->PreviousState, 0, 0));
		// printf("\nPrePrevious State: %d", playerDeobfuscate(&p->PrePreviousState, 0, 0));
		// printf("\nState Type: %d", playerDeobfuscate(&p->StateType, 0, 0));
		// printf("\nPrevious Type: %d", playerDeobfuscate(&p->PreviousType, 0, 0));
		// printf("\nPrePrevious Type: %d", playerDeobfuscate(&p->PrePreviousType, 0, 0));
		// printf("\nground: %x", (u32)((u32)&p->ground - (u32)PLAYER_STRUCT));
		// printf("\nground->ground.pMoby: %x", (u32)((u32)&p->ground.pMoby - (u32)PLAYER_STRUCT));
		// printf("\nmtxFxActive: %x", (u32)((u32)&p->mtxFxActive - (u32)PLAYER_STRUCT));
		// printf("\npnetplayer: %x", (u32)((u32)&p->pNetPlayer - (u32)PLAYER_STRUCT));

		// float x = SCREEN_WIDTH * 0.3;
		// float y = SCREEN_HEIGHT * 0.85;
		// gfxScreenSpaceText(x, y, 1, 1, 0x80FFFFFF, "TEST YOUR MOM FOR HUGS", -1, 4);
		
		// printf("Collision: %d\n", CollHotspot());
        
		// drawCollider(p->PlayerMoby);
        runB6HitVisualizer();
		InfiniteChargeboot();
		InfiniteHealthMoonjump();
    	DebugInGame(p);
    } else {
		DebugInMenus();
	}
}
