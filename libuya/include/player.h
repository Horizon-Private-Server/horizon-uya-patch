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

#include "math.h"
#include "pad.h"
#include "math3d.h"
#include "common.h"
#include "guber.h"

/*
 * Maximum health of player.
 */
#define PLAYER_MAX_HEALTH                   (50)

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
    struct Guber Guber;                                             // 0x00

    VECTOR PlayerPosition;                                          // 0xA0

    char UNK6[0x80];

    VECTOR PlayerVelocity;                                          // 0x120

    char UNK0[0x328];// Needs corrct offset

    float WeaponCooldownTimer;                                      // 0x328

    char UNK5[0x130];

    // is 1 is player is shooting
    int IsShooting;                                                 // 0x458

    char UNK3[0xD28];

    VECTOR CameraPos;                                               // 0x1180

    char UNK1[0x50];

    CameraAngle CameraYaw;                                          // 0x11E0
    CameraAngle CameraPitch;                                        // 0x1200

    char UNK4[0x54];

    float CameraPitchMin;                                           // 0x1254
    float CameraPitchMax;                                           // 0x1258

    // In libdl this is: Moby * SkinMoby;
    // Using int for now because moby.h is not impamented yet.
    int SkinMoby;                                                   // 0x125C

    float CameraDistance;                                           // 0x1260

    char UNK7[0x7A8];

    char HideWeapon;                                                // 0x1A08
    char HidePlayer;                                                // 0x1A09
    char UNK8[0x4];
    char WrenchOnly;                                                // 0x1A0D
    char UNK9[0x1];
    char SwingShotOnly;                                             // 0x1A10
    char ForceWrenchSwitch;                                         // 0x1A11
    char UNK10[0x4];
    char ShieldTrigger;                                             // 0x1A15


    char UNK5[0x12D8];

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
