#include "string.h"
#include "player.h"
#include "interop.h"
#include "game.h"
#include "spawnpoint.h"
#include "team.h"
#include "utils.h"

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

variableAddressCompact_t vaPlayerStructArray = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00249330),
	.Hoven = VA_COMPACT_OFFSET(0x00249330, 0x00249530),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00249330, 0x00249420),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00249330, 0x002492a0),
	.Metropolis = VA_COMPACT_OFFSET(0x00249330, 0x00249320),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00249330, 0x002492a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00249330, 0x00248ea0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00249330, 0x00248fa0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00249330, 0x00248fa0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00249330, 0x00248fa0),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x002494b0),
	.Hoven = VA_COMPACT_OFFSET(0x002494b0, 0x002496b0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x002494b0, 0x002495a0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x002494b0, 0x00249420),
	.Metropolis = VA_COMPACT_OFFSET(0x002494b0, 0x002494a0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x002494b0, 0x00249420),
	.CommandCenter = VA_COMPACT_OFFSET(0x002494b0, 0x00249020),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x002494b0, 0x00249120),
	.AquatosSewers = VA_COMPACT_OFFSET(0x002494b0, 0x00249120),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x002494b0, 0x00249120),
#endif
};

// playerGetFromslot();
variableAddressCompact_t vaGetMissionDef = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004ede30),
	.Hoven = VA_COMPACT_OFFSET(0x004ede30, 0x004eff48),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ede30, 0x004e5820),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ede30, 0x004e2fb8),
	.Metropolis = VA_COMPACT_OFFSET(0x004ede30, 0x004e2308),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ede30, 0x004dfba0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ede30, 0x004dfb68),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ede30, 0x004e23e8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ede30, 0x004e16e8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ede30, 0x004e1068),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004eb710),
	.Hoven = VA_COMPACT_OFFSET(0x004eb710, 0x004ed768),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004eb710, 0x004e3080),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004eb710, 0x004e0898),
	.Metropolis = VA_COMPACT_OFFSET(0x004eb710, 0x004dfbe8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004eb710, 0x004dd400),
	.CommandCenter = VA_COMPACT_OFFSET(0x004eb710, 0x004dd588),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004eb710, 0x004dfdc8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004eb710, 0x004df108),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004eb710, 0x004dea48),
#endif
};

variableAddressCompact_t vaPlayerRespawnFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004fa320),
	.Hoven = VA_COMPACT_OFFSET(0x004fa320, 0x004fc438),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004fa320, 0x004f1d10),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004fa320, 0x004ef4a8),
	.Metropolis = VA_COMPACT_OFFSET(0x004fa320, 0x004ee7f8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004fa320, 0x004ec090),
	.CommandCenter = VA_COMPACT_OFFSET(0x004fa320, 0x004ec058),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004fa320, 0x004ee8d8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004fa320, 0x004edbd8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004fa320, 0x004ed558),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004f7ba0),
	.Hoven = VA_COMPACT_OFFSET(0x004f7ba0, 0x004f9bf8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f7ba0, 0x004ef510),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f7ba0, 0x004ecd28),
	.Metropolis = VA_COMPACT_OFFSET(0x004f7ba0, 0x004ec078),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f7ba0, 0x004e9890),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f7ba0, 0x004e9a18),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f7ba0, 0x004ec258),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f7ba0, 0x004eb598),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f7ba0, 0x004eaed8),
#endif
};

variableAddressCompact_t vaWeaponStripMeFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004fa658),
	.Hoven = VA_COMPACT_OFFSET(0x004fa658, 0x004fc770),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004fa658, 0x004f2048),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004fa658, 0x004ef7e0),
	.Metropolis = VA_COMPACT_OFFSET(0x004fa658, 0x004eeb30),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004fa658, 0x004ec3c8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004fa658, 0x004ec390),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004fa658, 0x004eec10),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004fa658, 0x004edf10),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004fa658, 0x004ed890),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004f7ed8),
	.Hoven = VA_COMPACT_OFFSET(0x004f7ed8, 0x004f9f30),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f7ed8, 0x004ef848),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f7ed8, 0x004ed060),
	.Metropolis = VA_COMPACT_OFFSET(0x004f7ed8, 0x004ec3b0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f7ed8, 0x004e9bc8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f7ed8, 0x004e9d50),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f7ed8, 0x004ec590),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f7ed8, 0x004eb8d0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f7ed8, 0x004eb210),
