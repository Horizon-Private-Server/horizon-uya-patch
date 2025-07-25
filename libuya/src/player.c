#include "string.h"
#include "player.h"
#include "interop.h"
#include "game.h"
#include "spawnpoint.h"
#include "team.h"

#define PLAYER_STRUCT_ARRAY                         ((Player**)GetAddress(&vaPlayerStructArray))

#if UYA_PAL
#define TNW_PLAYER ((tNW_Player*)0x0022f3d0)
#define PLAYER_WALK_SPEED_TABLE ((PlayerWalkSpeedTable)0x00245ff8)
#else
#define TNW_PLAYER ((tNW_Player*)0x0022f550)
#define PLAYER_WALK_SPEED_TABLE ((PlayerWalkSpeedTable)0x00246178)
#endif

void playerRespawn(Player * player);
void playerStripWeapons(Player * player);
void internal_HurtPlayer(Player * player, float health);

VariableAddress_t vaPlayerStructArray = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00249330,
    .Hoven = 0x00249530,
    .OutpostX12 = 0x00249420,
    .KorgonOutpost = 0x002492a0,
    .Metropolis = 0x00249320,
    .BlackwaterCity = 0x002492a0,
    .CommandCenter = 0x00248ea0,
    .BlackwaterDocks = 0x00248fa0,
    .AquatosSewers = 0x00248fa0,
    .MarcadiaPalace = 0x00248fa0,
#else
    .Lobby = 0,
    .Bakisi = 0x002494B0,
    .Hoven = 0x002496B0,
    .OutpostX12 = 0x002495a0,
    .KorgonOutpost = 0x00249420,
    .Metropolis = 0x002494A0,
    .BlackwaterCity = 0x00249420,
    .CommandCenter = 0x00249020,
    .BlackwaterDocks = 0x00249120,
    .AquatosSewers = 0x00249120,
    .MarcadiaPalace = 0x00249120,
#endif
};

// playerGetFromslot();
VariableAddress_t vaGetMissionDef = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004ede30,
    .Hoven = 0x004eff48,
    .OutpostX12 = 0x004e5820,
    .KorgonOutpost = 0x004e2fb8,
    .Metropolis = 0x004e2308,
    .BlackwaterCity = 0x004dfba0,
    .CommandCenter = 0x004dfb68,
    .BlackwaterDocks = 0x004e23e8,
    .AquatosSewers = 0x004e16e8,
    .MarcadiaPalace = 0x004e1068,
#else
    .Lobby = 0,
    .Bakisi = 0x004eb710,
    .Hoven = 0x004ed768,
    .OutpostX12 = 0x004e3080,
    .KorgonOutpost = 0x004e0898,
    .Metropolis = 0x004dfbe8,
    .BlackwaterCity = 0x004dd400,
    .CommandCenter = 0x004dd588,
    .BlackwaterDocks = 0x004dfdc8,
    .AquatosSewers = 0x004df108,
    .MarcadiaPalace = 0x004dea48,
#endif
};

VariableAddress_t vaPlayerRespawnFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004fa320,
    .Hoven = 0x004fc438,
    .OutpostX12 = 0x004f1d10,
    .KorgonOutpost = 0x004ef4a8,
    .Metropolis = 0x004ee7f8,
    .BlackwaterCity = 0x004ec090,
    .CommandCenter = 0x004ec058,
    .BlackwaterDocks = 0x004ee8d8,
    .AquatosSewers = 0x004edbd8,
    .MarcadiaPalace = 0x004ed558,
#else
    .Lobby = 0,
    .Bakisi = 0x004f7ba0,
    .Hoven = 0x004f9bf8,
    .OutpostX12 = 0x004ef510,
    .KorgonOutpost = 0x004ecd28,
    .Metropolis = 0x004ec078,
    .BlackwaterCity = 0x004e9890,
    .CommandCenter = 0x004e9a18,
    .BlackwaterDocks = 0x004ec258,
    .AquatosSewers = 0x004eb598,
    .MarcadiaPalace = 0x004eaed8,
#endif
};

