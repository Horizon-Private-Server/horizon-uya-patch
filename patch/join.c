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
#include <libuya/math.h>
#include <libuya/math3d.h>
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
#define JOIN_GET_TEAM (0x00199c48)
#define JOIN_TEAM_RESERVE_CALL (0x0019ac1c)
#define JOIN_TEAM_SETUP_CALL (0x0019a650)
#define JOIN_REPORT_HOST (0x0019b788)
#define JOIN_REPORT_UPDATE_CALL (0x00195b38)
#define JOIN_REPORT_SEND_CALL (0x001964b8)
#define JOIN_REPORT_LEAVE_CALL (0x00196048)
#define JOIN_FORCE_WORLD_REPORT (0x00196468)
#else
#define JOIN_ADMISSION_GATE (0x0019a890)
#define JOIN_SETUP_REQUEST_GATE (0x0019a6a4)
#define JOIN_SETUP_STATE_GATE (0x0019a720)
#define JOIN_PLAYER_ARRAY (0x0022f550)
#define JOIN_COMPLETE_PLAYER (0x001999b8)
#define JOIN_INPUT_CALLBACK (0x0024078c)
#define JOIN_STATE_CALLBACK (0x0024077c)
#define JOIN_DECODE_INPUT (0x0066c910)
#define JOIN_GET_TEAM (0x00199d88)
#define JOIN_TEAM_RESERVE_CALL (0x0019ad5c)
#define JOIN_TEAM_SETUP_CALL (0x0019a790)
#define JOIN_REPORT_HOST (0x0019b8c8)
#define JOIN_REPORT_UPDATE_CALL (0x00195c28)
#define JOIN_REPORT_SEND_CALL (0x001965a8)
#define JOIN_REPORT_LEAVE_CALL (0x00196138)
#define JOIN_FORCE_WORLD_REPORT (0x00196558)
#endif

typedef struct joinWorldRequest {
	/* 0x00 */ u8 client;
	/* 0x01 */ u8 level;
	/* 0x02 */ u16 sequence;
} joinWorldRequest_t;

typedef struct joinWorldObject {
	/* 0x00 */ joinWorldRequest_t request;
	/* 0x04 */ u16 uid;
	/* 0x06 */ u16 oClass;
	/* 0x08 */ u8 state;
	/* 0x09 */ s8 team;
	/* 0x0a */ u16 reserved;
	/* 0x0c */ float value; /* Health, crank progress, or bridge travel. */
} joinWorldObject_t;

typedef struct joinFlagState {
	/* 0x00 */ joinWorldRequest_t request;
	/* 0x04 */ u16 uid;
	/* 0x06 */ u16 oClass;
	/* 0x08 */ float position[3];
	/* 0x14 */ int droppedAge;
	/* 0x18 */ int captures;
	/* 0x1c */ int saves;
	/* 0x20 */ s8 carrier;
	/* 0x21 */ s8 lastCarrier;
	/* 0x22 */ u8 state;
	/* 0x23 */ u8 reserved;
} joinFlagState_t;

Player** playerGetAll(void);
void FlushCache(int operation);
int flagGetTrackerIndex(Moby* flagMoby);
int getBaseHealthPercent(LocalPlayerYourBaseGameData* baseGameData, int team);
static void joinSetLoadingReceivers(int enabled);
static int joinGetMode(void);

#if !UYA_PAL
static void joinSetPlayerUpdates(int enabled);
#endif

extern PatchGameConfig_t gameConfig;
extern int mapOverrideResponse;
extern int expectedMapVersion;
extern int flagTrackedLastCarrierIdx[2];

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
static int joinNeedsWorldState;
static int joinWorldRetry;
static int joinLastWorldRequest;
static u16 joinWorldSequence;
static u16 joinWorldSequences[GAME_MAX_PLAYERS];
static u32 joinWorldRequests;
static int joinKeepWorldOnLeave;
static int joinPreviousHost = -1;
static int joinHostLoadTime;

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
static int joinPendingLevel;
static int joinPendingMode;
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
	customModeDownloadReady = 0;
	customModeDownloadPending = 0;
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
	joinNeedsWorldState = joinClientLoading;
	joinWorldRetry = 0;
	joinLastWorldRequest = gameGetTime();
	++joinWorldSequence;
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
	customModeDownloadPending = 0;
	customModeDownloadReady = 0;
	joinClientLoading = 0;
	joinNeedsGameTime = 0;
	joinNeedsWorldState = 0;
	joinRememberedPlayers = 0;
	joinSetLoadingReceivers(0);
	joinSetPlayerUpdates(0);
	return ((int (*)(void*))0x006d1928)(context);
}