#endif
};

variableAddressCompact_t vaGiveShieldFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0042ab78),
	.Hoven = VA_COMPACT_OFFSET(0x0042ab78, 0x0042c5d0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0042ab78, 0x00423500),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0042ab78, 0x004210a8),
	.Metropolis = VA_COMPACT_OFFSET(0x0042ab78, 0x00420400),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0042ab78, 0x0041c268),
	.CommandCenter = VA_COMPACT_OFFSET(0x0042ab78, 0x00421f08),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0042ab78, 0x00424758),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0042ab78, 0x00423a70),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0042ab78, 0x004233d8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0042a1e8),
	.Hoven = VA_COMPACT_OFFSET(0x0042a1e8, 0x0042bb78),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0042a1e8, 0x00422ac0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0042a1e8, 0x00420710),
	.Metropolis = VA_COMPACT_OFFSET(0x0042a1e8, 0x0041fa70),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0042a1e8, 0x0041b890),
	.CommandCenter = VA_COMPACT_OFFSET(0x0042a1e8, 0x00421600),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0042a1e8, 0x00423e38),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0042a1e8, 0x00423168),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0042a1e8, 0x00422ab8),
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
                memcpy(padHistory, &playerPad->btns, sizeof(struct PadHistory));
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

    return (paddata->btns & buttonMask) == 0;
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
variableAddressCompact_t vaPlayerObfuscateAddr = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x003bfe21),
	.Hoven = VA_COMPACT_OFFSET(0x003bfe21, 0x003bf461),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003bfe21, 0x003b7361),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003bfe21, 0x003b7121),
	.Metropolis = VA_COMPACT_OFFSET(0x003bfe21, 0x003b6ea1),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003bfe21, 0x003b70e1),
	.CommandCenter = VA_COMPACT_OFFSET(0x003bfe21, 0x003c7661),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003bfe21, 0x003c7a61),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003bfe21, 0x003c81e1),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003bfe21, 0x003c7761),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x003bff61),
	.Hoven = VA_COMPACT_OFFSET(0x003bff61, 0x003bf5a1),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003bff61, 0x003b74a1),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003bff61, 0x003b7261),
	.Metropolis = VA_COMPACT_OFFSET(0x003bff61, 0x003b6fe1),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003bff61, 0x003b7221),
	.CommandCenter = VA_COMPACT_OFFSET(0x003bff61, 0x003c77a1),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003bff61, 0x003c7ba1),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003bff61, 0x003c8321),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003bff61, 0x003c78a1),
#endif
};
// Weapon Obfuscate Address
variableAddressCompact_t vaPlayerObfuscateWeaponAddr = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x003aa041),
	.Hoven = VA_COMPACT_OFFSET(0x003aa041, 0x003a9681),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003aa041, 0x003a1581),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003aa041, 0x003a1341),
	.Metropolis = VA_COMPACT_OFFSET(0x003aa041, 0x003a10c1),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003aa041, 0x003a1301),
	.CommandCenter = VA_COMPACT_OFFSET(0x003aa041, 0x003b1881),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003aa041, 0x003b1c81),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003aa041, 0x003b2401),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003aa041, 0x003b1981),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x003aa181),
	.Hoven = VA_COMPACT_OFFSET(0x003aa181, 0x003a97c1),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003aa181, 0x003a16c1),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003aa181, 0x003a1481),
	.Metropolis = VA_COMPACT_OFFSET(0x003aa181, 0x003a1201),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003aa181, 0x003a1441),
	.CommandCenter = VA_COMPACT_OFFSET(0x003aa181, 0x003b19c1),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003aa181, 0x003b1dc1),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003aa181, 0x003b2541),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003aa181, 0x003b1ac1),
