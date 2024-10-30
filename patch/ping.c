#include <libuya/game.h>
#include <libuya/stdio.h>
#include <libuya/time.h>
#include <libuya/player.h>
#include <libuya/net.h>
#include <libuya/utils.h>
#include <libuya/gamesettings.h>
#include "messageid.h"

#define LATENCY_PING_COOLDOWN_TICKS (60 * 1)

typedef struct LatencyPing {
    int fromClientIdx;
    int toClientIdx;
    long ticks;
} LatencyPing_t;
int clientLatency[GAME_MAX_PLAYERS];
int myPing = 0;

enum GameNetMessage {
	CUSTOM_MSG_ID_PLAYER_LATENCY_TEST_PING = CUSTOM_MSG_ID_GAME_MODE_START,
};

int pingRemoteLatencyPing(void* connection, void* data)
{
    LatencyPing_t msg;
    memcpy(&msg, data, sizeof(msg));

    printf("\nfrom: %d, mine: %d", msg.fromClientIdx, gameGetMyClientId());

    if (msg.fromClientIdx == gameGetMyClientId()) {
        long dticks = timerGetSystemTime() - msg.ticks;
        int dms = dticks / SYSTEM_TIME_TICKS_PER_MS;
        clientLatency[msg.toClientIdx] = dms;
        myPing = dms;
        printf("\nRTT to CLIENT %d : %dms", msg.toClientIdx, dms);
    } else {
        // send back
        netSendCustomAppMessage(connection, msg.fromClientIdx, CUSTOM_MSG_ID_PLAYER_LATENCY_TEST_PING, sizeof(msg), &msg);
    }

    return sizeof(struct LatencyPing);
}

void pingSendLatencyPings(void)
{
    LatencyPing_t msg;
    msg.fromClientIdx = gameGetMyClientId();
    msg.ticks = timerGetSystemTime();

    // ensure we have a connection to the dme server
    void * connection = netGetDmeServerConnection();
    if (!connection)
        return;

    // send to each client
    int i;
    GameSettings* gs = gameGetSettings();
    if (gs) {
        for (i = 0; i < gs->PlayerCount; ++i) {
            if (gs->PlayerClients[i] >= 0)
                msg.toClientIdx = gameGetMyClientId();
                netSendCustomAppMessage(connection, msg.toClientIdx, CUSTOM_MSG_ID_PLAYER_LATENCY_TEST_PING, sizeof(msg), &msg);
                printf("\ngs: netSend toClient: %d", msg.toClientIdx);
            }
    } else {
        msg.toClientIdx = gameGetMyClientId();
        netSendCustomAppMessage(connection, msg.toClientIdx, CUSTOM_MSG_ID_PLAYER_LATENCY_TEST_PING, sizeof(msg), &msg);
        printf("\nnon-gs: netSend toClient: %d", msg.toClientIdx);
    }

}

void runPing(void)
{
    static int pingCooldown = 0;
    static int reset = 0;

    netInstallCustomMsgHandler(CUSTOM_MSG_ID_PLAYER_LATENCY_TEST_PING, &pingRemoteLatencyPing);

    // if (!isInGame()) {
    //     if (!reset) {
    //         memset(clientLatency, 0, sizeof(clientLatency));
    //         pingCooldown = 0;
    //         reset = 1;
    //         printf("\nping: Reset!");
    //     }
    //     return;
    // }

    if (!reset) {
        memset(clientLatency, 0, sizeof(clientLatency));
        pingCooldown = 0;
        reset = 1;
        printf("\nping: Reset!");    
    }

    // send pings
    if (pingCooldown) {
        --pingCooldown;
    } else {
        pingSendLatencyPings();
        printf("\nmyPing: %d", myPing);
        pingCooldown = LATENCY_PING_COOLDOWN_TICKS;
    }
    // reset = 0;
}
