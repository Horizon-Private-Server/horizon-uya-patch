#include <tamtypes.h>
#include <string.h>

#include <libuya/time.h>
#include <libuya/math.h>
#include <libuya/math3d.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/player.h>
#include <libuya/camera.h>
#include <libuya/ui.h>
#include <libuya/stdio.h>
#include <libuya/graphics.h>
#include <libuya/pad.h>
#include <libuya/uya.h>
#include <libuya/utils.h>
#include <libuya/interop.h>

const float CAMERA_POSITION_SHARPNESS = 50;
const float CAMERA_ROTATION_SHARPNESS = 30;
const float VEHICLE_CAMERA_ROTATION_SHARPNESS = 3;

const float VEHICLE_DISTANCE[] =
{
    3,                  // Turboslider
    0,                  // Turboslider Passenger
    2,                  // Hovership
    0,                  // Hovership Passenger
	3,					// Tank
	0,					// Tank Passenger
};

const float VEHICLE_ELEVATION[] =
{
    3,                  // Turboslider
    3,                  // Turboslider Passenger
    2.5,                // Hovership
    3,                  // Hovership Passenger
	3,					// Tank
	3,					// Tank Passanger
};

struct PlayerSpectateData
{
    int Enabled;
    int Index;
    int HasShownEnterMsg;
    int HasShownNavMsg;
    VECTOR LastCameraPos;
    float LastCameraZ;
    float LastCameraY;
} SpectateData[2];

int InitSpectate = 0;