#endif
};
u32 playerDeobfuscate(u32 src, DeobfuscateMode_e mode)
{
	u8 *i = src;
	// i = address, *i = data
    if (mode == 0) {
        if (!*i)
            return 0;
    }

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
	do {
		u32 offset = (u32)((int)i - (u32)*i & 7) + n;
		n += stack.step;
        *data = stack.randData[(*i + (offset & 7) * stack.multiplyVal)];
        ++data;
	} while (n < stack.max);
	stack.addr = (u32)((u32)stack.val ^ ((u32)(stack.addr) ^ (u32)i));
	stack.val = (u32)stack.addr >> 0x10;
	if (mode == 0) {
		return stack.addr;
	} else if (mode == 1) {
        return stack.val & 0xff;
	} else if (mode == 2) {
		return stack.val;
	} else {
        return 0;
    }
}

void playerObfuscate(u32 src, int value, ObfuscateMode_e mode)
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
	u8 *i = src; // i: address, *i: value
	u8 *data = &stack.data;
    u8 *rData = &stack.randData;
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
    // if (*i == 0) {
    //     *(rData - 1) += 1;
    //     *i = *(rData - 1);
    // } else {
    //     char a = *i;
    //     *i = *rData;
    //     *rData = a;
    // }
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

variableAddressCompact_t vaSetPlayerStateFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00520b40),
	.Hoven = VA_COMPACT_OFFSET(0x00520b40, 0x00522c58),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00520b40, 0x00518530),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00520b40, 0x00515cc8),
	.Metropolis = VA_COMPACT_OFFSET(0x00520b40, 0x00515018),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00520b40, 0x005128b0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00520b40, 0x00512670),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00520b40, 0x00514ef0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00520b40, 0x005141f0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00520b40, 0x00513b70),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0051e2f0),
	.Hoven = VA_COMPACT_OFFSET(0x0051e2f0, 0x00520348),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0051e2f0, 0x00515c60),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0051e2f0, 0x00513478),
	.Metropolis = VA_COMPACT_OFFSET(0x0051e2f0, 0x005127c8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0051e2f0, 0x0050ffe0),
	.CommandCenter = VA_COMPACT_OFFSET(0x0051e2f0, 0x0050ff60),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0051e2f0, 0x005127a0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0051e2f0, 0x00511ae0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0051e2f0, 0x00511420),
#endif
};
void playerSetPlayerState(Player * player, u8 state)
{
    // aka: InitBodyState
    internal_SetPlayerState(player, state, 1, 0, 1);
}

//--------------------------------------------------------------------------------
variableAddressCompact_t vaHurtPlayerFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0050d4d8),
	.Hoven = VA_COMPACT_OFFSET(0x0050d4d8, 0x0050f5f0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0050d4d8, 0x00504ec8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0050d4d8, 0x00502660),
	.Metropolis = VA_COMPACT_OFFSET(0x0050d4d8, 0x005019b0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0050d4d8, 0x004ff248),
	.CommandCenter = VA_COMPACT_OFFSET(0x0050d4d8, 0x004ff210),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0050d4d8, 0x00501a90),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0050d4d8, 0x00500d90),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0050d4d8, 0x00500710),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0050ace8),
	.Hoven = VA_COMPACT_OFFSET(0x0050ace8, 0x0050cd40),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0050ace8, 0x00502658),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0050ace8, 0x004ffe70),
	.Metropolis = VA_COMPACT_OFFSET(0x0050ace8, 0x004ff1c0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0050ace8, 0x004fc9d8),
	.CommandCenter = VA_COMPACT_OFFSET(0x0050ace8, 0x004fcb60),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0050ace8, 0x004ff3a0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0050ace8, 0x004fe6e0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0050ace8, 0x004fe020),
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
void playerSetMaxHealth(Player * player, int health)
{
    if (!player)
        return;

    // set player struct max health
    playerObfuscate(&player->maxHP, health, OBFUSCATE_MODE_HEALTH);
	// set Resurrect max health variable
    POKE_U16(((u32)GetAddress(&vaPlayerRespawnFunc) + 0x140), FLOAT_TO_U16(health));
    // set player pvar max health
	*(float*)((u32)player->pMoby->pVar + 0x30) = (float)health;
}