VariableAddress_t vaWeaponStripMeFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004fa658,
    .Hoven = 0x004fc770,
    .OutpostX12 = 0x004f2048,
    .KorgonOutpost = 0x004ef7e0,
    .Metropolis = 0x004eeb30,
    .BlackwaterCity = 0x004ec3c8,
    .CommandCenter = 0x004ec390,
    .BlackwaterDocks = 0x004eec10,
    .AquatosSewers = 0x004edf10,
    .MarcadiaPalace = 0x004ed890,
#else
    .Lobby = 0,
    .Bakisi = 0x004f7ed8,
    .Hoven = 0x004f9f30,
    .OutpostX12 = 0x004ef848,
    .KorgonOutpost = 0x004ed060,
    .Metropolis = 0x004ec3b0,
    .BlackwaterCity = 0x004e9bc8,
    .CommandCenter = 0x004e9d50,
    .BlackwaterDocks = 0x004ec590,
    .AquatosSewers = 0x004eb8d0,
    .MarcadiaPalace = 0x004eb210,
#endif
};

VariableAddress_t vaGiveShieldFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0042ab78,
    .Hoven = 0x0042c5d0,
    .OutpostX12 = 0x00423500,
    .KorgonOutpost = 0x004210a8,
    .Metropolis = 0x00420400,
    .BlackwaterCity = 0x0041c268,
    .CommandCenter = 0x00421f08,
    .BlackwaterDocks = 0x00424758,
    .AquatosSewers = 0x00423a70,
    .MarcadiaPalace = 0x004233d8,
#else
    .Lobby = 0,
    .Bakisi = 0x0042a1e8,
    .Hoven = 0x0042bb78,
    .OutpostX12 = 0x00422ac0,
    .KorgonOutpost = 0x00420710,
    .Metropolis = 0x0041fa70,
    .BlackwaterCity = 0x0041b890,
    .CommandCenter = 0x00421600,
    .BlackwaterDocks = 0x00423e38,
    .AquatosSewers = 0x00423168,
    .MarcadiaPalace = 0x00422ab8,
#endif
};

// 
extern const PadHistory DefaultPadHistory;

// Player pad history
PadHistory PlayerPadHistory[GAME_MAX_PLAYERS];

//--------------------------------------------------------------------------------
Player ** playerGetAll(void)
{
    return PLAYER_STRUCT_ARRAY;
}

//--------------------------------------------------------------------------------
void playerSetTeam(Player * player, int teamId)
{
    if (!player)
        return;

    player->mpTeam = teamId;
    // player->pMoby->primaryColor = TEAM_COLORS[teamId];
    player->pMoby->color = player->pMoby->color & 0x8f | 0x80 | (teamId << 4); // 0x80 + (0x10 * teamId);
}

//--------------------------------------------------------------------------------
PAD * playerGetPad(Player * player)
{
    if (!player)
        return 0;

    if (player->isLocal) {
        return player->pPad;
    } else {
        struct tNW_Player* netPlayer = player->pNetPlayer;
        if (!netPlayer)
            return 0;

        return (PadButtonStatus*)(&netPlayer->padMessageElems[0].msg.pad_data);
    }
}

//--------------------------------------------------------------------------------
void playerPadUpdate(void)
{
    int i;
    PAD * playerPad;
    struct PadHistory * padHistory;
    Player ** players = playerGetAll();
    Player * player;

    // Update player pad in game
    if (isInGame())
    {
        for (i = 0; i < GAME_MAX_PLAYERS; ++i)
        {
            player = players[i];
            padHistory = &PlayerPadHistory[i];
            playerPad = playerGetPad(player);

            // Copy last player pad
            if (playerPad)
            {
                memcpy(padHistory, &playerPad->buffer.btns, sizeof(struct PadHistory));
                padHistory->id = player->mpIndex;
            }
            // Reset pad if no player
            else if (padHistory->id >= 0)
            {
                memcpy(padHistory, &DefaultPadHistory, sizeof(struct PadHistory));
            }
        }
    }

    // Reset player pad history when not in game
    else if (PlayerPadHistory[0].id >= 0)
    {
        for (i = 0; i < GAME_MAX_PLAYERS; ++i)
            memcpy(PlayerPadHistory + i, &DefaultPadHistory, sizeof(struct PadHistory));
    }
}

