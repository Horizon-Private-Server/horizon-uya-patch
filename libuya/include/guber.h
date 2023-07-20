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
#include "moby.h"

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
    Moby * Moby;
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

typedef struct NetEvent
{
    struct {
        unsigned int EventID : 4;
        unsigned int NetDataSize : 6;
        unsigned int OriginClientIdx : 4;
        unsigned int RelDispatchTime : 18;
    };
	int CreateTime;
	unsigned int ObjUID;
	char NetData[64];
} NetEvent;

typedef struct GuberEvent
{
    NetEvent NetEvent;
    int NetSendTime;
    int NetSendTo;
    u8 NetDataOffset;
    char MsgSendPending;
    u8 PADDING[2];
    void * NextEvent;
} GuberEvent;

Guber * guberGetObjectByMoby(Moby* moby);
GuberEvent * guberEventCreate(Guber * guber, int eventId, int arg3, int arg4);

#endif // _LIBUYA_GUBER_H_
