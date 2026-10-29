#include <tamtypes.h>
#include <stdio.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/player.h>
#include <libuya/pad.h>
#include <libuya/string.h>
#include <libuya/interop.h>
#include "config.h"
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
#define JOIN_DECODE_INPUT (0x0066f260)
#else
#define JOIN_ADMISSION_GATE (0x0019a890)
#define JOIN_SETUP_REQUEST_GATE (0x0019a6a4)
#define JOIN_SETUP_STATE_GATE (0x0019a720)
#define JOIN_PLAYER_ARRAY (0x0022f550)
#define JOIN_COMPLETE_PLAYER (0x001999b8)
#define JOIN_INPUT_CALLBACK (0x0024078c)
#define JOIN_DECODE_INPUT (0x0066c910)
#endif

Player** playerGetAll(void);
void FlushCache(int operation);
static void joinSetInputReceiver(int enabled);

#if !UYA_PAL
static void joinSetPlayerUpdates(int enabled);
#endif

extern PatchGameConfig_t gameConfig;

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

/* These two lobby call sites bracket the native Join request. Keep the
 * loading receivers scoped to that request, including its failure cleanup. */
static int joinBeginClient(void* context)
{
	int result = ((int (*)(void*))0x006d25c0)(context);
	joinClientLoading = result != 0;
	joinSetInputReceiver(joinClientLoading);
	joinSetPlayerUpdates(joinClientLoading);
	return result;
}

static int joinEndClient(void* context)
{
	joinClientLoading = 0;
	joinSetInputReceiver(0);
	joinSetPlayerUpdates(0);
	return ((int (*)(void*))0x006d1928)(context);
}

/* A late join can leave staging before its controller-selection update.
 * Use the already active menu pad for the sole local player, rather than
 * letting the player constructor dereference an empty gameplay-pad slot. */
static void joinLoadClient(int level, int mode)
{
	if (joinClientLoading && GAME_NET_INFO->numLocalPlayers == 1 &&
	    !PAD_POINTER[3] && PAD_POINTER[0])
		PAD_POINTER[3] = PAD_POINTER[0];
	((void (*)(int, int))0x005992a8)(level, mode);
}

/* Objects may already exist when staging resets the player records. Bind the
 * current object through the native creation callback before marking fields
 * on its player data; the original update assumes this binding always exists. */
static void joinPlayerObjectUpdated(int connection, int objectId, int fields)
{
	void* object = 0;
	if (((int (*)(void**, int))0x0016ed00)(&object, objectId) || !object)
		return;
	struct tNW_PlayerData* data = *(struct tNW_PlayerData**)((u8*)object + 0x1c);
	if (!data || data->playerIndex < 0 || data->playerIndex >= GAME_MAX_PLAYERS)
		return;
	struct tNW_Player* player = (struct tNW_Player*)(JOIN_PLAYER_ARRAY + data->playerIndex * JOIN_PLAYER_STRIDE);
	if (!player->pNetPlayerData)
		((void (*)(int, int))0x00199ba0)(connection, objectId);
	if (player->pNetPlayerData == data)
		((void (*)(int, int, int))0x00199c40)(connection, objectId, fields);
}

static void joinSetPlayerUpdates(int enabled)
{
	int type = *(int*)0x002407bc;
	((int (*)(int, void*, void*, void*))0x0016eeb0)
	    (type, (void*)0x00199ba0,
	    enabled ? (void*)&joinPlayerObjectUpdated : (void*)0x00199c40,
	    (void*)0x00199cb8);
}
#endif

/* A running host sends input before the joining client has loaded gameplay's
 * receiver. Consume the native compressed format without applying it to a
 * player. The level installs its normal receiver when gameplay initializes. */
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

static void joinSetInputReceiver(int enabled)
{
	volatile u32* callback = (volatile u32*)JOIN_INPUT_CALLBACK;
	if (enabled && !*callback)
		*callback = (u32)&joinReceiveLoadingInput;
	else if (!enabled && *callback == (u32)&joinReceiveLoadingInput)
		*callback = 0;
}

/* Only relax the host-state checks. Native password, duplicate-account,
 * reserved-slot and capacity checks still decide whether to admit a client.
 * Never reset the host's state or the running game's start/load timestamps. */
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

/* Loading normally completes remote hero creation. Existing clients have
 * already passed that phase, so complete a late human's pending creation
 * with the same regional wrapper once its native player object arrives.
 * Bot guests and our own local players remain owned by their existing code. */
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
		if (!data || data->playerIndex != i || data->accountId != settings->PlayerAccountIds[i] ||
		    netPlayer->netClientIndex != client || !netPlayer->bSpawned ||
		    netPlayer->bCallbackCalled || netPlayer->bUnk_1a4)
			continue;

		((void (*)(struct tNW_Player*))JOIN_COMPLETE_PLAYER)(netPlayer);
	}
}

/* Reuse the native departure feed, including its duration and oldest-entry
 * eviction. Resolve its storage from the current map's leave function. */
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

/* Each existing client observes the same roster. No extra network message is
 * needed, and a newly loading client seeds its baseline without announcing
 * everyone who was already present. Negative account IDs belong to bots. */
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

void joinTick(void)
{
	if (isInGame())
		joinSetInputReceiver(0);
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
	int supported = running && settings->GameType == GAMETYPE_DM &&
	    settings->GameLevel >= 40 && settings->GameLevel <= 49 &&
	    !patchStateContainer.CustomMapId && !gameConfig.customModeId;

	joinSetAdmission(supported && gameAmIHost());
	if (supported) {
		joinCompletePlayers(settings);
		joinNotifyPlayers(settings);
	} else {
		joinRosterInitialized = 0;
	}
}