int playerGetMaxHealth(Player * player)
{
    u32 fMaxHP = playerDeobfuscate(&player->maxHP, 0);
    return *(float*)&fMaxHP;
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
int playerStateIsDead(int state)
{
    // return player->pNetPlayer->pNetPlayerData->hitPoints <= 0;
    // int Health = (int)playerGetHealth(player) <= 0;
    int CurrentState = state == PLAYER_STATE_DEATH
        || state == PLAYER_STATE_DROWN
        || state == PLAYER_STATE_DEATH_FALL
        || state == PLAYER_STATE_DEATHSAND_SINK
        || state == PLAYER_STATE_LAVA_DEATH
        || state == PLAYER_STATE_DEATH_NO_FALL
        || state == PLAYER_STATE_WAIT_FOR_RESURRECT
        || state == PLAYER_STATE_ICEWATER_FREEZE
        ;
    return CurrentState;
}

//--------------------------------------------------------------------------------
variableAddressCompact_t vaPlayerSetPosRotFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00509dc0),
	.Hoven = VA_COMPACT_OFFSET(0x00509dc0, 0x0050bed8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00509dc0, 0x005017b0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00509dc0, 0x004fef48),
	.Metropolis = VA_COMPACT_OFFSET(0x00509dc0, 0x004fe298),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00509dc0, 0x004fbb30),
	.CommandCenter = VA_COMPACT_OFFSET(0x00509dc0, 0x004fbaf8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00509dc0, 0x004fe378),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00509dc0, 0x004fd678),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00509dc0, 0x004fcff8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005075d0),
	.Hoven = VA_COMPACT_OFFSET(0x005075d0, 0x00509628),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005075d0, 0x004fef40),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005075d0, 0x004fc758),
	.Metropolis = VA_COMPACT_OFFSET(0x005075d0, 0x004fbaa8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005075d0, 0x004f92c0),
	.CommandCenter = VA_COMPACT_OFFSET(0x005075d0, 0x004f9448),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005075d0, 0x004f92c0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005075d0, 0x004fafc8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005075d0, 0x004fa908),
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

variableAddressCompact_t vaGiveWeaponUpgradeFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005472a0),
	.Hoven = VA_COMPACT_OFFSET(0x005472a0, 0x00549468),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005472a0, 0x0053ed40),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005472a0, 0x0053c428),
	.Metropolis = VA_COMPACT_OFFSET(0x005472a0, 0x0053b828),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005472a0, 0x00539010),
	.CommandCenter = VA_COMPACT_OFFSET(0x005472a0, 0x00538868),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005472a0, 0x0053b0e8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005472a0, 0x0053a3e8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005472a0, 0x00539d68),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00544998),
	.Hoven = VA_COMPACT_OFFSET(0x00544998, 0x00546aa0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00544998, 0x0053c3b8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00544998, 0x00539b20),
	.Metropolis = VA_COMPACT_OFFSET(0x00544998, 0x00538f20),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00544998, 0x00536688),
	.CommandCenter = VA_COMPACT_OFFSET(0x00544998, 0x005360b8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00544998, 0x005388f8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00544998, 0x00537c38),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00544998, 0x00537578),
#endif
};
void playerGiveWeaponUpgrade(Player * player, int weaponId)
{
    internal_playerGiveWeaponUpgrade((u32)player + 0x1a40, weaponId);
}

variableAddressCompact_t vaGiveWeaponFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00546c78),
	.Hoven = VA_COMPACT_OFFSET(0x00546c78, 0x00548e40),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00546c78, 0x0053e718),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00546c78, 0x0053be00),
	.Metropolis = VA_COMPACT_OFFSET(0x00546c78, 0x0053b200),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00546c78, 0x005389e8),
	.CommandCenter = VA_COMPACT_OFFSET(0x00546c78, 0x00538240),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00546c78, 0x0053aac0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00546c78, 0x00539dc0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00546c78, 0x00539740),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00544370),
	.Hoven = VA_COMPACT_OFFSET(0x00544370, 0x00546478),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00544370, 0x0053bd90),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00544370, 0x005394f8),
	.Metropolis = VA_COMPACT_OFFSET(0x00544370, 0x005388f8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00544370, 0x00536060),
	.CommandCenter = VA_COMPACT_OFFSET(0x00544370, 0x00535a90),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00544370, 0x005382d0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00544370, 0x00537610),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00544370, 0x00536f50),