/*
 * NAME : joinLoadClient
 * DESCRIPTION : Wait for custom-mode code, then supply the gameplay pad and start loading.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinLoadClient(int level, int mode)
{
	if (joinClientLoading && joinGetMode() > 0 && !customModeDownloadReady) {
		joinPendingLevel = level;
		joinPendingMode = mode;
		customModeDownloadPending = 1;
		return;
	}
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
 * NAME : joinGetMode
 * DESCRIPTION : Read the selected custom mode, including a map-forced mode.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinGetMode(void)
{
	int mode = gameConfig.customModeId;
	int index = patchStateContainer.customMapId - 1;
	if (gameConfig.isCustomMap && customMapDefs && index >= 0 && index < customMapDefCount &&
	    customMapDefs[index].ForcedCustomModeId)
		mode = customMapDefs[index].ForcedCustomModeId;
	return mode;
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
 * NAME : joinReceiveWorldRequest
 * DESCRIPTION : Queue a current-roster client's map-state catch-up request.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinReceiveWorldRequest(void* connection, void* data)
{
	joinWorldRequest_t* request = data;
	GameSettings* settings = gameGetSettings();
	if (connection != netGetDmeServerConnection() || !settings || !isInGame() ||
	    !gameAmIHost() ||
	    request->level != settings->GameLevel || request->client >= GAME_MAX_PLAYERS ||
	    request->client == GAME_CLIENT_ID)
		return sizeof(*request);
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (settings->PlayerClients[i] == request->client && settings->PlayerAccountIds[i] >= 0) {
			joinWorldSequences[request->client] = request->sequence;
			joinWorldRequests |= 1 << request->client;
			break;
		}
	}
	return sizeof(*request);
}

/*
 * NAME : joinWorldObjectEnabled
 * DESCRIPTION : Let host rules and native options take precedence over object catch-up.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinWorldObjectEnabled(u16 oClass, Moby* moby)
{
	if (gameConfig.grDestructableBridges &&
	    (oClass == MOBY_ID_KORGON_BRIDGE_PIECE ||
	    oClass == MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_CENTER ||
	    oClass == MOBY_ID_BLACKWATER_CITY_DESTRUCTABLE_BRIDGE_CENTER))
		return 0;
	if (oClass == MOBY_ID_PLAYER_TURRET && gameConfig.grDisablePlayerTurrets)
		return 0;
	if (gameConfig.grDisableDrones &&
	    (oClass == MOBY_ID_DRONE_BOT ||
	    oClass == MOBY_ID_DRONE_BOT_CLUSTER_CONFIG ||
	    oClass == MOBY_ID_DRONE_BOT_CLUSTER_UPDATER))
		return 0;
	/* Removing the glass still leaves health pickups and their respawn cycle.
	 * Only the fully disabled setting excludes health objects from catch-up. */
	if (gameConfig.grHealthBoxes == 2 &&
	    (oClass == MOBY_ID_HEALTH_BOX_MP || oClass == MOBY_ID_HEALTH_ORB_MP))
		return 0;
	GameOptions* options = gameGetOptions();
	if (options && oClass == MOBY_ID_TURBOSLIDER && !options->GameFlags.MultiplayerGameFlags.Turboslider)
		return 0;
	if (options && oClass == MOBY_ID_HOVERSHIP && !options->GameFlags.MultiplayerGameFlags.Hovership)
		return 0;
	if (options && oClass == MOBY_ID_SIEGE_NODE && !options->GameFlags.MultiplayerGameFlags.Nodes)
		return 0;
	if (options && oClass == MOBY_ID_GATLING_TURRET &&
	    !options->GameFlags.MultiplayerGameFlags.BaseDefense_GatlinTurrets)
		return 0;
	if (oClass == MOBY_ID_NODE_TURRET) {
		if (gameConfig.grNoBaseDefense_SmallTurrets >= 2)
			return 0;
		if (moby && moby->pVar) {
			/* Match the gameplay rule: base turrets belong to BASE_LIGHT,
			 * while capture-node turrets belong to SIEGE_NODE. */
			Moby* start = mobyListGetStart();
			Moby* end = mobyListGetEnd();
			int parent = *(int*)((u8*)moby->pVar + 0x24c);
			if (start && end && parent >= 0 && parent < end - start) {
				if (start[parent].oClass == MOBY_ID_BASE_LIGHT &&
				    (gameConfig.grNoBaseDefense_SmallTurrets == 1 ||
				    (options && !options->GameFlags.MultiplayerGameFlags.BaseDefense_SmallTurrets)))
					return 0;
				if (start[parent].oClass == MOBY_ID_SIEGE_NODE && options &&
				    !options->GameFlags.MultiplayerGameFlags.Nodes)
					return 0;
			}
		}
	}
	return 1;
}

