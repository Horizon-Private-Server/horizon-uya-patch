#include <tamtypes.h>
#include <stdio.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/player.h>
#include <libuya/pad.h>
#include <libuya/string.h>
#include <libuya/ui.h>
#include <libuya/interop.h>
#include <libuya/net.h>
#include <libuya/time.h>
#include <libuya/moby.h>
#include <libuya/guber.h>
#include "config.h"
#include "messageid.h"
#include "include/config.h"

#define JOIN_GATE_ORIGINAL (0x28420007)
#define JOIN_GATE_OPEN (0x24020001)
#define JOIN_SETUP_GATE_OPEN (0x01001025)
#define JOIN_PLAYER_STRIDE (0xc70)

#if UYA_PAL
#define JOIN_ADMISSION_GATE (0x0019a750)
#define JOIN_SETUP_REQUEST_GATE (0x0019a564)
#define JOIN_SETUP_STATE_GATE (0x0019a5e0)
#define JOIN_PLAYER_ARRAY (0x0022f3d0)
#define JOIN_COMPLETE_PLAYER (0x00199878)
#define JOIN_INPUT_CALLBACK (0x0024060c)
#define JOIN_STATE_CALLBACK (0x002405fc)
#define JOIN_DECODE_INPUT (0x0066f260)
#else
#define JOIN_ADMISSION_GATE (0x0019a890)
#define JOIN_SETUP_REQUEST_GATE (0x0019a6a4)
#define JOIN_SETUP_STATE_GATE (0x0019a720)
#define JOIN_PLAYER_ARRAY (0x0022f550)
#define JOIN_COMPLETE_PLAYER (0x001999b8)
#define JOIN_INPUT_CALLBACK (0x0024078c)
#define JOIN_STATE_CALLBACK (0x0024077c)
#define JOIN_DECODE_INPUT (0x0066c910)
#endif

typedef struct joinSiegeRequest {
	/* 0x00 */ u8 client;
	/* 0x01 */ u8 level;
	/* 0x02 */ u16 sequence;
} joinSiegeRequest_t;

typedef struct joinSiegeObject {
	/* 0x00 */ joinSiegeRequest_t request;
	/* 0x04 */ u16 uid;
	/* 0x06 */ u16 oClass;
	/* 0x08 */ u8 state;
	/* 0x09 */ s8 team;
	/* 0x0a */ u16 reserved;
	/* 0x0c */ float health;
} joinSiegeObject_t;

Player** playerGetAll(void);
void FlushCache(int operation);
static void joinSetLoadingReceivers(int enabled);

#if !UYA_PAL
static void joinSetPlayerUpdates(int enabled);
#endif

extern PatchGameConfig_t gameConfig;
extern int mapOverrideResponse;
extern int expectedMapVersion;

static const u32 joinGates[] = {
	JOIN_ADMISSION_GATE,
	JOIN_SETUP_REQUEST_GATE,
	JOIN_SETUP_STATE_GATE
};
/* t0 already contains message->state < 7 at the setup-state gate. Profile
 * setup may proceed late, but in-game state packets must not copy names. */
static const u32 joinGateValues[] = {
	JOIN_GATE_OPEN,
	JOIN_GATE_OPEN,
	JOIN_SETUP_GATE_OPEN
};
static int joinAdmissionOpen;
static int joinRosterInitialized;
static int joinSeenAccounts[GAME_MAX_PLAYERS];
static int joinLastTimeSent;
static int joinNeedsSiegeState;
static int joinSiegeRetry;
static int joinLastSiegeRequest;
static u16 joinSiegeSequence;
static u16 joinSiegeSequences[GAME_MAX_PLAYERS];
static u32 joinSiegeRequests;

