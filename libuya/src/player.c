#include "string.h"
#include "player.h"
#include "interop.h"
#include "game.h"


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

#if UYA_PAL

VariableAddress_t vaPlayerStructArray = {
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
};

#else

VariableAddress_t vaPlayerStructArray = {
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
};

#endif

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

    
    player->Team = teamId;
    //player->PlayerMoby->SecondaryColor = TEAM_COLORS[teamId];
    //player->SkinMoby->TextureId = 0x80 + (8 * teamId);
    //player->SkinMoby->UNK_BE = 0;
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

    // if (playerIsLocal(player))
    // {
    //     return player->Paddata;
    // }
    // else
    // {
    //     u8 * remotePadInfo = player->RemotePadInfo;
    //     if (!remotePadInfo)
    //         return 0;

    //     return (PadButtonStatus*)(remotePadInfo + 0x70);
    // }
    return player->Paddata;
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
                padHistory->id = player->PlayerId;
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
            (PlayerPadHistory[player->PlayerId].btns & buttonMask) != 0;
}

//--------------------------------------------------------------------------------
int playerPadGetButtonUp(Player * player, u16 buttonMask)
{
    if (!player)
        return 0;

    return !playerPadGetButton(player, buttonMask) &&
        (PlayerPadHistory[player->PlayerId].btns & buttonMask) != 0;
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
    internal_SetPlayerState(player, state, 2, 0);
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
void playerDecHealth(Player * player, u8 amount)
{
    internal_HurtPlayer(player, amount);
}

//--------------------------------------------------------------------------------
void playerIncHealth(Player * player, u8 amount)
{
    if (playerGetHealth(player) >= 15)
    {
        playerSetHealth(player, 15);
    }
    else
    {
        // Grab address where math is done
        int math = ((u32)GetAddress(&vaHurtPlayerFunc) + 0xb0);
        // Change to addition
        *(u8*)math = 0;
        // Run normal function
        internal_HurtPlayer(player, amount);
        // Revert back to subtraction
        *(u8*)math = 1;
    }
}

void playerSetHealth(Player * player, u8 amount)
{
    // Make sure amount is not more than the max health.
    if (amount > PLAYER_MAX_HEALTH)
        amount = 15;

    // Grab address where math is done
    int math = ((u32)GetAddress(&vaHurtPlayerFunc) + 0xb0);
    // Instead of subtracting, move f01 to f00.
    *(u32*)math = 0x46000806; // mov.s $f0, $f1
    // Run normal function
    internal_HurtPlayer(player, amount);
    // Revert back to subtraction
    *(u32*)math = 0x46010001; // sub.s $f0, $f0, $f1
}

int playerGetHealth(Player * player)
{
    // Grab address where math is done
    int math = ((u32)GetAddress(&vaHurtPlayerFunc) + 0xb0);
    // Remove math.
    *(u32*)math = 0;
    // Run normal function
    internal_HurtPlayer(player, 1);
    // Revert back to subtraction
    *(u32*)math = 0x46010001; // sub.s $f0, $f0, $f1
    float CurrentHealth;
    asm __volatile__ (
        "swc1 $f0, 0x0(%0);"
        : 
        : "r" (&CurrentHealth)
    );
    return CurrentHealth;
}
