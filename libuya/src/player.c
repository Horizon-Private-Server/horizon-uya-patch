#include "string.h"
#include "player.h"
#include "interop.h"
#include "game.h"
#include "spawnpoint.h"

void playerRespawn(Player * player);
void playerStripWeapons(Player * player);
void internal_HurtPlayer(Player * player, float health);

VariableAddress_t vaEmpty = {
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

/*
 * 
 */
#define PLAYER_STRUCT_ARRAY                         ((Player**)GetAddress(&vaPlayerStructArray))

/*
 * Local player 1 dme player index.
 */
#define PLAYER_1_ID                                 (*(u32*)0x0017218C)

/*
 * Local player 2 dme player index.
 */
#define PLAYER_2_ID                                 (*(u32*)0x001B6ED8)

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
// void playerSetLocalEquipslot(int localPlayerId, int slot, int weaponId)
// {
//     int * equipslots = WEAPON_EQUIPSLOT;
//     equipslots[slot + (localPlayerId * 3)] = weaponId;
// }

//--------------------------------------------------------------------------------
// void playerSetWeapon(Player * player, int weaponId)
// {
//     if (!player)
//         return;

//     player->ChangeWeaponHeldId = weaponId;
// }

//--------------------------------------------------------------------------------
void playerSetTeam(Player * player, int teamId)
{
    if (!player)
        return;

    player->mpTeam = teamId;
    // player->PlayerMoby->GlowRGBA = TEAM_COLORS[teamId];
    player->PlayerMoby->ModeBits2 = player->PlayerMoby->ModeBits2 & 0x8f | 0x80 | (teamId << 4);
    // player->PlayerMoby->Triggers = 0;
}

//--------------------------------------------------------------------------------
int playerIsLocal(Player * player)
{
    return player->IsLocal;
}

//--------------------------------------------------------------------------------
PadButtonStatus * playerGetPad(Player * player)
{
    if (!player)
        return 0;

    if (playerIsLocal(player))
    {
        return player->Paddata;
    }
    else
    {
        struct tNW_Player * netPlayer = player->pNetPlayer;
        if (!netPlayer)
            return 0;

        return (PadButtonStatus*)(&netPlayer->padMessageElems[0].msg.pad_data);
    }
}

//--------------------------------------------------------------------------------
void playerPadUpdate(void)
{
    int i;
    PadButtonStatus * playerPad;
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
                padHistory->id = player->fps.Vars.camSettingsIndex;
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

    PadButtonStatus * paddata = playerGetPad(player);
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
            (PlayerPadHistory[player->fps.Vars.camSettingsIndex].btns & buttonMask) != 0;
}

//--------------------------------------------------------------------------------
int playerPadGetButtonUp(Player * player, u16 buttonMask)
{
    if (!player)
        return 0;

    return !playerPadGetButton(player, buttonMask) &&
        (PlayerPadHistory[player->fps.Vars.camSettingsIndex].btns & buttonMask) != 0;
}

//--------------------------------------------------------------------------------
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
    if (((int)playerGetHealth(player) + health) >= PLAYER_MAX_HEALTH)
    {
        playerSetHealth(player, PLAYER_MAX_HEALTH);
        return;
    }
    else
    {
        // Grab address where math is done
        int math = ((u32)GetAddress(&vaHurtPlayerFunc) + 0xb0);
        // Change to addition
        *(u8*)math = 0;
        // Run normal function
        internal_HurtPlayer(player, health);
        // Revert back to subtraction
        *(u8*)math = 1;
    }
}

void playerSetHealth(Player * player, int health)
{
    // Grab address where math is done
    int math = ((u32)GetAddress(&vaHurtPlayerFunc) + 0xb0);
    // Instead of subtracting, move f01 to f00.
    *(u32*)math = 0x46000806; // mov.s $f0, $f1
    // Run normal function
    internal_HurtPlayer(player, (health > PLAYER_MAX_HEALTH) ? PLAYER_MAX_HEALTH : health);
    // Revert back to subtraction
    *(u32*)math = 0x46010001; // sub.s $f0, $f0, $f1
}