static VariableAddress_t vaJoinPlayerLeave = {
#if UYA_PAL
	.Bakisi = 0x00538f48,
	.Hoven = 0x0053b060,
	.OutpostX12 = 0x00530938,
	.KorgonOutpost = 0x0052e0d0,
	.Metropolis = 0x0052d420,
	.BlackwaterCity = 0x0052acb8,
	.CommandCenter = 0x0052aa78,
	.BlackwaterDocks = 0x0052d2f8,
	.AquatosSewers = 0x0052c5f8,
	.MarcadiaPalace = 0x0052bf78,
#else
	.Bakisi = 0x005366c8,
	.Hoven = 0x00538720,
	.OutpostX12 = 0x0052e038,
	.KorgonOutpost = 0x0052b850,
	.Metropolis = 0x0052aba0,
	.BlackwaterCity = 0x005283b8,
	.CommandCenter = 0x00528338,
	.BlackwaterDocks = 0x0052ab78,
	.AquatosSewers = 0x00529eb8,
	.MarcadiaPalace = 0x005297f8,
#endif
};

#if !UYA_PAL
static int joinClientLoading;
static int joinNeedsGameTime;
static char joinSelectedMap[32];
static int joinPlayerObjectIds[GAME_MAX_PLAYERS];
static u32 joinRememberedPlayers;

/*
 * NAME : joinSetSelectedMap
 * DESCRIPTION : Remember the advertised map for the pre-join installation check.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
void joinSetSelectedMap(const char* name)
{
	joinSelectedMap[0] = 0;
	if (name) {
		strncpy(joinSelectedMap, name, sizeof(joinSelectedMap) - 1);
		joinSelectedMap[sizeof(joinSelectedMap) - 1] = 0;
	}
}

/*
 * NAME : joinBeginClient
 * DESCRIPTION : Start joining and enable the temporary loading receivers.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinBeginClient(void* context)
{
	if (joinSelectedMap[0]) {
		int i;
		int installed = 0;
		for (i = 0; customMapDefs && i < customMapDefCount; ++i) {
			if (strncmp(customMapDefs[i].Name, joinSelectedMap, strlen(joinSelectedMap)) == 0) {
				installed = 1;
				break;
			}
		}
		if (!installed) {
			uiShowOkDialog("Custom Map", "Install this custom map before joining the game.");
			return 0;
		}
	}

	/* Initial player objects can arrive inside this native wait loop,
	 * before staging resets the player records. Remember their IDs now. */
	joinRememberedPlayers = 0;
	joinSetLoadingReceivers(1);
	joinSetPlayerUpdates(1);
	int result = ((int (*)(void*))0x006d25c0)(context);
	joinClientLoading = result != 0;
	joinNeedsGameTime = joinClientLoading;
	joinNeedsSiegeState = joinClientLoading;
	joinSiegeRetry = 0;
	joinLastSiegeRequest = gameGetTime();
	++joinSiegeSequence;
	joinSetLoadingReceivers(joinClientLoading);
	joinSetPlayerUpdates(joinClientLoading);
	return result;
}