VariableAddress_t vaGetMissionDefHook = {
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
	.Bakisi = 0x00545210,
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

Player* playerGetFromSlot_Hook(int i)
{
    if (SpectateData[i].Enabled) {
        Player** players = playerGetAll();
        Player* player = players[SpectateData[i].Index];
        if (player)
            return player;
    }
    
    return playerGetFromSlot(0);
}

void enableSpectate(Player * player, struct PlayerSpectateData * data)
{
	// if (GetAddress(&vaGetMissionDefHook)) {
	// 	u32 hookv = 0x0C000000 | ((u32)&playerGetFromSlot_Hook >> 2);
	// 	*(u32*)GetAddress(&vaGetMissionDefHook) = hookv;
	// 	*(u32*)0x004CC030 = hookv;
	// 	*(u32*)0x00547F3C = hookv;
	// 	*(u32*)0x0048DCA8 = hookv;
	// 	*(u32*)0x003D6344 = hookv;
	// 	*(u32*)0x004C1880 = hookv;
	// 	*(u32*)0x004D5FFC = hookv;
	// 	*(u32*)0x004D2274 = hookv;
	// 	*(u32*)0x004B1BAC = hookv;
	// 	*(u32*)0x00545C0C = hookv;
	// 	*(u32*)0x004D69A8 = hookv;
	// 	*(u32*)0x004D7518 = hookv;
	// 	*(u32*)0x004EC8A0 = hookv;
	// 	*(u32*)0x00545C0C = hookv;
	// }

    // Fixes void fall bug
    // *((u8*)0x00171DE0 + player->PlayerId) = 1;
    // *(u32*)0x004DB88C = 0;
    // *(u32*)0x005542D8 = 0x10000017; // allow min/max map when dead
    // *(u32*)0x005F4198 = 0; // disables weird camera yaw when spectated player respawns

    // *(u32*)0x0054F46C = hookv; // healthbar
    // *(u32*)0x0054f898 = hookv; // healthbar widgets
    // *(u32*)0x00541708 = hookv; // get current gadget
    // *(u32*)0x005418c4 = hookv; // get gadget version name
    // *(u32*)0x00541f3c = hookv; // get player hud team
    // *(u32*)0x0054209c = hookv; // get gadget color
    // *(u32*)0x00552bd0 = hookv; // ammo update ammo
    // *(u32*)0x00555904 = hookv; // radar update map
    // *(u32*)0x0055615c = hookv; // radar update blip

    data->Enabled = 1;
    // hudGetPlayerFlags(player->LocalPlayerIndex)->Flags.Weapons = 0;
}

void disableSpectate(Player * player, struct PlayerSpectateData * data)
{
    // *((u8*)0x00171DE0 + player->PlayerId) = 0;
    // *(u32*)0x004DB88C = 0xA48200E0;
    // *(u32*)0x005542D8 = 0x45010182; // disable min/max map when dead
    // *(u32*)0x005F4198 = 0x3042FFFD; // re-enable weird camera yaw when spectated player respawns

    data->Enabled = 0;
    // PlayerHUDFlags* pHUD = hudGetPlayerFlags(player->LocalPlayerIndex);
    // if (pHUD)
    //     pHUD->Flags.Weapons = 1;
}

void spectate(Player * currentPlayer, Player * playerToSpectate)
{
    float cameraT;
    struct PlayerSpectateData * spectateData = SpectateData + currentPlayer->mpIndex;
    if(!playerToSpectate)
        return;

    // Fix void fall spectate bug
    currentPlayer->fps.Vars.flags = 2;

    currentPlayer->fps.Vars.CameraYMin = playerToSpectate->fps.Vars.CameraYMin;
    currentPlayer->fps.Vars.CameraYMax = playerToSpectate->fps.Vars.CameraYMax;
    currentPlayer->fps.Vars.CameraPositionOffset[0] = -6;

    if (playerToSpectate->Vehicle) {
        struct Moby * vehicleMoby = playerToSpectate->Vehicle->pMoby;
        int isPassenger = playerToSpectate->Vehicle->pPassenger == playerToSpectate;
        cameraT = 1 - powf(MATH_E, -(isPassenger ? CAMERA_ROTATION_SHARPNESS : VEHICLE_CAMERA_ROTATION_SHARPNESS) * MATH_DT);

        // Grab rotation
        float yaw = playerToSpectate->Vehicle->netUpdatedRot[2];
        float pitch = 0.08;
        float distance = 0;
        float elevation = 0;
        
        // Determine distance and elevation by vehicle id
        switch (vehicleMoby->OClass) {
            case MOBY_ID_TURBOSLIDER: {
                distance = VEHICLE_DISTANCE[0 + isPassenger];
                elevation = VEHICLE_ELEVATION[0 + isPassenger];
                if (isPassenger) {
                    yaw = playerToSpectate->Vehicle->netUpdatedPassengerRot[2];
                    pitch = playerToSpectate->Vehicle->netUpdatedPassengerRot[1] + (float)0.08;
                }
                break;
            }
            case MOBY_ID_HOVERSHIP: {
                distance = VEHICLE_DISTANCE[2 + isPassenger];
                elevation = VEHICLE_ELEVATION[2 + isPassenger];
                if (isPassenger) {
                    yaw = playerToSpectate->Vehicle->netUpdatedPassengerRot[2];
                    pitch = playerToSpectate->Vehicle->netUpdatedPassengerRot[1];
                }
                break;
            }
			case MOBY_ID_TANK: {
                distance = VEHICLE_DISTANCE[4 + isPassenger];
                elevation = VEHICLE_ELEVATION[4 + isPassenger];
                if (isPassenger) {
                    yaw = playerToSpectate->Vehicle->netUpdatedPassengerRot[2];
                    pitch = playerToSpectate->Vehicle->netUpdatedPassengerRot[1] + (float)0.08;
                }
                break;
			}
        }

        // Interpolate camera rotation towards target player
        float lerpZ = lerpfAngle(spectateData->LastCameraZ, yaw, cameraT);
        currentPlayer->fps.Vars.CameraZ.rotation = lerpZ;
        spectateData->LastCameraZ = lerpZ;
        float lerpY = lerpfAngle(spectateData->LastCameraY, pitch, cameraT);
        currentPlayer->fps.Vars.CameraY.rotation = lerpY;
        spectateData->LastCameraY = lerpY;
        
        // Generate target based off distance and elevation
        VECTOR target;
        vector_copy(target, playerToSpectate->Vehicle->pMoby->Position);
        target[0] -= cosf(spectateData->LastCameraZ) * distance;
        target[1] -= sinf(spectateData->LastCameraZ) * distance;
        target[2] += (sinf(spectateData->LastCameraY) * distance) + elevation;

        // Interpolate camera towards target player
        vector_lerp(spectateData->LastCameraPos, spectateData->LastCameraPos, target, 1 - powf(MATH_E, -CAMERA_POSITION_SHARPNESS * MATH_DT));
        vector_copy(currentPlayer->fps.CameraPos, spectateData->LastCameraPos);
    } else {
        cameraT = 1 - powf(MATH_E, -CAMERA_ROTATION_SHARPNESS * MATH_DT);

        // Interpolate camera rotation towards target player
        float playerToSpectateZ = playerToSpectate->fps.Vars.CameraZ.rotation;
        currentPlayer->fps.Vars.CameraZ.rotation = playerToSpectateZ;
        spectateData->LastCameraZ = playerToSpectateZ;
        float playerToSpectateY = playerToSpectate->fps.Vars.CameraY.rotation;
        currentPlayer->fps.Vars.CameraY.rotation = playerToSpectateY;
        spectateData->LastCameraY = playerToSpectateY;

        // Interpolate camera towards target player
        vector_lerp(spectateData->LastCameraPos, spectateData->LastCameraPos, playerToSpectate->fps.CameraPos, .5);
        vector_copy(currentPlayer->fps.CameraPos, spectateData->LastCameraPos);
    }
}

int findNextPlayerIndex(int currentPlayerIndex, int currentSpectateIndex, int direction)
{
    Player ** players = playerGetAll();
    // int teamOnly = gameConfig.customModeId == CUSTOM_MODE_SEARCH_AND_DESTROY || gameConfig.customModeId == CUSTOM_MODE_INFECTED;
    int teamOnly = 0;
	int newIndex = currentSpectateIndex;

    do {
    loop:
        newIndex += direction;

        if (newIndex < 0)
            newIndex = GAME_MAX_PLAYERS - 1;
        else if (newIndex >= GAME_MAX_PLAYERS)
            newIndex = 0;
        // Fail if the only player is us
        if(newIndex == currentPlayerIndex && newIndex == currentSpectateIndex)
            return -1;
        // prevent infinite loop
        if (newIndex == currentSpectateIndex)
            return players[currentSpectateIndex] ? currentSpectateIndex : -1;
        // skip self
        if (newIndex == currentPlayerIndex)
            goto loop;
        // skip enemy team
        if (teamOnly && players[newIndex] && players[newIndex]->mpTeam != players[currentPlayerIndex]->mpTeam)
            goto loop;
    }
    while(!players[newIndex]);
    
    return newIndex;
}

void Init(void)
{
    memset(SpectateData, 0, sizeof(SpectateData));
    InitSpectate = 1;
}

void runSpectate(void)
{
	GameSettings * gameSettings = gameGetSettings();
	Player ** players = playerGetAll();
	struct PlayerSpectateData * spectateData = 0;
	int i = 0;
	int direction = 0;
	int spectateIndex = 0;

	// First, we have to ensure we are in-game
	if (!gameSettings || !isInGame()) {
		SpectateData->Enabled = 0;
		InitSpectate = 0;
		return;
	}

	if (InitSpectate != 1)
		Init();

	// Loop through every player
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (!players[i])
			continue;

		Player * player = players[i];

		// Next, we have to ensure the player is the local player and they are dead
		if (playerIsLocal(player) || playerGetHealth(player) <= 0) {
			// Grab player-specific spectate data
			spectateData = SpectateData + player->mpIndex;
			spectateIndex = spectateData->Index;

			// If dead
			if (playerIsDead(player)) {
				if (!spectateData->Enabled) {
					// Show help message when player dies
					if (!spectateData->HasShownEnterMsg) {
						spectateData->HasShownEnterMsg = 1;
						uiShowPopup(player, "Press \x13 to enter spectate mode.", 5);
					}

					// When the player presses square and spectate isn't already enabled. Try to enable it.
					if (playerPadGetButtonDown(player, PAD_SQUARE) > 0) {
						// First check if there is a player to spectate
						spectateIndex = findNextPlayerIndex(i, spectateIndex, 1);
						if (spectateIndex >= 0) {
							enableSpectate(player, spectateData);
							spectateData->Index = spectateIndex;
							vector_copy(spectateData->LastCameraPos, players[spectateIndex]->fps.CameraPos);
						}
					}
				}
				// Let the player exit spectate by pressing square
				else if (playerPadGetButtonDown(player, PAD_SQUARE) > 0) {
					disableSpectate(player, spectateData);
				}
				// If the actively spectated player left find the next player
				else if (!players[spectateIndex]) {
					// First check if there is a player to spectate
					spectateIndex = findNextPlayerIndex(i, spectateIndex, 1);
					if (spectateIndex < 0)
						disableSpectate(player, spectateData);

					// Update spectate index
					spectateData->Index = spectateIndex;
				} else {
					// Show nav message
					if (!spectateData->HasShownNavMsg) {
						spectateData->HasShownNavMsg = 1;
						uiShowPopup(player, "Use \x14 and \x15 to navigate between players.", 5);
					}
				
					// If the currently spectated player becomes null, we move forward to the next player            
					if (!players[spectateIndex])
						direction = 1;
					// If the player is pressing R1, move forward
					else if (playerPadGetButtonDown(player, PAD_R1) > 0) 
						direction = 1;
					// If the player is pressing L1, move backward
					else if (playerPadGetButtonDown(player, PAD_L1) > 0)
						direction = -1;

					// 
					if (direction)
						spectateIndex = findNextPlayerIndex(i, spectateIndex, direction);

					if (spectateIndex >= 0) {
						Player * nextPlayer = players[spectateIndex];
						if (nextPlayer) {
							// Update last camera position to new target
							// This snaps the camera to the new target instead of lerping
							if (spectateIndex != spectateData->Index)
								vector_copy(spectateData->LastCameraPos, nextPlayer->fps.CameraPos);

							spectate(player, nextPlayer);
						}
					}
					// Finally update stored index value
					spectateData->Index = spectateIndex;
				}
			} else if (spectateData->Enabled) {
				disableSpectate(player, spectateData);
			}
		}
	}
}