/*
 * NAME : joinApplyWorldObject
 * DESCRIPTION : Restore an authored objective through its native event handler.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinApplyWorldObject(joinWorldObject_t* object)
{
	if (!joinWorldObjectEnabled(object->oClass, 0))
		return 1;
	Moby* moby;
	Moby* end = mobyListGetEnd();
	for (moby = mobyListGetStart(); moby && moby < end; ++moby) {
		if ((u16)moby->UID == object->uid && (u16)moby->oClass == object->oClass)
			break;
	}
	if (!moby || moby == end || !moby->pVar || moby->state == 0)
		return 0;
	if ((u8)moby->state >= 0x80 || !joinWorldObjectEnabled(object->oClass, moby))
		return 1;

	u8* vars = moby->pVar;
	if (object->oClass == MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_CENTER ||
	    object->oClass == MOBY_ID_BLACKWATER_CITY_DESTRUCTABLE_BRIDGE_CENTER) {
		/* Bridges are local controllers, not gubers. Schedule their normal
		 * break path so collision and linked bridge pieces update together. */
		if (object->state == 2 && moby->state == 1) {
			*(int*)vars = gameGetTime() - 1;
			*(int*)(vars + 0x10) = 0;
		}
		return 1;
	}
	if (object->oClass == MOBY_ID_KORGON_BRIDGE_PIECE) {
		float range = *(float*)(vars + 0xbc);
		if (!(range > 0) || !(object->value >= 0 && object->value <= range))
			return 0;
		/* Match the bridge's native eased travel, including collision position.
		 * Its normal update continues following the linked crank afterward. */
		*(float*)(vars + 0x88) = object->value;
		*(float*)(vars + 0xb4) = 0;
		float progress = object->value / range;
		float eased = (1.0f - cosf(progress * MATH_PI)) * 0.5f;
		vector_lerp(moby->position, (float*)(vars + 0x90), (float*)(vars + 0xa0), eased);
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

	if (object->oClass == MOBY_ID_BOLT_CRANK) {
		if (!(object->value >= 0 && object->value <= 1))
			return 1;
		/* Native progress event: bias, force, player, release (13 bytes).
		 * Do not claim or release a player currently operating the crank. */
		u32 progress[4] = { 0, 0, 0xffffffff, 0 };
		memcpy(progress, &object->value, sizeof(object->value));
		event.NetEvent.NetDataSize = 13;
		memcpy(event.NetEvent.NetData, progress, 13);
	} else if (object->oClass == MOBY_ID_SIEGE_NODE) {
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
	} else if (object->oClass == MOBY_ID_GATLING_TURRET || object->oClass == MOBY_ID_BASE_COMPUTER ||
	    object->oClass == MOBY_ID_NODE_TURRET) {
		int turret = object->oClass != MOBY_ID_BASE_COMPUTER;
		int smallTurret = object->oClass == MOBY_ID_NODE_TURRET;
		int destroyed = smallTurret ? 7 : turret ? 5 : 2;
		if (moby->state == destroyed)
			return 1;
		if (object->state == destroyed || object->value <= 0) {
			event.NetEvent.EventID = turret ? 1 : 0;
			event.NetEvent.NetDataSize = 8;
		} else {
			float damage = *(float*)(vars + 0x30) - object->value;
			if (damage <= 0)
				return 1;
			if (smallTurret) {
				/* Its native damage event carries no health change. Restore
				 * remaining health directly; destruction uses its event below. */
				*(float*)(vars + 0x30) = object->value;
				return 1;
			}
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
 * NAME : joinReceiveWorldState
 * DESCRIPTION : Apply only this join attempt's objective snapshot.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinReceiveWorldState(void* connection, void* data)
{
	joinWorldObject_t* object = data;
	GameSettings* settings = gameGetSettings();
	if (connection != netGetDmeServerConnection() || !settings || !isInGame() ||
	    gameAmIHost() || !joinNeedsWorldState ||
	    object->request.client != GAME_CLIENT_ID || object->request.level != settings->GameLevel ||
	    object->request.sequence != joinWorldSequence)
		return sizeof(*object);
	if (!object->oClass) {
		joinNeedsWorldState = joinWorldRetry;
		joinWorldRetry = 0;
	} else if (!joinApplyWorldObject(object)) {
		joinWorldRetry = 1;
	}
	return sizeof(*object);
}

/*
 * NAME : joinReceiveFlagState
 * DESCRIPTION : Restore a late joiner's flags and team totals without awarding captures.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinReceiveFlagState(void* connection, void* data)
{
	joinFlagState_t* snapshot = data;
	GameSettings* settings = gameGetSettings();
	if (connection != netGetDmeServerConnection() || !settings || !isInGame() ||
	    gameAmIHost() || !joinNeedsWorldState || settings->GameType != GAMETYPE_CTF ||
	    snapshot->request.client != GAME_CLIENT_ID || snapshot->request.level != settings->GameLevel ||
	    snapshot->request.sequence != joinWorldSequence)
		return sizeof(*snapshot);
	if ((snapshot->oClass != MOBY_ID_CTF_RED_FLAG && snapshot->oClass != MOBY_ID_CTF_BLUE_FLAG) ||
	    (snapshot->state != 1 && snapshot->state != 2) ||
	    snapshot->carrier < -1 || snapshot->carrier >= GAME_MAX_PLAYERS ||
	    (snapshot->state == 2 && snapshot->carrier < 0) ||
	    (snapshot->state == 1 && snapshot->carrier != -1) || snapshot->droppedAge < 0)
		return sizeof(*snapshot);
	Moby* moby;
	Moby* end = mobyListGetEnd();
	for (moby = mobyListGetStart(); moby && moby < end; ++moby) {
		if ((u16)moby->UID == snapshot->uid && moby->oClass == snapshot->oClass)
			break;
	}
	GameData* game = gameGetData();
	if (!moby || moby == end || !moby->pVar || moby->state == 0 || !game || !game->CTFGameData ||
	    (snapshot->carrier >= 0 && !playerGetAll()[snapshot->carrier])) {
		joinWorldRetry = 1;
		return sizeof(*snapshot);
	}
	if ((u8)moby->state >= 0x80)
		return sizeof(*snapshot);
	flagPVars_t* vars = moby->pVar;
	MobyFunctions* functions = mobyGetFunctions(moby);
	if (!functions || !functions->FUNC_1C) {
		joinWorldRetry = 1;
		return sizeof(*snapshot);
	}
	GuberEvent event;
	memset(&event, 0, sizeof(event));
	event.NetEvent.CreateTime = gameGetTime();
	event.NetEvent.OriginClientIdx = gameGetHostId();
	if (moby->guberMoby)
		event.NetEvent.ObjUID = moby->guberMoby->Guber.Id.UID;
	if (snapshot->state == 2) {
		/* Reuse the native pickup handler to bind the existing carrier. The
		 * host-authoritative pickup code in main.c remains unchanged. */
		if (moby->state != 2 || vars->carrierIdx != snapshot->carrier) {
			int carrier = snapshot->carrier;
			event.NetEvent.NetDataSize = sizeof(carrier);
			memcpy(event.NetEvent.NetData, &carrier, sizeof(carrier));
			((void (*)(Moby*, GuberEvent*))functions->FUNC_1C)(moby, &event);
		}
	} else {
		/* The native drop event restores its transform and releases a previous
		 * carrier. A return/capture event would incorrectly award old points. */
		VECTOR position = { snapshot->position[0], snapshot->position[1], snapshot->position[2], 0 };
		VECTOR delta;
		vector_subtract(delta, moby->position, position);
		if (moby->state != 1 || vars->carrierIdx != -1 ||
		    vector_sqrmag(delta) > 0.01f) {
			event.NetEvent.EventID = 2;
			event.NetEvent.NetDataSize = sizeof(position);
			memcpy(event.NetEvent.NetData, position, sizeof(position));
			((void (*)(Moby*, GuberEvent*))functions->FUNC_1C)(moby, &event);
		}
		vars->timeFlagDropped = gameGetTime() - snapshot->droppedAge;
	}
	int flag = flagGetTrackerIndex(moby);
	if (flag >= 0 && snapshot->lastCarrier >= -1 && snapshot->lastCarrier < GAME_MAX_PLAYERS)
		flagTrackedLastCarrierIdx[flag] = snapshot->lastCarrier;
	if (snapshot->oClass == MOBY_ID_CTF_BLUE_FLAG) {
		game->CTFGameData->blueTeamCaptures = snapshot->captures;
		game->CTFGameData->blueTeamSaves = snapshot->saves;
	} else {
		game->CTFGameData->redTeamCaptures = snapshot->captures;
		game->CTFGameData->redTeamSaves = snapshot->saves;
	}
	return sizeof(*snapshot);
}

/*
 * NAME : joinSendWorldState
 * DESCRIPTION : Send active map objects by class and UID, independent of game mode.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinSendWorldState(int client)
{
	GameSettings* settings = gameGetSettings();
	GameData* game = gameGetData();
	if (!settings || !game)
		return;
	joinWorldObject_t object;
	memset(&object, 0, sizeof(object));
	object.request.client = client;
	object.request.level = settings->GameLevel;
	object.request.sequence = joinWorldSequences[client];
	Moby* moby;
	Moby* end = mobyListGetEnd();
	for (moby = mobyListGetStart(); moby && moby < end; ++moby) {
		if (!moby->pVar || (u8)moby->state >= 0x80 || (u16)moby->UID == 0xffff)
			continue;
		if (!joinWorldObjectEnabled(moby->oClass, moby))
			continue;
		if (settings->GameType == GAMETYPE_CTF &&
		    (moby->oClass == MOBY_ID_CTF_RED_FLAG || moby->oClass == MOBY_ID_CTF_BLUE_FLAG)) {
			if (!game->CTFGameData || (moby->state != 1 && moby->state != 2))
				continue;
			flagPVars_t* vars = moby->pVar;
			joinFlagState_t flag;
			memset(&flag, 0, sizeof(flag));
			flag.request = object.request;
			flag.uid = moby->UID;
			flag.oClass = moby->oClass;
			memcpy(flag.position, moby->position, sizeof(flag.position));
			flag.droppedAge = gameGetTime() - vars->timeFlagDropped;
			if (flag.droppedAge < 0)
				flag.droppedAge = 0;
			flag.carrier = vars->carrierIdx;
			flag.lastCarrier = flagTrackedLastCarrierIdx[flagGetTrackerIndex(moby)];
			flag.state = moby->state;
			flag.captures = moby->oClass == MOBY_ID_CTF_BLUE_FLAG ?
			    game->CTFGameData->blueTeamCaptures : game->CTFGameData->redTeamCaptures;
			flag.saves = moby->oClass == MOBY_ID_CTF_BLUE_FLAG ?
			    game->CTFGameData->blueTeamSaves : game->CTFGameData->redTeamSaves;
			netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_JOIN_FLAG_STATE,
			    sizeof(flag), &flag);
			continue;
		}
		object.team = -1;
		object.value = 0;
		if (moby->oClass == MOBY_ID_SIEGE_NODE) {
			int node = *(int*)((u8*)moby->pVar + 0x2c);
			if (!game->allYourBaseGameData || node < 0 || node >= 8)
				continue;
			object.team = game->allYourBaseGameData->nodeTeam[node];
		} else if (moby->oClass == MOBY_ID_BOLT_CRANK) {
			object.value = *(float*)moby->pVar;
		} else if (moby->oClass == MOBY_ID_KORGON_BRIDGE_PIECE) {
			object.value = *(float*)((u8*)moby->pVar + 0x88);
		} else if (moby->oClass == MOBY_ID_GATLING_TURRET || moby->oClass == MOBY_ID_BASE_COMPUTER ||
		    moby->oClass == MOBY_ID_NODE_TURRET) {
			object.value = *(float*)((u8*)moby->pVar + 0x30);
		} else if (moby->oClass != MOBY_ID_HOVEN_AND_OUTPOST_X12_DESTRUCTABLE_BRIDGE_CENTER &&
		    moby->oClass != MOBY_ID_BLACKWATER_CITY_DESTRUCTABLE_BRIDGE_CENTER) {
			continue;
		}
		object.uid = moby->UID;
		object.oClass = moby->oClass;
		object.state = moby->state;
		netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_JOIN_WORLD_STATE,
		    sizeof(object), &object);
	}
	object.oClass = 0;
	netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_JOIN_WORLD_STATE,
	    sizeof(object), &object);
}

/*
 * NAME : joinRequestWorldState
 * DESCRIPTION : Retry catch-up after loading until all objective objects exist.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinRequestWorldState(GameSettings* settings)
{
	if (!joinNeedsWorldState || gameAmIHost() ||
	    gameGetTime() - joinLastWorldRequest < TIME_SECOND)
		return;
	joinWorldRequest_t request = { GAME_CLIENT_ID, settings->GameLevel, joinWorldSequence };
	joinWorldRetry = 0;
	netBroadcastCustomAppMessage(netGetDmeServerConnection(), CUSTOM_MSG_ID_JOIN_WORLD_REQUEST,
	    sizeof(request), &request);
	joinLastWorldRequest = gameGetTime();
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
 * NAME : joinTeamScore
 * DESCRIPTION : Compare current team scores without using account statistics.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinTeamScore(GameSettings* settings, int team, int joiningPlayer)
{
	GameData* game = gameGetData();
	if (!game)
		return 0;
	if (joinGetMode() == CUSTOM_MODE_KOTH)
		return patchStateContainer.gameStateUpdate.TeamScores[team];
	if (settings->GameType == GAMETYPE_CTF && game->CTFGameData)
		return team == 0 ? game->CTFGameData->blueTeamCaptures : game->CTFGameData->redTeamCaptures;
	int score = 0;
	int i;
	if (settings->GameType == GAMETYPE_SIEGE && game->allYourBaseGameData) {
		GameOptions* options = gameGetOptions();
		if (options && options->GameFlags.MultiplayerGameFlags.Nodes) {
			for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
				if (game->allYourBaseGameData->nodeTeam[i] == team)
					++score;
			}
		}
		/* Node ownership takes priority; remaining base health breaks its ties. */
		return score * 101 + getBaseHealthPercent(game->allYourBaseGameData, team);
	}
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		if (i != joiningPlayer && settings->PlayerClients[i] >= 0 && settings->PlayerTeams[i] == team)
			score += game->playerStats.frag[i].kills - game->playerStats.frag[i].deaths;
	}
	return score;
}

