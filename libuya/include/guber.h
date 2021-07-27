/***************************************************
 * FILENAME :		guber.h
 * 
 * DESCRIPTION :
 * 		Contains guber specific offsets and structures for UYA.
 * 
 * NOTES :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GUBER_H_
#define _LIBUYA_GUBER_H_

#include <tamtypes.h>
#include "math3d.h"
#include "common.h"
#include "gid.h"

/*
 * NAME :		GuberDef
 * 
 * DESCRIPTION :
 * 			Contains the guber definition struct data.
 * 
 * NOTES :
 *          Mobies are objects that can be spawned in game.
 *          This includes things like vehicles, turrets, mod pads, etc
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */



typedef struct Guber 
{
    union
    {
        Gid GID;
        u32 UID;
    } Id;

    int MasterHostId;
    int State;
    struct Guber * Next;
    struct Guber * Prev;
    void * Vtable;

} Guber;

typedef struct GuberMoby
{
    Guber Guber;
    void * Moby;
    short ModeBits;
    u8 PADDING_0[2];
    void * UpdateFunc;
    int ClientUpdateTime;
    int TeamNum;
    int EnableAutoMigrateMaster;
    int LastMasterMigrateTime;
    int CurrentMaster;
    int NextMaster;
    int MasterTransferTime;
    int LastMasterMessageTime;
    int MasterHandler;
    int AssignedMaster;
    int LastAssignedTransferMaster;
    char LastMessageNum;
    char MessageNum;
    u8 PADDING_1[2];
} GuberMoby;

typedef struct GuberEvent
{
    u8 NetEvent[0x4C];
    int NetSendTime;
    int NetSendTo;
    u8 NetDataOffset;
    char MsgSendPending;
    u8 PADDING[2];
    void * NextEvent;
} GuberEvent;

#endif // _LIBUYA_GUBER_H_