/*
 * NAME : joinEndClient
 * DESCRIPTION : Clear temporary join state through the native cleanup path.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinEndClient(void* context)
{
	joinClientLoading = 0;
	joinNeedsGameTime = 0;
	joinNeedsSiegeState = 0;
	joinRememberedPlayers = 0;
	joinSetLoadingReceivers(0);
	joinSetPlayerUpdates(0);
	return ((int (*)(void*))0x006d1928)(context);
}

/*
 * NAME : joinLoadClient
 * DESCRIPTION : Supply the local gameplay pad before starting the level load.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinLoadClient(int level, int mode)
{
	if (joinClientLoading && GAME_NET_INFO->numLocalPlayers == 1 &&
	    !PAD_POINTER[3] && PAD_POINTER[0])
		PAD_POINTER[3] = PAD_POINTER[0];
	((void (*)(int, int))0x005992a8)(level, mode);
}

/*
 * NAME : joinPlayerObjectCreated
 * DESCRIPTION : Remember initial player objects across staging's player-record reset.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinPlayerObjectCreated(int connection, int objectId)
{
	((void (*)(int, int))0x00199ba0)(connection, objectId);
	void* object = 0;
	if (((int (*)(void**, int))0x0016ed00)(&object, objectId) || !object)
		return;
	struct tNW_PlayerData* data = *(struct tNW_PlayerData**)((u8*)object + 0x1c);
	if (data && data->playerIndex >= 0 && data->playerIndex < GAME_MAX_PLAYERS) {
		joinPlayerObjectIds[data->playerIndex] = objectId;
		joinRememberedPlayers |= 1 << data->playerIndex;
	}
}

/*
 * NAME : joinPlayerObjectUpdated
 * DESCRIPTION : Bind an existing network player object before applying its update.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinPlayerObjectUpdated(int connection, int objectId, int fields)
{
	void* object = 0;
	if (((int (*)(void**, int))0x0016ed00)(&object, objectId) || !object)
		return;
	struct tNW_PlayerData* data = *(struct tNW_PlayerData**)((u8*)object + 0x1c);
	if (!data || data->playerIndex < 0 || data->playerIndex >= GAME_MAX_PLAYERS)
		return;
	joinPlayerObjectIds[data->playerIndex] = objectId;
	joinRememberedPlayers |= 1 << data->playerIndex;
	struct tNW_Player* player = (struct tNW_Player*)(JOIN_PLAYER_ARRAY + data->playerIndex * JOIN_PLAYER_STRIDE);
	if (!player->pNetPlayerData)
		((void (*)(int, int))0x00199ba0)(connection, objectId);
	if (player->pNetPlayerData == data)
		((void (*)(int, int, int))0x00199c40)(connection, objectId, fields);
}

/*
 * NAME : joinSetPlayerUpdates
 * DESCRIPTION : Select the safe loading receiver or the native player receiver.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinSetPlayerUpdates(int enabled)
{
	int type = *(int*)0x002407bc;
	((int (*)(int, void*, void*, void*))0x0016eeb0)
	    (type, enabled ? (void*)&joinPlayerObjectCreated : (void*)0x00199ba0,
	    enabled ? (void*)&joinPlayerObjectUpdated : (void*)0x00199c40,
	    (void*)0x00199cb8);
}
#endif

/*
 * NAME : joinSupportsMode
 * DESCRIPTION : Check the late-join whitelist, including a map-forced mode.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinSupportsMode(void)
{
	int mode = gameConfig.customModeId;
	int index = patchStateContainer.customMapId - 1;
	if (gameConfig.isCustomMap && customMapDefs && index >= 0 && index < customMapDefCount &&
	    customMapDefs[index].ForcedCustomModeId)
		mode = customMapDefs[index].ForcedCustomModeId;
	return mode == CUSTOM_MODE_NONE || mode == CUSTOM_MODE_KOTH;
}

/*
 * NAME : joinSupportsMap
 * DESCRIPTION : Require an installed custom map matching the host version.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinSupportsMap(void)
{
	int index = patchStateContainer.customMapId - 1;
	if (!gameConfig.isCustomMap)
		return index < 0 && !MapLoaderState.Enabled;
	return customMapDefs && index >= 0 && index < customMapDefCount &&
	    MapLoaderState.Enabled &&
	    MapLoaderState.MapFileName[0] && expectedMapVersion >= 0 &&
	    mapOverrideResponse == expectedMapVersion;
}

/*
 * NAME : joinReceiveGameTime
 * DESCRIPTION : Match the host timer without changing the local start timestamp.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinReceiveGameTime(void* connection, void* data)
{
#if !UYA_PAL
	if (connection == netGetDmeServerConnection() && joinNeedsGameTime &&
	    isInGame() && !gameAmIHost()) {
		GameData* game = gameGetData();
		int remaining;
		memcpy(&remaining, data, sizeof(remaining));
		if (game && game->timeStart > 0 && remaining >= -1) {
			/* The native HUD uses -1 for an untimed game; zero is expired. */
			game->timeEnd = remaining < 0 ? -1 : gameGetTime() - game->timeStart + remaining;
			joinNeedsGameTime = 0;
		}
	}
#endif
	return sizeof(int);
}