float playerGetHealth(Player * player)
{
    return player->pNetPlayer->pNetPlayerData->hitPoints;
}
//--------------------------------------------------------------------------------
int playerIsDead(Player * player)
{
	return player->pNetPlayer->pNetPlayerData->hitPoints <= 0;
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
void playerSetPosRot(Player * player, u32 * vPosition, u32 * vRotation)
{
    /*
        a4: warpToState
        t0: Unk
        t1: resurrecting
        t2: dropFlag
    */
    if (!vPosition || !vRotation)
    {
        SpawnPointPosRot pos;
        spawnPointGetRandom(player, &pos.position, &pos.rotation);
        internal_playerWarp(player, &pos.position, &pos.rotation, 0, 1, 1, 1);
        return;
    }

    internal_playerWarp(player, vPosition, vRotation, 0, 1, 1, 1);
}

//--------------------------------------------------------------------------------
PlayerVTable * playerGetVTable(Player * player)
{
    if (!player)
        return NULL;

    return (PlayerVTable*)player->Guber.Vtable;
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
void playerGiveWeapon(Player * player, int weaponId)
{
    internal_playerGiveWeapon((u32)player + 0x1a40, weaponId, 1);
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
    internal_GiveMeRandomWeapons(player, (!amount) ? 3 : amount);
}

// Other Obfuscate Address (Health, State)
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
u32 playerDeobfuscate(u32 src, int addr, int mode)
{
    static int StackAddr[3];
    u32 Player_Addr = src;
    u32 Player_Value = *(u8*)Player_Addr;
    int RandDataAddr = !addr ? GetAddress(&vaPlayerObfuscateAddr) : GetAddress(&vaPlayerObfuscateWeaponAddr);
    int n = 0;
	int rawr = 0;
    u32 Converted;
    // Mode 0: For Health and Player State (Possibly more)
    if (mode == 0) {
        do {
            u32 Offset = (u32)((int)Player_Addr - (u32)Player_Value & 7) + n;
            n = n + 5;
            *(u8*)((int)StackAddr + rawr) = *(u8*)((u32)RandDataAddr + (Player_Value + (Offset & 7) * 0xff));
            ++rawr;
        } while (n < 0x28);
        // XORAddr (first 4 bytes of StackAddr)
        StackAddr[0] = (u32)(StackAddr[0]) ^ (u32)Player_Addr;
        // XORValue (second 4 bytes of StackAddr)
        StackAddr[1] = (u32)((u32)StackAddr[1] ^ StackAddr[0]);
        Converted = StackAddr[1];
    // Mode 1: Meant for Weapons (and other items)
    } else if (mode == 1) {
        do {
            u32 Offset = (u32)((u32)Player_Addr - (u8)Player_Value & 7) + n;
            n = n + 3;
            *(u8*)((u32)StackAddr + rawr) = *(u8*)((u32)RandDataAddr + (Offset & 7) * 0xd1 + (u8)Player_Value);
            ++rawr;
        } while (n < 0x18);
        StackAddr[0] = (u32)StackAddr[0] ^ (u32)Player_Addr;
        StackAddr[2] = ((u32)StackAddr[1] ^ (u32)StackAddr[0]) >> 0x10;
        n = (u32)((u32)StackAddr[1] ^ (u32)StackAddr[0] ^ (u32)StackAddr[2]) * 0x10000 + 1;
        StackAddr[1] = (u32)StackAddr[2] / n;
        // u64 hi is used later on in the UYA code, keeping just in case
        // u64 hi = (u64)((u32)StackAddr[2] % n);
        StackAddr[2] = (u32)StackAddr[2] & 0xff;
        Converted = StackAddr[2];
    }
    return Converted;
}