//--------------------------------------------------------------------------------
int playerPadGetButton(Player * player, u16 buttonMask)
{
    if (!player)
        return 0;

   PAD * paddata = playerGetPad(player);
    if (!paddata)
        return 0;

    return (paddata->buffer.btns & buttonMask) == 0;
}

//--------------------------------------------------------------------------------
int playerPadGetButtonDown(Player * player, u16 buttonMask)
{
    if (!player)
        return 0;

    return playerPadGetButton(player, buttonMask) &&
            (PlayerPadHistory[player->mpIndex].btns & buttonMask) != 0;
}

//--------------------------------------------------------------------------------
int playerPadGetButtonUp(Player * player, u16 buttonMask)
{
    if (!player)
        return 0;

    return !playerPadGetButton(player, buttonMask) &&
        (PlayerPadHistory[player->mpIndex].btns & buttonMask) != 0;
}

//--------------------------------------------------------------------------------
// Other Obfuscate Address (Health, State, some Timers)
// Another Area (Pointer): 0x002429A0 and 0x002429A4
VariableAddress_t vaPlayerObfuscateAddr = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x003bfe21,
    .Hoven = 0x003bf461,
    .OutpostX12 = 0x003b7361,
    .KorgonOutpost = 0x003b7121,
    .Metropolis = 0x003b6ea1,
    .BlackwaterCity = 0x003b70e1,
    .CommandCenter = 0x003c7661,
    .BlackwaterDocks = 0x003c7a61,
    .AquatosSewers = 0x003c81e1,
    .MarcadiaPalace = 0x003c7761,
#else
    .Lobby = 0,
    .Bakisi = 0x003bff61,
    .Hoven = 0x003bf5a1,
    .OutpostX12 = 0x003b74a1,
    .KorgonOutpost = 0x003b7261,
    .Metropolis = 0x003b6fe1,
    .BlackwaterCity = 0x003b7221,
    .CommandCenter = 0x003c77a1,
    .BlackwaterDocks = 0x003c7ba1,
    .AquatosSewers = 0x003c8321,
    .MarcadiaPalace = 0x003c78a1,
#endif
};
// Weapon Obfuscate Address
VariableAddress_t vaPlayerObfuscateWeaponAddr = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x003aa041,
    .Hoven = 0x003a9681,
    .OutpostX12 = 0x003a1581,
    .KorgonOutpost = 0x003a1341,
    .Metropolis = 0x003a10c1,
    .BlackwaterCity = 0x003a1301,
    .CommandCenter = 0x003b1881,
    .BlackwaterDocks = 0x003b1c81,
    .AquatosSewers = 0x003b2401,
    .MarcadiaPalace = 0x003b1981,
#else
    .Lobby = 0,
    .Bakisi = 0x003aa181,
    .Hoven = 0x003a97c1,
    .OutpostX12 = 0x003a16c1,
    .KorgonOutpost = 0x003a1481,
    .Metropolis = 0x003a1201,
    .BlackwaterCity = 0x003a1441,
    .CommandCenter = 0x003b19c1,
    .BlackwaterDocks = 0x003b1dc1,
    .AquatosSewers = 0x003b2541,
    .MarcadiaPalace = 0x003b1ac1,
