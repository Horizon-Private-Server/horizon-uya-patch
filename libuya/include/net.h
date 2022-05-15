/***************************************************
 * FILENAME :		net.h
 * 
 * DESCRIPTION :
 * 		
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBDL_NET_H_
#define _LIBDL_NET_H_

#include <tamtypes.h>
#include "common.h"

#define NET_CUSTOM_MESSAGE_CLASS                    (0)
#define NET_CUSTOM_MESSAGE_ID                       (7)

// typedef int (*NET_CALLBACK_DELEGATE)(void * connection, void * data);


// void netInstallCustomMsgHandler(u8 id, NET_CALLBACK_DELEGATE callback);
// int netSendMediusAppMessage(void * connection, int msgClass, int msgId, int msgSize, void * payload);
// int netSendCustomAppMessage(void * connection, u8 customMsgId, int msgSize, void * payload);
// int netBroadcastMediusAppMessage(void * connection, int msgId, int msgSize, void * payload);
// int netBroadcastCustomAppMessage(void * connection, u8 customMsgId, int msgSize, void * payload);

// __LIBDL_GETTER__ void* netGetLobbyServerConnection(void);
// __LIBDL_GETTER__ void* netGetDmeServerConnection(void);

#endif // _LIBDL_NET_H_
