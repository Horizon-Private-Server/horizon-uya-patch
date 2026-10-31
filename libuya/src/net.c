#include <tamtypes.h>
#include "stdio.h"
#include "net.h"
#include "string.h"
#include "game.h"
#include "gamesettings.h"

#if UYA_PAL

#define DME_CALLBACK_TABLE                  ((u32*)0x002405e8)

#define NET_LOBBY_CONNECTION                ((void*)(*(u32*)0x001cd4cc))
#define NET_DME_CONNECTION                  ((void*)(*(u32*)0x001cd4d0))
#define NET_HEARTBEAT_1 (0x001a1bb0)
#define NET_HEARTBEAT_2 (0x001a1bb4)
#else

#define DME_CALLBACK_TABLE                  ((u32*)0x00240768)

#define NET_LOBBY_CONNECTION                ((void*)(*(u32*)0x001CD64C))
#define NET_DME_CONNECTION                  ((void*)(*(u32*)0x001CD650))
#define NET_HEARTBEAT_1 (0x001a1d30)
#define NET_HEARTBEAT_2 (0x001a1d34)
#endif

#define GET_MEDIUS_APP_HANDLER_HOOK         (*(u32*)0x00153248)
#define NET_GLOBAL_CALLBACKS_PTR            (*(NET_CALLBACK_DELEGATE **)NET_GLOBAL_CALLBACKS_ADDRESS)

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
	buffer[1] = buffer[2] = buffer[3] = 0;

	GameSettings* settings = gameGetSettings();
	if (connection == NET_DME_CONNECTION && settings && !GAME_NET_INFO->onlyLocalPlayers) {
		/* The native broadcast route can miss clients added after loading.
		 * Use the current roster and send once per client, including split
		 * screen players who share a connection. Keep the native app class. */
		u32 sent = 0;
		int result = 0;
		int i;
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			int client = settings->PlayerClients[i];
			if (client < 0 || client >= GAME_MAX_PLAYERS || client == GAME_CLIENT_ID ||
			    (sent & (1 << client)))
				continue;
			sent |= 1 << client;
			int status = internal_netSendAppMessage(0x40, connection, client,
			    NET_CUSTOM_MESSAGE_ID, msgSize + 4, buffer);
			if (status && !result)
				result = status;
		}
		return result;
	}

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
