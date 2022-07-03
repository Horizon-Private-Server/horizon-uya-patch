/***************************************************
 * FILENAME :		player.h
 * 
 * DESCRIPTION :
 * 		Contains player specific offsets and structures for UYA.
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_PLAYER_H_
#define _LIBUYA_PLAYER_H_

#include "vehicle.h"
#include "math.h"
#include "pad.h"
#include "math3d.h"
#include "common.h"
#include "guber.h"

/*
 * Maximum health of player.
 */
#define PLAYER_MAX_HEALTH                   (15)
#define PLAYER_HEALTH_ADDR                  (*(float*)0x00235964)

#define PLAYER_STRUCT_POINTER               (*(u32*)0x00225e70)
#define PLAYER_STRUCT                       (PLAYER_STRUCT_POINTER - 0x430C)

/*
 * Weapon Info
 * NOTE: Setting a value to this address may have odd effects
 * usually fires the selected weapon once then switches to the programatically
 * set weapon and continues firing. Does not update the currently selected weapon
 * image in the top corner and ammo count
 */
//#define WEAPON_ADDRESS                      0x20301190

enum WeaponType {
    WEAPON_TYPE_N60 = 1,
    WEAPON_TYPE_GBOMB = 5,
    WEAPON_TYPE_ROCKET = 4,
    WEAPON_TYPE_LAVA = 7,
    WEAPON_TYPE_BLITZ = 2,
    WEAPON_TYPE_MINE = 6,
    WEAPON_TYPE_FLUX = 3,
    WEAPON_TYPE_MORPH = 9,
    WEAPON_TYPE_HOLO = 12
 //   WEAPON_TYPE_ = ,
}

/*
 * NAME :		PlayerCameraType
 * 
 * DESCRIPTION :
 * 			Defines the three player camera types.
 *          These are used in the player struct under 'CameraType'
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
enum PlayerCameraType
{
    CAMERA_TYPE_THIRD_PERSON,
    CAMERA_TYPE_LOCK_STRAFE,
    CAMERA_TYPE_FIRST_PERSON
};

/*
 * NAME :		CameraAngle
 * 
 * DESCRIPTION :
 * 			Contains camera angle settings and information.
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct CameraAngle
{
    float Value;
    float Velocity;
    float MaxVelocity;
    float UNK0;
    float Acceleration;
    float UNK1[3];
} CameraAngle;

/*
 * NAME :		Functions
 * 
 * DESCRIPTION :
 * 			Contains Functions.  Unsure what all functions.
 * 
 * NOTES :
 * 
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */
typedef struct Functions
{
    int UNK0[0x33];
    int Player;                                                      // 0x34
} Functions;

/*
 * NAME :		Player
 * 
 * DESCRIPTION :
 * 			Contains the player data.
 * 
 * NOTES :
 *          This is very large and documentation is incomplete.
 *          Much of the padding is to skip over area's not yet understood.
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
typedef struct Player
{
    char UNK15[0x13];                                               // 0x00

    Functions Function;                                             // 0x14

    char UNK16[0x8C];

    VECTOR PlayerPosition;                                          // 0xA0

    char UNK6[0x80];

    VECTOR PlayerVelocity;                                          // 0x120

    char UNK0[0x200];
    
    short CantMoveTimer;                                            // 0x320

    char UNK13[0x06];

    float WeaponCooldownTimer;                                      // 0x328

    char UNK14[0x16];

    short GravityWallTimer;                                         // 0x33E

    char UNK5[0x11A];

    // is 1 is player is shooting
    int IsShooting;                                                 // 0x458

    char UNK3[0xD28];

    VECTOR CameraPos;                                               // 0x1180

    char UNK1[0x50];

    CameraAngle CameraYaw;                                          // 0x11E0
    CameraAngle CameraPitch;                                        // 0x1200

    char UNk_1204[0x2c];

    // Moby * LookAtMoby not implamented yet.
    int LookAtMoby;                                                 // 0x122C

    char UNK4[0x28];

    float CameraPitchMin;                                           // 0x1254
    float CameraPitchMax;                                           // 0x1258

    // In libdl this is: Moby * SkinMoby;
    // Using int for now because moby.h is not impamented yet.
    int SkinMoby;                                                   // 0x125C

    float CameraDistance;                                           // 0x1260

    char UNK_1260[0x6c];

    int LocalPlayerIndex;                                           // 0x12CC

    char UNK_12CC[0xC];

    int PlayerId;                                                   // 0x12D8

    char UNK7[0x730];

    char HideWeapon;                                                // 0x1A08
    char HidePlayer;                                                // 0x1A09
    char UNK8[0x4];
    char WrenchOnly;                                                // 0x1A0D
    char UNK9[0x1];
    char SwingShotOnly;                                             // 0x1A10
    char ForceWrenchSwitch;                                         // 0x1A11
    char UNK10[0x4];
    char ShieldTrigger;                                             // 0x1A15

    char UNK12[0x993];

    Vehicle * Vehicle;                                              // 0x23A8 (Online: 0x2528)

    char UNK18[0xC];

    PadButtonStatus * Paddata;                                      // 0x23B4 (Online: 0x2534)

    char UNK17[0x166];

    // When Start is pressed, this counts down from 0xE, then shows menu when it equals 0
    char StartMenuTimer;                                            // 0x251A
    // If greater than 0, fade to black.
    char FadeToBlack;                                               // 0x251B
    char UNK11[0x4];
    char ForceHoldFlag;                                             // 0x251F

    VECTOR PlayerPosition2;                                         // 0x2538
    
} Player;

/*
 * NAME :		playerGetPad
 * 
 * DESCRIPTION :
 * 			Returns a pointer to the player's pad data.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          player:                     Target player.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ PadButtonStatus * playerGetPad(Player * player);

/*
 * NAME :		playerPadGetButton
 * 
 * DESCRIPTION :
 * 			Returns 1 when the given player is pressing the given button combination.
 *          Returns negative on failure.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          buttonMask:                 Buttons to check.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int playerPadGetButton(Player * player, u16 buttonMask);

/*
 * NAME :		playerPadGetButtonDown
 * 
 * DESCRIPTION :
 * 			Returns 1 during the frame that the given player starts pressing the given button combination.
 *          Returns negative on failure.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          buttonMask:                 Buttons to check.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int playerPadGetButtonDown(Player * player, u16 buttonMask);

/*
 * NAME :		playerPadGetButtonUp
 * 
 * DESCRIPTION :
 * 			Returns 1 during the frame that the given player releases the given button combination.
 *          Returns negative on failure.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          buttonMask:                 Buttons to check.
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
__LIBUYA_GETTER__ int playerPadGetButtonUp(Player * player, u16 buttonMask);

#endif // _LIBUYA_PLAYER_H_