#endif
};
int playerDeobfuscate(int src, DeobfuscateMode_e mode)
{
	char *i = src;
	// i = address, *i = data
	if (!*i && mode == 0)
        return 0;

	Obfuscate_t stack;
	switch (mode) {
		case 0: { // state
			stack.max = 0x28;
			stack.step = 5;
			stack.multiplyVal = 0xff;
			stack.randData = GetAddress(&vaPlayerObfuscateAddr);
			break;
		}
		case 1: { // gadgets
			stack.max = 0x18;
			stack.step = 3;
			stack.multiplyVal = 0xd1;
    		stack.randData = GetAddress(&vaPlayerObfuscateWeaponAddr);
			break;
		}
		case 2: { // timers
			stack.max = 0x18;
			stack.step = 3;
			stack.multiplyVal = 0xff;
    		stack.randData = GetAddress(&vaPlayerObfuscateAddr);
			break;
		}
	}
	u8 *data = &stack.data;
	int n = 0;
	for (n; n < stack.max; n += stack.step) {
		u32 offset = (u32)((int)i - (u32)*i & 7) + n;
		*data = stack.randData[(*i + (offset & 7) * stack.multiplyVal)];
        ++data;
	}
	stack.addr = (u32)((u32)stack.val ^ ((u32)(stack.addr) ^ (u32)i));
	stack.val = (u32)stack.addr >> 0x10;
	if (mode == 0) {
		return stack.addr;
	} else if (mode == 1) {
        return stack.val & 0xff;
	} else if (mode == 2) {
		return stack.val;
	}
	// all other modes, return -1
	return -1;
}

void playerObfuscate(int src, int value, ObfuscateMode_e mode)
{
	union {
		float f;
		u32 i;
		u8 b;
		union {
			u16 s[2];
			struct {
				u16 low;
				u16 high;
			}
		}
	} v = {value};
	Obfuscate_t stack;
	char *i = src; // i: address, *i: value
	char *data = &stack.data;
	switch (mode) {
		case 0: { // states
			stack.step = 5;
			stack.max = 0x28;
			stack.multiplyVal = 0xff;
			stack.randData = GetAddress(&vaPlayerObfuscateAddr);
			break;		}
		case 1: { // health/hitPoints
			value = *(u32*)&v.f;
			stack.step = 5;
			stack.max = 0x28;
			stack.multiplyVal = 0xff;
			stack.randData = GetAddress(&vaPlayerObfuscateAddr);
			break;
		};
		case 2: { // gadgets
			value = (value & 0xffff | value << 0x10);
			stack.max = 0x18;
			stack.step = 3;
			stack.multiplyVal = 0xd1;
    		stack.randData = GetAddress(&vaPlayerObfuscateWeaponAddr);
			break;
		}
		case 3: { // timers
			value = (value << 0x10);
			stack.step = 3;
			stack.max = 0x18;
			stack.multiplyVal = 0xff;
			stack.randData = GetAddress(&vaPlayerObfuscateAddr);
			break;
		};
	}
	u32 algo = *(u32*)(((u32)stack.randData - 0x1) + (((int)i * gameGetGSFrame()) % 0x1fe) * 4);
	stack.addr = algo ^ (u32)i;
	stack.val = algo ^ (u32)value;
	int n = 0;
	for (n; n < stack.max; n += stack.step) {
		u32 offset = (u32)(((int)i - (u32)*i & 7) + n);
		stack.randData[(*i + (offset & 7) * stack.multiplyVal)] = *data;
		++data;
	}
}

VariableAddress_t vaSetPlayerStateFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00520b40,
    .Hoven = 0x00522c58,
    .OutpostX12 = 0x00518530,
    .KorgonOutpost = 0x00515cc8,
    .Metropolis = 0x00515018,
    .BlackwaterCity = 0x005128b0,
    .CommandCenter = 0x00512670,
    .BlackwaterDocks = 0x00514ef0,
    .AquatosSewers = 0x005141f0,
    .MarcadiaPalace = 0x00513b70,
#else
    .Lobby = 0,
    .Bakisi = 0x0051e2f0,
    .Hoven = 0x00520348,
    .OutpostX12 = 0x00515c60,
    .KorgonOutpost = 0x00513478,
    .Metropolis = 0x005127c8,
    .BlackwaterCity = 0x0050ffe0,
    .CommandCenter = 0x0050ff60,
    .BlackwaterDocks = 0x005127a0,
    .AquatosSewers = 0x00511ae0,
    .MarcadiaPalace = 0x00511420,
#endif
};
void playerSetPlayerState(Player * player, u8 state)
{
    // aka: InitBodyState
    internal_SetPlayerState(player, state, 1, 0, 1);
}