/*
 * NAME : joinSelectTeam
 * DESCRIPTION : Assign a late joiner to an unused color or the smaller trailing team.
 * NOTES : Runs while the host reserves the slot, before sending its roster.
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinSelectTeam(int gameType, int teams, int preferred, int player)
{
	GameSettings* settings = gameGetSettings();
	if (!joinAdmissionOpen || !settings || !gameAmIHost() || !isInGame() ||
	    player < 0 || player >= GAME_MAX_PLAYERS)
		return ((int (*)(int, int, int, int))JOIN_GET_TEAM)(gameType, teams, preferred, player);
	// The join request may still carry staging defaults; use the host's live rules.
	gameType = settings->GameType;
	GameOptions* options = gameGetOptions();
	if (options)
		teams = options->GameFlags.MultiplayerGameFlags.Teams;
	int counts[GAME_MAX_PLAYERS] = { 0 };
	u32 eligible = 0;
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		int team = settings->PlayerTeams[i];
		/* Reserved slots count even before their player bodies exist. */
		if (i == player || settings->PlayerClients[i] < 0 || team < 0 || team >= GAME_MAX_PLAYERS)
			continue;
		++counts[team];
		eligible |= 1 << team;
	}
	if (gameType == GAMETYPE_DM && !teams) {
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			if (!counts[i])
				return i;
		}
	} else {
		if (gameType == GAMETYPE_CTF || gameType == GAMETYPE_SIEGE)
			eligible = 3;
		int selected = -1;
		int bestScore = 0;
		for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
			if (!(eligible & (1 << i)))
				continue;
			if (selected >= 0 && counts[i] > counts[selected])
				continue;
			int score = joinTeamScore(settings, i, player);
			if (selected < 0 || counts[i] < counts[selected] || score < bestScore) {
				selected = i;
				bestScore = score;
			}
		}
		if (selected >= 0)
			return selected;
	}
	return ((int (*)(int, int, int, int))JOIN_GET_TEAM)(gameType, teams, preferred, player);
}

