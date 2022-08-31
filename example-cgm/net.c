#include <tamtypes.h>
#include <string.h>

#include <libuya/time.h>
#include <libuya/game.h>
#include <libuya/string.h>
#include <libuya/stdio.h>
#include <libuya/net.h>

#include "module.h"
#include "messageid.h"
#include "include/game.h"

//--------------------------------------------------------------------------
int onExampleMessageRemote(void * connection, void * data)
{
	ExampleMessage_t message;

	// copy data into message struct
	memcpy(&message, data, sizeof(ExampleMessage_t));

	// print out to console
	DPRINTF("recv example message %d:%d:%s\n", message.GameTime, message.SourcePlayerId, message.Message);

	return sizeof(ExampleMessage_t);
}

//--------------------------------------------------------------------------
void sendExampleMessage(char* message)
{
	ExampleMessage_t msg;

	// send out
	msg.GameTime = gameGetTime();
	msg.SourcePlayerId = 0; // todo add function to get local player
	strncpy(msg.Message, message, sizeof(msg.Message));
	netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_EXAMPLE, sizeof(ExampleMessage_t), &msg);
}

void netHookMessages(void)
{
	// Hook custom net events
	netInstallCustomMsgHandler(CUSTOM_MSG_EXAMPLE, &onExampleMessageRemote);
}