//--------------------------------------------------------------------------------
VariableAddress_t vaHurtPlayerFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0050d4d8,
    .Hoven = 0x0050f5f0,
    .OutpostX12 = 0x00504ec8,
    .KorgonOutpost = 0x00502660,
    .Metropolis = 0x005019b0,
    .BlackwaterCity = 0x004ff248,
    .CommandCenter = 0x004ff210,
    .BlackwaterDocks = 0x00501a90,
    .AquatosSewers = 0x00500d90,
    .MarcadiaPalace = 0x00500710,
#else
    .Lobby = 0,
    .Bakisi = 0x0050ace8,
    .Hoven = 0x0050cd40,
    .OutpostX12 = 0x00502658,
    .KorgonOutpost = 0x004ffe70,
    .Metropolis = 0x004ff1c0,
    .BlackwaterCity = 0x004fc9d8,
    .CommandCenter = 0x004fcb60,
    .BlackwaterDocks = 0x004ff3a0,
    .AquatosSewers = 0x004fe6e0,
    .MarcadiaPalace = 0x004fe020,
#endif
};
void playerDecHealth(Player * player, int health)
{
    internal_HurtPlayer(player, health);
}

//--------------------------------------------------------------------------------
void playerIncHealth(Player * player, int health)
{
    /*
        NOTES:
        playerIncHealth doesn't work correctly.  Use playerSetHealth for now.
        Maybe will update in the future.
        Old Version only works on Emulator!
        New version doesnt' work at all.
    */


    // asm(".set noreorder;");
    // int gsFrame = gameGetGSFrame();
    // int RandDataAddr = GetAddress(&vaPlayerObfuscateAddr);
    // int fHealth = playerGetHealth(player);
    // int PlayerIncHealthStack[1];
    // u32 Player_Addr = &player->Health;
    // u32 Player_Value = *(u8*)Player_Addr;
    // PlayerIncHealthStack[0] = (u32)Player_Addr ^ *(u32*)(((u32)RandDataAddr - 0x1) + (((int)Player_Addr * gsFrame) % 0x1fe) * 4);
    // PlayerIncHealthStack[1] = (float)((u32)(*(float*)&fHealth + (float)(int)health) ^ *(u32*)(((u32)RandDataAddr - 0x1) + (((int)Player_Addr * gsFrame) % 0x1fe) * 4));
    // int n = 0;
    // int m = 0;
    // do {
    //     u32 Offset = (u32)(((int)Player_Addr - (u32)Player_Value & 7) + n);
    //     n = n + 5;
    //     *(u8*)((u32)RandDataAddr + (Player_Value + (Offset & 7) * 0xff)) = *(u8*)((int)PlayerIncHealthStack + m);
    //     ++m;
    // } while (n < 0x28);

    // ===== OLD, BUT KEEPING
    // Grab address where math is done
    int math = ((u32)GetAddress(&vaHurtPlayerFunc) + 0xb0);
    // Change to addition
    *(u8*)math = 0;
    // Run normal function
    internal_HurtPlayer(player, health);
    // Revert back to subtraction
    *(u8*)math = 1;
}

void playerSetHealth(Player * player, int health)
{
    if (!player)
        return;

    playerObfuscate(&player->hitPoints, health, OBFUSCATE_MODE_HEALTH);
}

int playerGetHealth(Player * player)
{
    u32 fHealth = playerDeobfuscate(&player->hitPoints, 0);
    return *(float*)&fHealth;
}
//--------------------------------------------------------------------------------
int playerGetRespawnTimer(Player * player)
{
    return playerDeobfuscate(&player->timers.resurrectWait, 2);
}
//--------------------------------------------------------------------------------
int playerGetState(Player *player)
{
    return playerDeobfuscate(&player->state, DEOBFUSCATE_MODE_STATE);
}
//--------------------------------------------------------------------------------
int playerIsDead(Player * player)
{
    // return player->pNetPlayer->pNetPlayerData->hitPoints <= 0;
    // int Health = (int)playerGetHealth(player) <= 0;
    int State = playerDeobfuscate(&player->state, 0);
    int CurrentState = State == PLAYER_STATE_DEATH
        || State == PLAYER_STATE_DROWN
        || State == PLAYER_STATE_DEATH_FALL
        || State == PLAYER_STATE_DEATHSAND_SINK
        || State == PLAYER_STATE_LAVA_DEATH
        || State == PLAYER_STATE_DEATH_NO_FALL
        || State == PLAYER_STATE_WAIT_FOR_RESURRECT
        || State == PLAYER_STATE_ICEWATER_FREEZE
        ;
    return CurrentState;
}