#endif
};
void playerGiveWeapon(Player * player, int weaponId, int type)
{
    internal_playerGiveWeapon((u32)player + 0x1a40, weaponId, type);
}

variableAddressCompact_t vaEquipWeaponFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005471b8),
	.Hoven = VA_COMPACT_OFFSET(0x005471b8, 0x00549380),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005471b8, 0x0053ec58),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005471b8, 0x0053c340),
	.Metropolis = VA_COMPACT_OFFSET(0x005471b8, 0x0053b740),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005471b8, 0x00538f28),
	.CommandCenter = VA_COMPACT_OFFSET(0x005471b8, 0x00538780),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005471b8, 0x0053b000),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005471b8, 0x0053a300),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005471b8, 0x00539c80),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005448b0),
	.Hoven = VA_COMPACT_OFFSET(0x005448b0, 0x005469b8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005448b0, 0x0053c2d0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005448b0, 0x00539a38),
	.Metropolis = VA_COMPACT_OFFSET(0x005448b0, 0x00538e38),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005448b0, 0x005365a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x005448b0, 0x00535fd0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005448b0, 0x00538810),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005448b0, 0x00537b50),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005448b0, 0x00537490),
#endif
};
void playerEquipWeapon(Player * player, int weaponId)
{
    internal_playerEquipWeapon((u32)player + 0x1a40, weaponId);
}

variableAddressCompact_t vaGiveMeRandomWeaponsFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00519bd0),
	.Hoven = VA_COMPACT_OFFSET(0x00519bd0, 0x0051bce8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00519bd0, 0x005115c0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00519bd0, 0x0050ed58),
	.Metropolis = VA_COMPACT_OFFSET(0x00519bd0, 0x0050e0a8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00519bd0, 0x0050b940),
	.CommandCenter = VA_COMPACT_OFFSET(0x00519bd0, 0x0050b700),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00519bd0, 0x0050df80),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00519bd0, 0x0050d280),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00519bd0, 0x0050cc00),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005173c8),
	.Hoven = VA_COMPACT_OFFSET(0x005173c8, 0x00519420),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005173c8, 0x0050ed38),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005173c8, 0x0050c550),
	.Metropolis = VA_COMPACT_OFFSET(0x005173c8, 0x0050b8a0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005173c8, 0x005090b8),
	.CommandCenter = VA_COMPACT_OFFSET(0x005173c8, 0x00509038),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005173c8, 0x0050b878),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005173c8, 0x0050abb8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005173c8, 0x0050a4f8),
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

int playerIsGadgetAvailable(Player *player, eGADGET_IDs gadget_id)
{
    return playerDeobfuscate(&player->availableGadgets, DEOBFUSCATE_MODE_GADGET) & (1 << gadget_id);
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

int playerMapHealth(int health)
{
    const int diff[3] = {6, 7, 7};
    int result = 0;
	int i = 0;

    for (i = 0; i < health; i++) {
        result += diff[i % 3];
    }

    return result;
}

int playerGetLatency(Player *player)
{
    if (!player || GAME_NET_INFO->onlyLocalPlayers || player->isLocal)
        return 0;

    // Same indexing logic
    int clientId = player->pNetPlayer->netClientIndex;
    int netClientMap = GAME_NET_INFO->clientIndexRemapper[clientId];
    int lastPacketTime = GAME_NET_INFO->m_LastUdpPacketReceived[netClientMap];
    int deltaTime = GAME_NET_INFO->netFrameTime - lastPacketTime;

    return deltaTime;
}

int playerIsLinkHealthy(Player *player)
{
    return playerGetLatency(player) <= 150;
}
