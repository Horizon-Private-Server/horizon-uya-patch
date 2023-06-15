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

#ifdef UYA_PAL
#define TNW_PLAYERDATA                      (*(u32*)0x0022f3d4)
#define PLAYER_STRUCT_POINTER               (*(u32*)0x00225cf0)
#else
#define TNW_PLAYERDATA                      (*(u32*)0x0022f554)
#define PLAYER_STRUCT_POINTER               (*(u32*)0x00225e70)
#endif

#define PLAYER_STRUCT                       (PLAYER_STRUCT_POINTER - 0x430C)

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
 * NAME :		PlayerState
 * 
 * DESCRIPTION :
 * 			Defines the player states.
 * 
 * NOTES :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
enum PlayerState {
	PLAYER_STATE_IDLE = 0,
	PLAYER_STATE_LOOK = 1,
	PLAYER_STATE_WALK = 2,
	PLAYER_STATE_SKID = 3,
	PLAYER_STATE_CROUCH = 4,
	PLAYER_STATE_QUICK_TURN = 5,
	PLAYER_STATE_FALL = 6,
	PLAYER_STATE_JUMP = 7,
	PLAYER_STATE_GLIDE = 8,
	PLAYER_STATE_RUN_JUMP = 9,
	PLAYER_STATE_LONG_JUMP = 10,
	PLAYER_STATE_FLIP_JUMP = 11,
	PLAYER_STATE_JINK_JUMP = 12,
	PLAYER_STATE_ROCKET_JUMP = 13,
	PLAYER_STATE_DOUBLE_JUMP = 14,
	PLAYER_STATE_HELI_JUMP = 15,
	PLAYER_STATE_CHARGE_JUMP = 16,
	PLAYER_STATE_WALL_JUMP = 17,
	PLAYER_STATE_WATER_JUMP = 18,
	PLAYER_STATE_COMBO_ATTACK = 19,
	PLAYER_STATE_JUMP_ATTACK = 20,
	PLAYER_STATE_THROW_ATTACK = 21,
	PLAYER_STATE_GET_HIT = 22,
	PLAYER_STATE_LEDGE_GRAB = 23,
	PLAYER_STATE_LEDGE_IDLE = 24,
	PLAYER_STATE_LEDGE_TRAVERSE_LEFT = 25,
	PLAYER_STATE_LEDGE_TRAVERSE_RIGHT = 26,
	PLAYER_STATE_LEDGE_JUMP = 27,
	PLAYER_STATE_VISIBOMB = 28,
	PLAYER_STATE_TARGETING = 29,
	PLAYER_STATE_GUN_WAITING = 30,
	PLAYER_STATE_WALLOPER_ATTACK = 31,
	PLAYER_STATE_ATTACK_BOUNCE = 32,
	PLAYER_STATE_ROCKET_STOMP = 33,
	PLAYER_STATE_GLOVE_ATTACK = 34,
	PLAYER_STATE_GRAPPLE_SHOOT = 35,
	PLAYER_STATE_GRAPPLE_PULL = 36,
	PLAYER_STATE_GRAPPLE_PULL_VEHICLE = 37,
	PLAYER_STATE_SUCK_CANNON = 38,
	PLAYER_STATE_GRIND = 39,
	PLAYER_STATE_GRIND_JUMP = 40,
	PLAYER_STATE_GRIND_SWITCH_JUMP = 41,
	PLAYER_STATE_GRIND_ATTACK = 42,
	PLAYER_STATE_SWING = 43,
	PLAYER_STATE_SWING_FALL = 44,
	PLAYER_STATE_RECOIL = 45,
	PLAYER_STATE_ICE_WALK = 46,
	PLAYER_STATE_DEVASTATOR = 47,
	PLAYER_STATE_SLIDE = 48,
	PLAYER_STATE_VEHICLE = 49,
	PLAYER_STATE_SWIMUNDER = 50,
	PLAYER_STATE_IDLEUNDER = 51,
	PLAYER_STATE_CHARGEUNDER = 52,
	PLAYER_STATE_SWIMSURF = 53,
	PLAYER_STATE_IDLESURF = 54,
	PLAYER_STATE_BOLT_CRANK = 55,
	PLAYER_STATE_LAVA_JUMP = 56,
	PLAYER_STATE_DEATH = 57,
	PLAYER_STATE_BOARD = 58,
	PLAYER_STATE_MAGNE_WALK = 59,
	PLAYER_STATE_GRIND_HIT = 62,
	PLAYER_STATE_GRIND_JUMP_TURN = 63,
	PLAYER_STATE_VENDOR_BOOTH = 98,
	PLAYER_STATE_NPC = 99,
	PLAYER_STATE_WALK_TO_POS = 100,
	PLAYER_STATE_SKID_TO_POS = 101,
	PLAYER_STATE_IDLE_TO_POS = 102,
	PLAYER_STATE_JUMP_TO_POS = 103,
	PLAYER_STATE_QUICKSAND_SINK = 104,
	PLAYER_STATE_QUICKSAND_JUMP = 105,
	PLAYER_STATE_DROWN = 106,
	PLAYER_STATE_MAGNE_ATTACK = 111,
	PLAYER_STATE_MAGNE_JUMP = 112,
	PLAYER_STATE_CUT_SCENE = 113,
	PLAYER_STATE_WADE = 114,
	PLAYER_STATE_ZIP = 115,
	PLAYER_STATE_GET_HIT_SURF = 116,
	PLAYER_STATE_GET_HIT_UNDER = 117,
	PLAYER_STATE_DEATH_FALL = 118,
	PLAYER_STATE_SLOPESLIDE = 120,
	PLAYER_STATE_JUMP_BOUNCE = 121,
	PLAYER_STATE_DEATHSAND_SINK = 122,
	PLAYER_STATE_LAVA_DEATH = 123,
	PLAYER_STATE_CHARGE = 125,
	PLAYER_STATE_ICEWATER_FREEZE = 126,
	PLAYER_STATE_ELECTRIC_DEATH = 127,
	PLAYER_STATE_ROCKET_HOVER = 128,
	PLAYER_STATE_ELECTRIC_DEATH_UNDER = 129,
	PLAYER_STATE_SKATE = 130,
	PLAYER_STATE_MOON_JUMP = 131,
	PLAYER_STATE_JET = 132,
	PLAYER_STATE_THROW_SHURIKEN = 133,
	PLAYER_STATE_RACEBIKE = 134,
	PLAYER_STATE_SPEEDBOAT = 135,
	PLAYER_STATE_HOVERPLANE = 136,
	PLAYER_STATE_LATCH_GRAB = 137,
	PLAYER_STATE_LATCH_IDLE = 138,
	PLAYER_STATE_LATCH_JUMP = 139,
	PLAYER_STATE_PULLSHOT_ATTACH = 140,
	PLAYER_STATE_PULLSHOT_PULL = 141,
	PLAYER_STATE_GET_FLATTENED = 142,
	PLAYER_STATE_SKYDIVE = 143,
	PLAYER_STATE_ELECTRIC_GET_HIT = 144,
	PLAYER_STATE_FLAIL_ATTACK = 145,
	PLAYER_STATE_MAGIC_TELEPORT = 146,
	PLAYER_STATE_TELEPORT_IN = 147,
	PLAYER_STATE_DEATH_NO_FALL = 148,
	PLAYER_STATE_TAUNT_SQUAT = 149,
	PLAYER_STATE_TAUNT_ASSPOINT = 150,
	PLAYER_STATE_TAUNT_ASSRUB = 151,
	PLAYER_STATE_TURRET_DRIVER = 152,
	PLAYER_STATE_WAIT_FOR_RESURRECT = 153,
	PLAYER_STATE_WAIT_FOR_JOIN = 154,
	PLAYER_STATE_DROPPED = 155,
	PLAYER_STATE_CNT = 156
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

/*
 * NAME :		playerSetPlayerState
 * 
 * DESCRIPTION :
 * 			Set Player to a certain state.
 * 
 * NOTES :
 * 
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          state:                 		State to set player.
 * 
 * RETURN :
 * 
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */
void playerSetPlayerState(Player * player, u8 state);

/*
 * NAME :		playerDecHealth
 * DESCRIPTION :
 * 			Decrease a players health by a certain amount (1 to 15)
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          state:                 		Amount to hurt player (1 to 15)
 * RETURN :
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */
void playerDecHealth(Player * player, u8 amount);

/*
 * NAME :		playerIncHealth
 * DESCRIPTION :
 * 			Increase a players health by a certain amount (1 to 15)
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          state:                 		Amount to give to player (1 to 15)
 * RETURN :
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */
void playerIncHealth(Player * player, u8 amount);

/*
 * NAME :		playerSetHealth
 * DESCRIPTION :
 * 			Set a players health to a certain amount (1 to 15)
 * NOTES :
 * ARGS : 
 *          player:                     Pointer to player's player object.
 *          state:                 		Value to set health to (1 to 15)
 * RETURN :
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */
void playerSetHealth(Player * player, u8 amount);



#endif // _LIBUYA_PLAYER_H_