//--------------------------------------------------------------------------------
VariableAddress_t vaPlayerSetPosRotFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00509dc0,
    .Hoven = 0x0050bed8,
    .OutpostX12 = 0x005017b0,
    .KorgonOutpost = 0x004fef48,
    .Metropolis = 0x004fe298,
    .BlackwaterCity = 0x004fbb30,
    .CommandCenter = 0x004fbaf8,
    .BlackwaterDocks = 0x004fe378,
    .AquatosSewers = 0x004fd678,
    .MarcadiaPalace = 0x004fcff8,
#else
    .Lobby = 0,
    .Bakisi = 0x005075d0,
    .Hoven = 0x00509628,
    .OutpostX12 = 0x004fef40,
    .KorgonOutpost = 0x004fc758,
    .Metropolis = 0x004fbaa8,
    .BlackwaterCity = 0x004f92c0,
    .CommandCenter = 0x004f9448,
    .BlackwaterDocks = 0x004f92c0,
    .AquatosSewers = 0x004fafc8,
    .MarcadiaPalace = 0x004fa908,
#endif
};
void playerSetPosRot(Player *player, VECTOR position, VECTOR rotation)
{
    /*
        a4: warpToState
        t0: Unk
        t1: resurrecting
        t2: dropFlag
    */
    if (!position || !rotation) {
        Cuboid pos;
        spawnPointGetRandom(player, &pos.pos, &pos.rot);
        internal_playerWarp(player, &pos.pos, &pos.rot, 0, 1, 1, 1);
        return;
    }

    internal_playerWarp(player, &position, &rotation, 0, 1, 1, 1);
}

//--------------------------------------------------------------------------------
PlayerVTable * playerGetVTable(Player * player)
{
    if (!player)
        return NULL;

    return (PlayerVTable*)player->guber.Vtable;
}

VariableAddress_t vaGiveWeaponUpgradeFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x005472a0,
    .Hoven = 0x00549468,
    .OutpostX12 = 0x0053ed40,
    .KorgonOutpost = 0x0053c428,
    .Metropolis = 0x0053b828,
    .BlackwaterCity = 0x00539010,
    .CommandCenter = 0x00538868,
    .BlackwaterDocks = 0x0053b0e8,
    .AquatosSewers = 0x0053a3e8,
    .MarcadiaPalace = 0x00539d68,
#else
    .Lobby = 0,
    .Bakisi = 0x00544998,
    .Hoven = 0x00546aa0,
    .OutpostX12 = 0x0053c3b8,
    .KorgonOutpost = 0x00539b20,
    .Metropolis = 0x00538f20,
    .BlackwaterCity = 0x00536688,
    .CommandCenter = 0x005360b8,
    .BlackwaterDocks = 0x005388f8,
    .AquatosSewers = 0x00537c38,
    .MarcadiaPalace = 0x00537578,
#endif
};
void playerGiveWeaponUpgrade(Player * player, int weaponId)
{
    internal_playerGiveWeaponUpgrade((u32)player + 0x1a40, weaponId);
}

VariableAddress_t vaGiveWeaponFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00546c78,
    .Hoven = 0x00548e40,
    .OutpostX12 = 0x0053e718,
    .KorgonOutpost = 0x0053be00,
    .Metropolis = 0x0053b200,
    .BlackwaterCity = 0x005389e8,
    .CommandCenter = 0x00538240,
    .BlackwaterDocks = 0x0053aac0,
    .AquatosSewers = 0x00539dc0,
    .MarcadiaPalace = 0x00539740,