/*
 * NAME : joinSendGameTime
 * DESCRIPTION : Send the remaining game time to clients in the current roster.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinSendGameTime(void)
{
	GameData* game = gameGetData();
	int now = gameGetTime();
	if (!game || game->timeStart <= 0 || now - joinLastTimeSent < TIME_SECOND)
		return;
	int remaining = -1;
	if (game->timeEnd >= 0) {
		remaining = game->timeEnd - (now - game->timeStart);
		if (remaining < 0)
			remaining = 0;
	}
	netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_JOIN_GAME_TIME,
	    sizeof(remaining), &remaining);
	joinLastTimeSent = now;
}

/*
 * NAME : joinReceiveSiegeRequest
 * DESCRIPTION : Queue a current-roster client's Siege catch-up request.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinReceiveSiegeRequest(void* connection, void* data)
{
	joinSiegeRequest_t* request = data;
	GameSettings* settings = gameGetSettings();
	if (connection != netGetDmeServerConnection() || !settings || !isInGame() ||
	    !gameAmIHost() || settings->GameType != GAMETYPE_SIEGE ||
	    request->level != settings->GameLevel || request->client >= GAME_MAX_PLAYERS ||
	    request->client == GAME_CLIENT_ID)
		return sizeof(*request);
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (settings->PlayerClients[i] == request->client && settings->PlayerAccountIds[i] >= 0) {
			joinSiegeSequences[request->client] = request->sequence;
			joinSiegeRequests |= 1 << request->client;
			break;
		}
	}
	return sizeof(*request);
}

/*
 * NAME : joinApplySiegeObject
 * DESCRIPTION : Restore an authored objective through its native event handler.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinApplySiegeObject(joinSiegeObject_t* object)
{
	Moby* moby;
	Moby* end = mobyListGetEnd();
	for (moby = mobyListGetStart(); moby && moby < end; ++moby) {
		if ((u16)moby->UID == object->uid && (u16)moby->oClass == object->oClass)
			break;
	}
	if (!moby || moby == end || !moby->pVar || moby->state == 0)
		return 0;

	u8* vars = moby->pVar;
	if (object->oClass == MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_CENTER) {
		/* Bridges are local controllers, not gubers. Schedule their normal
		 * break path so collision and linked bridge pieces update together. */
		if (object->state == 2 && moby->state == 1) {
			*(int*)vars = gameGetTime() - 1;
			*(int*)(vars + 0x10) = 0;
		}
		return 1;
	}

	MobyFunctions* functions = mobyGetFunctions(moby);
	if (!functions || !functions->FUNC_1C)
		return 0;
	GuberEvent event;
	memset(&event, 0, sizeof(event));
	event.NetEvent.CreateTime = gameGetTime();
	event.NetEvent.OriginClientIdx = gameGetHostId();
	if (moby->guberMoby)
		event.NetEvent.ObjUID = moby->guberMoby->Guber.Id.UID;

	if (object->oClass == MOBY_ID_SIEGE_NODE) {
		GameData* game = gameGetData();
		int node = *(int*)(vars + 0x2c);
		if (!game || !game->allYourBaseGameData || node < 0 || node >= 8)
			return 0;
		if (object->team < 0 || object->team > 1 ||
		    game->allYourBaseGameData->nodeTeam[node] == object->team)
			return 1;
		int capture[2] = { object->team, -1 };
		event.NetEvent.NetDataSize = sizeof(capture);
		memcpy(event.NetEvent.NetData, capture, sizeof(capture));
	} else if (object->oClass == MOBY_ID_GATLING_TURRET || object->oClass == MOBY_ID_BASE_COMPUTER) {
		int turret = object->oClass == MOBY_ID_GATLING_TURRET;
		int destroyed = turret ? 5 : 2;
		if (moby->state == destroyed)
			return 1;
		if (object->state == destroyed || object->health <= 0) {
			event.NetEvent.EventID = turret ? 1 : 0;
			event.NetEvent.NetDataSize = 8;
		} else {
			float damage = *(float*)(vars + 0x30) - object->health;
			if (damage <= 0)
				return 1;
			event.NetEvent.EventID = turret ? 2 : 1;
			event.NetEvent.NetDataSize = 12;
			memcpy(event.NetEvent.NetData, &damage, sizeof(damage));
		}
	} else {
		return 1;
	}
	((void (*)(Moby*, GuberEvent*))functions->FUNC_1C)(moby, &event);
	return 1;
}

