/***************************************************
 * FILENAME :		guber.h
 * DESCRIPTION :
 * 		Contains guber specific offsets and structures for UYA.
 * NOTES :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_GUBER_H_
#define _LIBUYA_GUBER_H_

#include <tamtypes.h>
#include "math3d.h"
#include "common.h"
#include "gid.h"
#include "moby.h"

typedef Moby* (*GuberGetMoby_func)(struct Guber* guber);

struct GuberVTable
{
    void * FUNC_00;
    void * FUNC_04;
    void * FUNC_08;
    void * FUNC_0C;
    GuberGetMoby_func GetMoby;
    void * FUNC_14;
    void * FUNC_18;
    void * FUNC_1C;
    void * FUNC_20;
};

/*
 * NAME :		GuberDef
 * DESCRIPTION :
 * 			Contains the guber definition struct data.
 * NOTES :
 *          Mobies are objects that can be spawned in game.
 *          This includes things like vehicles, turrets, mod pads, etc
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */



typedef struct Guber 
{
    union
    {
        u32 UID;
        Gid GID;
    } Id;

/* 0x04 */ int MasterHostId;
/* 0x08 */ int State;
/* 0x0c */ struct Guber * Prev;
/* 0x10 */ struct Guber * Next;
/* 0x14 */ struct GuberVTable * Vtable;
} Guber;

typedef struct GuberMoby { // 0x54
/* 0x00 */ Guber Guber;
/* 0x18 */ Moby * Moby;
/* 0x1c */ short ModeBits;
/* 0x1e */ short pad_1e;
/* 0x20 */ void * Update;
/* 0x24 */ int ClientUpdateTime;
/* 0x28 */ int TeamNum;
/* 0x2c */ int EnableAutoMigrateMaster;
/* 0x30 */ int LastMasterMigrateTime;
/* 0x34 */ int CurrentMaster;
/* 0x38 */ int NextMaster;
/* 0x3c */ int MasterTransferTime;
/* 0x40 */ int LastMasterMessageTime;
/* 0x44 */ int MasterHandler;
/* 0x48 */ int AssignedMaster;
/* 0x4c */ int LastAssignedTransferMaster;
/* 0x50 */ char LastMessageNum;
/* 0x51 */ char MessageNum;
/* 0x52 */ short pad_52;
} GuberMoby;

typedef struct Master {
    u32 UID;
    struct Master* Prev;
    struct Master* Next;
    void* vtable;
    Moby* Moby;
    void* Update;
    int pad;
} Master;

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

__LIBUYA_GETTER__ GuberMoby * guberMobyGetFirst(void);
Guber * guberGetObjectByUID(u32 uid);
Guber * guberGetObjectByMoby(Moby* moby);
u32 guberGetUID(Moby* moby);
GuberEvent * guberEventCreate(Guber * guber, int eventId, int arg3, int arg4);

Master * masterGet(u32 uid);
Master * masterCreate(void * object, int sendCreateMsg);
Master * masterDelete(void * master);

#endif // _LIBUYA_GUBER_H_
