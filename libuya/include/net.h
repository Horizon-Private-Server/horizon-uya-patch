/***************************************************
 * FILENAME :		net.h
 * 
 * DESCRIPTION :
 * 		
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_NET_H_
#define _LIBUYA_NET_H_

#include <tamtypes.h>
#include "common.h"

#define NET_CUSTOM_MESSAGE_CLASS                    (0)
#define NET_CUSTOM_MESSAGE_ID                       (8)
#define NET_LOBBY_CLIENT_INDEX                      (0xFFFF)

typedef int (*NET_CALLBACK_DELEGATE)(void * connection, void * data);

void netInstallCustomMsgHook(int force);
void netInstallCustomMsgHandler(u8 id, NET_CALLBACK_DELEGATE callback);
int netSendMediusAppMessage(void * connection, int clientIndex, int msgClass, int msgId, int msgSize, void * payload);
int netSendCustomAppMessage(void * connection, int clientIndex, u8 customMsgId, int msgSize, void * payload);
int netBroadcastMediusAppMessage(void * connection, int msgId, int msgSize, void * payload);
int netBroadcastCustomAppMessage(void * connection, u8 customMsgId, int msgSize, void * payload);

void* netGetLobbyServerConnection(void);
void* netGetDmeServerConnection(void);

#endif // _LIBUYA_NET_H_