/*
 * NAME : joinReceiveSiegeState
 * DESCRIPTION : Apply only this join attempt's objective snapshot.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinReceiveSiegeState(void* connection, void* data)
{
	joinSiegeObject_t* object = data;
	GameSettings* settings = gameGetSettings();
	if (connection != netGetDmeServerConnection() || !settings || !isInGame() ||
	    gameAmIHost() || !joinNeedsSiegeState || settings->GameType != GAMETYPE_SIEGE ||
	    object->request.client != GAME_CLIENT_ID || object->request.level != settings->GameLevel ||
	    object->request.sequence != joinSiegeSequence)
		return sizeof(*object);
	if (!object->oClass) {
		joinNeedsSiegeState = joinSiegeRetry;
		joinSiegeRetry = 0;
	} else if (!joinApplySiegeObject(object)) {
		joinSiegeRetry = 1;
	}
	return sizeof(*object);
}

/*
 * NAME : joinSendSiegeState
 * DESCRIPTION : Send current node, base-defense and bridge state to one newcomer.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinSendSiegeState(int client)
{
	GameSettings* settings = gameGetSettings();
	GameData* game = gameGetData();
	if (!settings || !game || !game->allYourBaseGameData)
		return;
	joinSiegeObject_t object;
	memset(&object, 0, sizeof(object));
	object.request.client = client;
	object.request.level = settings->GameLevel;
	object.request.sequence = joinSiegeSequences[client];
	Moby* moby;
	Moby* end = mobyListGetEnd();
	for (moby = mobyListGetStart(); moby && moby < end; ++moby) {
		if (!moby->pVar || (u8)moby->state >= 0x80 || (u16)moby->UID == 0xffff)
			continue;
		object.team = -1;
		object.health = 0;
		if (moby->oClass == MOBY_ID_SIEGE_NODE) {
			int node = *(int*)((u8*)moby->pVar + 0x2c);
			if (node < 0 || node >= 8)
				continue;
			object.team = game->allYourBaseGameData->nodeTeam[node];
		} else if (moby->oClass == MOBY_ID_GATLING_TURRET || moby->oClass == MOBY_ID_BASE_COMPUTER) {
			object.health = *(float*)((u8*)moby->pVar + 0x30);
		} else if (moby->oClass != MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_CENTER) {
			continue;
		}
		object.uid = moby->UID;
		object.oClass = moby->oClass;
		object.state = moby->state;
		netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_JOIN_SIEGE_STATE,
		    sizeof(object), &object);
	}
	object.oClass = 0;
	netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_JOIN_SIEGE_STATE,
	    sizeof(object), &object);
}

/*
 * NAME : joinRequestSiegeState
 * DESCRIPTION : Retry catch-up after loading until all objective objects exist.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinRequestSiegeState(GameSettings* settings)
{
	if (!joinNeedsSiegeState || gameAmIHost() || settings->GameType != GAMETYPE_SIEGE ||
	    gameGetTime() - joinLastSiegeRequest < TIME_SECOND)
		return;
	joinSiegeRequest_t request = { GAME_CLIENT_ID, settings->GameLevel, joinSiegeSequence };
	joinSiegeRetry = 0;
	netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_JOIN_SIEGE_REQUEST,
	    sizeof(request), &request);
	joinLastSiegeRequest = gameGetTime();
}

/*
 * NAME : joinReceiveLoadingInput
 * DESCRIPTION : Consume compressed input before the gameplay receiver is available.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinReceiveLoadingInput(void* connection, int world, int source, void* data)
{
	u32 decoded[0x98 / sizeof(u32)];
	u32 flags;
	memcpy(&flags, (u8*)data + 0x14, sizeof(flags));
	memset(decoded, 0, sizeof(decoded));
	memcpy(decoded, (u8*)data + 0x18, 0x30);
	int size = ((int (*)(u32, void*))JOIN_DECODE_INPUT)(flags >> 28, decoded);
	return 0x18 + size + ((flags & 0x01000000) ? 0x11 : 0);
}

/*
 * NAME : joinReceiveLoadingState
 * DESCRIPTION : Consume native game-state packets until gameplay installs its receiver.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinReceiveLoadingState(void* connection, int world, int source, void* data)
{
	/* The native receiver always consumes 0x68 bytes. Its game-controller
	 * data does not exist yet; the loaded game handles subsequent updates. */
	return 0x68;
}