/*
 * NAME : joinKeepTeam
 * DESCRIPTION : Preserve the host's reservation when the joiner sends its preferred team.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinKeepTeam(int gameType, int teams, int preferred, int player)
{
	GameSettings* settings = gameGetSettings();
	if (joinAdmissionOpen && settings && gameAmIHost() && isInGame() &&
	    player >= 0 && player < GAME_MAX_PLAYERS && settings->PlayerClients[player] >= 0 &&
	    settings->PlayerTeams[player] >= 0 && settings->PlayerTeams[player] < GAME_MAX_PLAYERS)
		return settings->PlayerTeams[player];
	return ((int (*)(int, int, int, int))JOIN_GET_TEAM)(gameType, teams, preferred, player);
}

/*
 * NAME : joinSetAdmission
 * DESCRIPTION : Toggle native admission and team hooks while preserving normal lobby behavior.
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
	u32 original = 0x0c000000 | (JOIN_GET_TEAM >> 2);
	u32 reserve = 0x0c000000 | (((u32)&joinSelectTeam >> 2) & 0x03ffffff);
	u32 setup = 0x0c000000 | (((u32)&joinKeepTeam >> 2) & 0x03ffffff);
	if (*(volatile u32*)JOIN_TEAM_RESERVE_CALL != (joinAdmissionOpen ? reserve : original) ||
	    *(volatile u32*)JOIN_TEAM_SETUP_CALL != (joinAdmissionOpen ? setup : original))
		return;
	for (i = 0; i < 3; ++i)
		*(volatile u32*)joinGates[i] = enabled ? joinGateValues[i] : JOIN_GATE_ORIGINAL;
	*(volatile u32*)JOIN_TEAM_RESERVE_CALL = enabled ? reserve : original;
	*(volatile u32*)JOIN_TEAM_SETUP_CALL = enabled ? setup : original;

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
 * NAME : joinIsReportHost
 * DESCRIPTION : Let the elected session host publish the continuing game's world report.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinIsReportHost(void)
{
	if (gameConfig.allowMidGameJoining && isInGame() &&
	    !GAME_NET_INFO->onlyLocalPlayers && gameGetSettings())
		return gameAmIHost();
	return ((int (*)(void))JOIN_REPORT_HOST)();
}

/*
 * NAME : joinShouldCloseWorld
 * DESCRIPTION : Keep a live world listed when its departing host leaves players behind.
 * NOTES : The native leave routine still disconnects and cleans up normally.
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static int joinShouldCloseWorld(void)
{
	int keepWorld = joinKeepWorldOnLeave;
	joinKeepWorldOnLeave = 0;
	return keepWorld ? 0 : joinIsReportHost();
}

/*
 * NAME : joinInstallHostReports
 * DESCRIPTION : Patch resident world-report checks without changing other titles' server logic.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinInstallHostReports(void)
{
	static const u32 calls[] = { JOIN_REPORT_UPDATE_CALL, JOIN_REPORT_SEND_CALL, JOIN_REPORT_LEAVE_CALL };
	int changed = 0;
	int i;
	for (i = 0; i < 3; ++i) {
		volatile u32* call = (volatile u32*)calls[i];
		if (*call != (0x0c000000 | (JOIN_REPORT_HOST >> 2)))
			continue;
		u32 function = i == 2 ? (u32)&joinShouldCloseWorld : (u32)&joinIsReportHost;
		*call = 0x0c000000 | ((function >> 2) & 0x03ffffff);
		changed = 1;
	}
	if (changed) {
		FlushCache(0);
		FlushCache(2);
	}
}

/*
 * NAME : joinUpdateHost
 * DESCRIPTION : Keep continuing games listed after native host migration.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
static void joinUpdateHost(GameSettings* settings, int supported)
{
	if (!settings || !isInGame()) {
		joinPreviousHost = -1;
		return;
	}
	joinKeepWorldOnLeave = 0;
	if (!supported)
		return;
	if (joinHostLoadTime != settings->GameLoadStartTime) {
		joinHostLoadTime = settings->GameLoadStartTime;
		joinPreviousHost = -1;
	}
	int otherClients = 0;
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		int client = settings->PlayerClients[i];
		if (client < 0 || settings->PlayerAccountIds[i] < 0)
			continue;
		if (client != GAME_CLIENT_ID)
			++otherClients;
	}
	joinKeepWorldOnLeave = otherClients > 0;
	int host = gameGetHostId();
	if (joinPreviousHost >= 0 && joinPreviousHost != host && gameAmIHost()) {
		joinLastTimeSent = 0;
		((void (*)(void))JOIN_FORCE_WORLD_REPORT)();
	}
	joinPreviousHost = host;
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
	joinInstallHostReports();
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_JOIN_GAME_TIME, &joinReceiveGameTime);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_JOIN_WORLD_REQUEST, &joinReceiveWorldRequest);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_JOIN_WORLD_STATE, &joinReceiveWorldState);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_JOIN_FLAG_STATE, &joinReceiveFlagState);
	if (isInGame())
		joinSetLoadingReceivers(0);
#if !UYA_PAL
	if (customModeDownloadPending) {
		if (!joinClientLoading || !netGetLobbyServerConnection() || !gameGetSettings()) {
			customModeDownloadPending = 0;
		} else if (customModeDownloadReady) {
			customModeDownloadPending = 0;
			joinLoadClient(joinPendingLevel, joinPendingMode);
		}
	}
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
	    (settings->GameType == GAMETYPE_DM || settings->GameType == GAMETYPE_SIEGE ||
	    settings->GameType == GAMETYPE_CTF) &&
	    settings->GameLevel >= 40 && settings->GameLevel <= 49 &&
	    joinSupportsMap();

	joinUpdateHost(settings, supported && gameConfig.allowMidGameJoining);
	if (running && gameHasEnded())
		supported = 0;
	joinSetAdmission(supported && gameConfig.allowMidGameJoining && gameAmIHost());
	if (supported) {
		if (gameAmIHost()) {
			joinSendGameTime();
			int i;
			for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
				if (joinWorldRequests & (1 << i))
					joinSendWorldState(i);
			}
			joinWorldRequests = 0;
		} else {
			joinRequestWorldState(settings);
		}
		joinCompletePlayers(settings);
		joinNotifyPlayers(settings);
	} else {
		joinRosterInitialized = 0;
		joinLastTimeSent = 0;
		joinWorldRequests = 0;
	}
}