#else
    .Lobby = 0,
    .Bakisi = 0x00544370,
    .Hoven = 0x00546478,
    .OutpostX12 = 0x0053bd90,
    .KorgonOutpost = 0x005394f8,
    .Metropolis = 0x005388f8,
    .BlackwaterCity = 0x00536060,
    .CommandCenter = 0x00535a90,
    .BlackwaterDocks = 0x005382d0,
    .AquatosSewers = 0x00537610,
    .MarcadiaPalace = 0x00536f50,
#endif
};
void playerGiveWeapon(Player * player, int weaponId, int type)
{
    internal_playerGiveWeapon((u32)player + 0x1a40, weaponId, type);
}

VariableAddress_t vaEquipWeaponFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x005471b8,
    .Hoven = 0x00549380,
    .OutpostX12 = 0x0053ec58,
    .KorgonOutpost = 0x0053c340,
    .Metropolis = 0x0053b740,
    .BlackwaterCity = 0x00538f28,
    .CommandCenter = 0x00538780,
    .BlackwaterDocks = 0x0053b000,
    .AquatosSewers = 0x0053a300,
    .MarcadiaPalace = 0x00539c80,
#else
    .Lobby = 0,
    .Bakisi = 0x005448b0,
    .Hoven = 0x005469b8,
    .OutpostX12 = 0x0053c2d0,
    .KorgonOutpost = 0x00539a38,
    .Metropolis = 0x00538e38,
    .BlackwaterCity = 0x005365a0,
    .CommandCenter = 0x00535fd0,
    .BlackwaterDocks = 0x00538810,
    .AquatosSewers = 0x00537b50,
    .MarcadiaPalace = 0x00537490,
#endif
};
void playerEquipWeapon(Player * player, int weaponId)
{
    internal_playerEquipWeapon((u32)player + 0x1a40, weaponId);
}

VariableAddress_t vaGiveMeRandomWeaponsFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00519bd0,
    .Hoven = 0x0051bce8,
    .OutpostX12 = 0x005115c0,
    .KorgonOutpost = 0x0050ed58,
    .Metropolis = 0x0050e0a8,
    .BlackwaterCity = 0x0050b940,
    .CommandCenter = 0x0050b700,
    .BlackwaterDocks = 0x0050df80,
    .AquatosSewers = 0x0050d280,
    .MarcadiaPalace = 0x0050cc00,
#else
    .Lobby = 0,
    .Bakisi = 0x005173c8,
    .Hoven = 0x00519420,
    .OutpostX12 = 0x0050ed38,
    .KorgonOutpost = 0x0050c550,
    .Metropolis = 0x0050b8a0,
    .BlackwaterCity = 0x005090b8,
    .CommandCenter = 0x00509038,
    .BlackwaterDocks = 0x0050b878,
    .AquatosSewers = 0x0050abb8,
    .MarcadiaPalace = 0x0050a4f8,
#endif
};
void playerGiveRandomWeapons(Player * player, int amount)
{
    internal_GiveMeRandomWeapons(player, amount);
}

int playerGetGadgetId(Player *player, int slot)
{
    return playerDeobfuscate(&player->quickSelect.Slot[slot], DEOBFUSCATE_MODE_GADGET);
}

int playerGetGadgetLevel(Player *player, int slot)
{
    return playerDeobfuscate(&player->weaponMeter.Slot[slot], DEOBFUSCATE_MODE_GADGET);
}

int playerGetGadgetAmmo(Player *player, int slot)
{
    return playerDeobfuscate(&player->weaponAmmo.Slot[slot], DEOBFUSCATE_MODE_GADGET);
}

int playerHasShield(Player * player)
{
    Moby *shield = mobyListGetStart();
    while ((shield = mobyFindNextByOClass(shield, MOBY_ID_OMNI_SHIELD))) {
        if (shield->pVar) {
            int PlayerShield = *(u32*)((u32)shield->pVar + 0x40);
            if (PlayerShield == player)
                return 1;
        }
        ++shield;
    }
    return 0;
}