/*
 * NAME : joinSetLoadingReceivers
 * DESCRIPTION : Fill empty loading callbacks and remove only our own receivers.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinSetLoadingReceivers(int enabled)
{
	volatile u32* callback = (volatile u32*)JOIN_INPUT_CALLBACK;
	if (enabled && !*callback)
		*callback = (u32)&joinReceiveLoadingInput;
	else if (!enabled && *callback == (u32)&joinReceiveLoadingInput)
		*callback = 0;

	callback = (volatile u32*)JOIN_STATE_CALLBACK;
	if (enabled && !*callback)
		*callback = (u32)&joinReceiveLoadingState;
	else if (!enabled && *callback == (u32)&joinReceiveLoadingState)
		*callback = 0;
}

/*
 * NAME : joinSetAdmission
 * DESCRIPTION : Toggle host admission gates while preserving native slot and password checks.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinSetAdmission(int enabled)
{
	int i;
	if (enabled == joinAdmissionOpen)
		return;

	for (i = 0; i < 3; ++i) {
		u32 expected = joinAdmissionOpen ? joinGateValues[i] : JOIN_GATE_ORIGINAL;
		if (*(volatile u32*)joinGates[i] != expected)
			return;
	}
	for (i = 0; i < 3; ++i)
		*(volatile u32*)joinGates[i] = enabled ? joinGateValues[i] : JOIN_GATE_ORIGINAL;

	FlushCache(0);
	FlushCache(2);
	joinAdmissionOpen = enabled;
}

/*
 * NAME : joinCompletePlayers
 * DESCRIPTION : Finish native player creation when a late human joins.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinCompletePlayers(GameSettings* settings)
{
	Player** players = playerGetAll();
	int i;
	if (!players)
		return;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		int client = settings->PlayerClients[i];
		if (players[i] || client < 0 || client == GAME_CLIENT_ID || settings->PlayerAccountIds[i] < 0)
			continue;

		struct tNW_Player* netPlayer = (struct tNW_Player*)(JOIN_PLAYER_ARRAY + i * JOIN_PLAYER_STRIDE);
		struct tNW_PlayerData* data = netPlayer->pNetPlayerData;
#if !UYA_PAL
		if (!data && (joinRememberedPlayers & (1 << i))) {
			void* object = 0;
			int objectId = joinPlayerObjectIds[i];
			if (((int (*)(void**, int))0x0016ed00)(&object, objectId) || !object)
				continue;
			data = *(struct tNW_PlayerData**)((u8*)object + 0x1c);
			if (!data || data->playerIndex != i || data->accountId != settings->PlayerAccountIds[i] ||
			    *(int*)((u8*)object + 8) != client)
				continue;
			((void (*)(int, int))0x00199ba0)(client, objectId);
			data = netPlayer->pNetPlayerData;
		}
#endif
		if (!data || data->playerIndex != i || data->accountId != settings->PlayerAccountIds[i] ||
		    netPlayer->netClientIndex != client || !netPlayer->bSpawned ||
		    netPlayer->bCallbackCalled || netPlayer->bUnk_1a4)
			continue;

		((void (*)(struct tNW_Player*))JOIN_COMPLETE_PLAYER)(netPlayer);
	}
}

/*
 * NAME : joinNotifyPlayer
 * DESCRIPTION : Add a named join message to the native notification feed.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinNotifyPlayer(const char* name)
{
	u32* leave = (u32*)GetAddress(&vaJoinPlayerLeave);
	if (!leave || (leave[0x88 / 4] & 0xfc000000) != 0x0c000000)
		return 0;
	u32* feed = (u32*)((leave[0x88 / 4] & 0x03ffffff) << 2);
	if (feed[0] != 0x27bdffe0 || feed[1] != 0x24030030 ||
	    (feed[3] & 0xffff0000) != 0x3c020000 || feed[4] != 0x3c100025 ||
	    (feed[5] & 0xffff0000) != 0x8e100000 || (feed[6] & 0xffff0000) != 0x24420000 ||
	    feed[20] != 0x3c040025 || (feed[21] & 0xffff0000) != 0x24840000)
		return 0;
	int* count = (int*)((feed[4] << 16) + (s16)feed[5]);
	int* timers = (int*)((feed[20] << 16) + (s16)feed[21]);
	char* messages = (char*)((feed[3] << 16) + (s16)feed[6]);
	if ((u32)count - (u32)timers != 8 * sizeof(int) || *count < 0 || *count > 8)
		return 0;
	if (*count == 8) {
		int i;
		for (i = 0; i < 7; ++i) {
			memcpy(messages + i * 48, messages + (i + 1) * 48, 48);
			timers[i] = timers[i + 1];
		}
		*count = 7;
	}
	snprintf(messages + *count * 48, 48, "%.15s joined the game.", name);
	timers[*count] = 0x708;
	++*count;
	return 1;
}

/*
 * NAME : joinNotifyPlayers
 * DESCRIPTION : Announce new humans without repeating the initial roster or bots.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinNotifyPlayers(GameSettings* settings)
{
	Player** players = playerGetAll();
	int i;
	if (!joinRosterInitialized) {
		for (i = 0; i < GAME_MAX_PLAYERS; ++i)
			joinSeenAccounts[i] = settings->PlayerClients[i] < 0 ? -1 : settings->PlayerAccountIds[i];
		joinRosterInitialized = 1;
		return;
	}
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		int account = settings->PlayerAccountIds[i];
		if (settings->PlayerClients[i] < 0 || account < 0) {
			joinSeenAccounts[i] = -1;
			continue;
		}
		if (joinSeenAccounts[i] == account || !players || !players[i] ||
		    settings->PlayerStates[i] < 7 || !settings->PlayerNames[i][0])
			continue;
		if (joinNotifyPlayer(settings->PlayerNames[i]))
			joinSeenAccounts[i] = account;
	}
}

/*
 * NAME : joinTick
 * DESCRIPTION : Maintain join hooks, admission, player creation and timer synchronization.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
void joinTick(void)
{
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_JOIN_GAME_TIME, &joinReceiveGameTime);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_JOIN_SIEGE_REQUEST, &joinReceiveSiegeRequest);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_JOIN_SIEGE_STATE, &joinReceiveSiegeState);
	if (isInGame())
		joinSetLoadingReceivers(0);
#if !UYA_PAL
	if (joinClientLoading && isInGame()) {
		joinClientLoading = 0;
		joinSetPlayerUpdates(0);
	}
	if (isInMenus()) {
		int changed = 0;
		if (*(volatile u32*)0x006d0d90 == (0x0c000000 | (0x006d25c0 >> 2))) {
			*(volatile u32*)0x006d0d90 = 0x0c000000 | (((u32)&joinBeginClient >> 2) & 0x03ffffff);
			changed = 1;
		}
		if (*(volatile u32*)0x006d1174 == (0x0c000000 | (0x006d1928 >> 2))) {
			*(volatile u32*)0x006d1174 = 0x0c000000 | (((u32)&joinEndClient >> 2) & 0x03ffffff);
			changed = 1;
		}
		if (*(volatile u32*)0x006c1a08 == (0x0c000000 | (0x005992a8 >> 2))) {
			*(volatile u32*)0x006c1a08 = 0x0c000000 | (((u32)&joinLoadClient >> 2) & 0x03ffffff);
			changed = 1;
		}
		if (changed) {
			FlushCache(0);
			FlushCache(2);
		}
	}
#endif
	GameSettings* settings = gameGetSettings();
	int running = settings && isInGame() && !GAME_NET_INFO->onlyLocalPlayers && !gameHasEnded();
	int supported = running &&
	    (settings->GameType == GAMETYPE_DM || settings->GameType == GAMETYPE_SIEGE) &&
	    settings->GameLevel >= 40 && settings->GameLevel <= 49 &&
	    joinSupportsMode() && joinSupportsMap();

	joinSetAdmission(supported && gameConfig.allowMidGameJoining && gameAmIHost());
	if (supported) {
		if (gameAmIHost()) {
			joinSendGameTime();
			int i;
			for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
				if (joinSiegeRequests & (1 << i))
					joinSendSiegeState(i);
			}
			joinSiegeRequests = 0;
		} else {
			joinRequestSiegeState(settings);
		}
		joinCompletePlayers(settings);
		joinNotifyPlayers(settings);
	} else {
		joinRosterInitialized = 0;
		joinLastTimeSent = 0;
		joinSiegeRequests = 0;
	}
}
