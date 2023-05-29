#include <tamtypes.h>
#include "stdio.h"
#include "net.h"
#include "string.h"

#if UYA_PAL

#define DME_CALLBACK_TABLE                  ((u32*)0x002405e8)

#define NET_LOBBY_CONNECTION                ((void*)(*(u32*)0x001cd4cc))
#define NET_DME_CONNECTION                  ((void*)(*(u32*)0x001cd4d0))

#else

#define DME_CALLBACK_TABLE                  ((u32*)0x00240768)

#define NET_LOBBY_CONNECTION                ((void*)(*(u32*)0x001CD64C))
#define NET_DME_CONNECTION                  ((void*)(*(u32*)0x001CD650))

#endif

#define GET_MEDIUS_APP_HANDLER_HOOK         (*(u32*)0x00153248)
#define NET_GLOBAL_CALLBACKS_PTR            ((NET_CALLBACK_DELEGATE*)(*(u32*)0x000A0000))

int internal_netSendMessage(int transport, void * connection, long clientIndex, int msgClass, int msgId, int msgSize, void * payload);
int internal_netSendAppMessage(int transport, void * connection, long clientIndex, int msgId, int msgSize, void * payload);

NET_CALLBACK_DELEGATE callbacks[256] = {};

int customMsgHandler(void * connection, u64 a1, u64 a2, u8 * data)
{
    u8 id = data[0];


    NET_CALLBACK_DELEGATE callback = NET_GLOBAL_CALLBACKS_PTR[id];
    if (callback)
        return 4 + callback(connection, (void*)(data + 4));
    else
        printf("unhandled custom message id:%d\n", id);

    return 4;
}

int mediusMsgHandler(u64 a0, u64 a1, u32 * callback, u64 a3, u64 t0)
{
    if (a3 == NET_CUSTOM_MESSAGE_CLASS && t0 == NET_CUSTOM_MESSAGE_ID)
    {
        *callback = (u32)&customMsgHandler;
        return 0;
    }

    return a0 == 0;
}

void netInstallCustomMsgHook(int force)
{
    // set static global callbacks ptr to ours if empty
    if (force || NET_GLOBAL_CALLBACKS_PTR == 0)
        NET_GLOBAL_CALLBACKS_PTR = (NET_CALLBACK_DELEGATE*)callbacks;
    
    // only update handlers if we're the callback handler
    if (NET_GLOBAL_CALLBACKS_PTR == (NET_CALLBACK_DELEGATE*)callbacks)
    {
        GET_MEDIUS_APP_HANDLER_HOOK = 0x08000000 | ((u32)&mediusMsgHandler / 4);
        *(DME_CALLBACK_TABLE + NET_CUSTOM_MESSAGE_ID) = (u32)&customMsgHandler;
    }
}

void netInstallCustomMsgHandler(u8 id, NET_CALLBACK_DELEGATE callback)
{
    // install hook
    netInstallCustomMsgHook(0);

    // install callback
    NET_GLOBAL_CALLBACKS_PTR[id] = callback;
}

int netSendMediusAppMessage(void * connection, int clientIndex, int msgClass, int msgId, int msgSize, void * payload)
{
    if (!connection)
        return 0;
    return internal_netSendMessage(0x40, connection, clientIndex, msgClass, msgId, msgSize, payload);
}

int netBroadcastMediusAppMessage(void * connection, int msgId, int msgSize, void * payload)
{
    if (!connection)
        return 0;
    return internal_netSendAppMessage(0x40, connection, -1, msgId, msgSize, payload);
}

int netSendCustomAppMessage(void * connection, int clientIndex, u8 customMsgId, int msgSize, void * payload)
{
    if (!connection)
        return 0;
        
    u8 buffer[512];
    if (payload && msgSize > 0)
        memcpy(buffer + 4, payload, msgSize);
        
    buffer[0] = customMsgId;
    return netSendMediusAppMessage(connection, clientIndex, NET_CUSTOM_MESSAGE_CLASS, NET_CUSTOM_MESSAGE_ID, msgSize + 4, buffer);
}

int netBroadcastCustomAppMessage(void * connection, u8 customMsgId, int msgSize, void * payload)
{
    if (!connection)
        return 0;

    u8 buffer[512];
    if (payload && msgSize > 0)
        memcpy(buffer + 4, payload, msgSize);
        
    buffer[0] = customMsgId;
    return netBroadcastMediusAppMessage(connection, NET_CUSTOM_MESSAGE_ID, msgSize + 4, buffer);
}

void* netGetLobbyServerConnection(void)
{
    return NET_LOBBY_CONNECTION;
}

void* netGetDmeServerConnection(void)
{
    return NET_DME_CONNECTION;
}
