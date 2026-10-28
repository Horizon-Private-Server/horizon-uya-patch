#include <tamtypes.h>
#include <stddef.h>
#include <string.h>

#include <libuya/math.h>
#include <libuya/math3d.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/player.h>
#include <libuya/pad.h>
#include <libuya/ui.h>
#include <libuya/camera.h>
#include <libuya/interop.h>
#include <libuya/hud.h>

/* EE ABI: VECTOR has 16-byte alignment; the structure ends at 0x40. */
typedef struct playerSpectateData {
	/* 0x00 */ int Enabled;
	/* 0x04 */ int Index;
	/* 0x08 */ int HasShownHelp;
	/* EE alignment inserts four bytes before the next VECTOR. */
	/* 0x10 */ VECTOR LastCameraPos;
	/* 0x20 */ float LastCameraZ;
	/* 0x24 */ float LastCameraY;
	/* EE alignment inserts eight bytes before the next VECTOR. */
	/* 0x30 */ VECTOR FollowOffset;
} playerSpectateData_t;

typedef char spectateFollowOffsetCheck[(offsetof(playerSpectateData_t, FollowOffset) == 0x30) ? 1 : -1];
typedef char spectateSizeCheck[(sizeof(playerSpectateData_t) == 0x40) ? 1 : -1];
Player **playerGetAll(void);
void FlushCache(int mode);
static void spectateCameraPost(void);
static void spectateHudPreRender(void);

/* Deadlocked's follow-camera interpolation, using UYA's FPS camera fields. */
static const float CAMERA_POSITION_SHARPNESS = 50;
static const float CAMERA_ROTATION_SHARPNESS = 30;
static const float VEHICLE_CAMERA_ROTATION_SHARPNESS = 3;
static const float VEHICLE_DISTANCE[] = { 3, 0, 2, 0, 3, 0 };
static const float VEHICLE_ELEVATION[] = { 3, 3, 2.5, 3, 3, 3 };

static playerSpectateData_t SpectateData[GAME_MAX_LOCALS];

#if UYA_PAL
static VariableAddress_t cameraRender = {
	.Bakisi = 0x447960,
	.Hoven = 0x4494e0,
	.OutpostX12 = 0x4402e0,
	.KorgonOutpost = 0x43dea0,
	.Metropolis = 0x43d1e0,
	.BlackwaterCity = 0x43a9e0,
	.CommandCenter = 0x43b660,
	.BlackwaterDocks = 0x43dee0,
	.AquatosSewers = 0x43d1e0,
	.MarcadiaPalace = 0x43cb60
};
#else
static VariableAddress_t cameraRender = {
	.Bakisi = 0x446ab8,
	.Hoven = 0x448578,
	.OutpostX12 = 0x43f3b8,
	.KorgonOutpost = 0x43cff8,
	.Metropolis = 0x43c338,
	.BlackwaterCity = 0x439ab8,
	.CommandCenter = 0x43a8f8,
	.BlackwaterDocks = 0x43d138,
	.AquatosSewers = 0x43c478,
	.MarcadiaPalace = 0x43bdb8
};
#endif
static VariableAddress_t scoreboardRender = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004b16a8,
	.Hoven = 0x004b37c0,
	.OutpostX12 = 0x004a9098,
	.KorgonOutpost = 0x004a6830,
	.Metropolis = 0x004a5b80,
	.BlackwaterCity = 0x004a3418,
	.CommandCenter = 0x004a3410,
	.BlackwaterDocks = 0x004a5c90,
	.AquatosSewers = 0x004a4f90,
	.MarcadiaPalace = 0x004a4910,
#else
	.Lobby = 0,
	.Bakisi = 0x004af158,
	.Hoven = 0x004b11b0,
	.OutpostX12 = 0x004a6ac8,
	.KorgonOutpost = 0x004a42e0,
	.Metropolis = 0x004a3630,
	.BlackwaterCity = 0x004a0e48,
	.CommandCenter = 0x004a1000,
	.BlackwaterDocks = 0x004a3840,
	.AquatosSewers = 0x004a2b80,
	.MarcadiaPalace = 0x004a24c0,
#endif
};

static GuiState_t *scoreboardStates[GAME_MAX_LOCALS];
static u32 cameraPostAddress;
static u32 cameraRenderAddress;
static u32 reticleCallAddress;
static u32 reticleOriginal;

/* Native scoreboard update owns the respawn timer/text. Keep its root and
 * header container visible: the respawn widget (0x60038) is a child of 0x60001. */
static void spectateHudPreRender(void)
{
	int localIndex = hudGetCurrentCanvasIndex();
	u32 widget;
	if (localIndex < 0 || localIndex >= GAME_MAX_LOCALS || !SpectateData[localIndex].Enabled)
		return;
	for (widget = 0x60002; widget <= 0x60007; ++widget)
		hudSetFlags(widget, 1, 0);
}

static int installScoreboardHook(int localIndex)
{
	u32 render = GetAddress(&scoreboardRender);
	u32 getState;
	GuiState_t *state;

	/* hud_RenderScoreboard calls GetDeathmatchScoreHudState at +0x14.
	 * Decode the native JAL only after checking its surrounding instructions. */
	if (!render || *(u32 *)render != 0x27bdffe0 ||
		*(u32 *)(render + 0x04) != 0xffb00000 ||
		*(u32 *)(render + 0x14) >> 26 != 3 ||
		*(u32 *)(render + 0x18) != 0x00a0882d)
		return 0;
	getState = (*(u32 *)(render + 0x14) & 0x03ffffff) << 2;
	state = ((GuiState_t *(*)(int))getState)(localIndex);
	if (!state || (state->pre_render && state->pre_render != spectateHudPreRender))
		return 0;
	scoreboardStates[localIndex] = state;
	state->pre_render = spectateHudPreRender;
	return 1;
}

static void restoreScoreboardHooks(void)
{
	int i;
	for (i = 0; i < GAME_MAX_LOCALS; ++i) {
		GuiState_t *state = scoreboardStates[i];
		if (state && state->pre_render == spectateHudPreRender)
			state->pre_render = 0;
		scoreboardStates[i] = 0;
	}
}

static int showSpectateHelp(void)
{
	return uiShowTipPopup("Spectate: \x13 enter / exit\x01"
		"\x14 / \x15 switch players", 8);
}
/* Use Forge's verified post-update epilogue so UYA's death camera cannot
 * overwrite the interpolated spectate position before rendering. */
static void spectateCameraPost(void)
{
	int i;
	if (!isInGame())
		return;
	for (i = 0; i < GAME_MAX_LOCALS; ++i) {
		playerSpectateData_t *data = &SpectateData[i];
		GameCamera *camera;
		MATRIX matrix;
		VECTOR offset;
		if (!data->Enabled)
			continue;
		camera = cameraGetGameCamera(i);
		if (!camera)
			continue;
		vector_copy(camera->pos, data->LastCameraPos);
		camera->rot[0] = 0;
		camera->rot[1] = data->LastCameraY;
		camera->rot[2] = data->LastCameraZ;
		matrix_unit(matrix);
		matrix_rotate_y(matrix, matrix, data->LastCameraY);
		matrix_rotate_z(matrix, matrix, data->LastCameraZ);
		/* UYA stores an eye anchor in fps.cameraPos, unlike DL's final camera position. */
		vector_apply(offset, data->FollowOffset, matrix);
		vector_add(camera->pos, camera->pos, offset);
		memcpy(&camera->uMtx, matrix, sizeof(float) * 12);
		((void (*)(int))cameraRenderAddress)(i);
	}
}

static void restoreSpectateHook(void)
{
	restoreScoreboardHooks();
	if (cameraPostAddress && *(u32 *)cameraPostAddress == (0x08000000 | ((u32)spectateCameraPost >> 2)) &&
		*(u32 *)(cameraPostAddress + 4) == 0x27bd0080) {
		*(u32 *)cameraPostAddress = 0x03e00008;
		if (reticleCallAddress && *(u32 *)reticleCallAddress == 0)
			*(u32 *)reticleCallAddress = reticleOriginal;
		FlushCache(0);
		FlushCache(2);
	}
	cameraPostAddress = 0;
	cameraRenderAddress = 0;
	reticleCallAddress = 0;
	reticleOriginal = 0;
}

static int installSpectateHook(void)
{
	u32 render = GetAddress(&cameraRender);
	u32 address = render + 0x40c;
	u32 jump = 0x08000000 | ((u32)spectateCameraPost >> 2);
	u32 reticle = render + 0x15b0;
	u32 reticleCall = 0x0c000000 | ((render + 0xee0) >> 2);
	if (cameraPostAddress == address && *(u32 *)address == jump)
		return 1;
	if (!render || *(u32 *)render != 0x27bdffd0 ||
		*(u32 *)address != 0x03e00008 || *(u32 *)(address + 4) != 0x27bd0080 ||
		*(u32 *)reticle != reticleCall || *(u32 *)(render + 0xee0) != 0x27bdff90)
		return 0;
	cameraRenderAddress = render;
	cameraPostAddress = address;
	/* Forge verifies this shared reticle call across all regional overlays. */
	reticleCallAddress = reticle;
	reticleOriginal = reticleCall;
	*(u32 *)address = jump;
	*(u32 *)reticle = 0;
	FlushCache(0);
	FlushCache(2);
	return 1;
}

static void disableSpectate(playerSpectateData_t *data)
{
	data->Enabled = 0;
}

static void enableSpectate(playerSpectateData_t *data)
{
	data->Enabled = 1;
}
static void spectate(Player *currentPlayer, Player *playerToSpectate)
{
	float cameraT;
	playerSpectateData_t * spectateData = SpectateData + currentPlayer->fps.vars.cam_slot;
	if(!playerToSpectate)
		return;

	// Do not move UYA's native death-camera anchor: its collision sweep runs before our render hook.

	if (playerToSpectate->vehicle && playerToSpectate->vehicle->pMoby) {
		vector_write(spectateData->FollowOffset, 0);
		struct Moby * vehicleMoby = playerToSpectate->vehicle->pMoby;
		int isPassenger = (void *)playerToSpectate->vehicle->pPassenger == (void *)playerToSpectate;
		cameraT = 1 - powf(MATH_E, -(isPassenger ? CAMERA_ROTATION_SHARPNESS : VEHICLE_CAMERA_ROTATION_SHARPNESS) * MATH_DT);

		// Grab rotation
		float yaw = vehicleMoby->rotation[2];
		float pitch = 0.08;
		float distance = 0;
		float elevation = 0;

		// Determine distance and elevation by vehicle id
		switch (vehicleMoby->oClass) {
			case MOBY_ID_TURBOSLIDER: {
				distance = VEHICLE_DISTANCE[0 + isPassenger];
				elevation = VEHICLE_ELEVATION[0 + isPassenger];
				if (isPassenger) {
					yaw = playerToSpectate->fps.vars.cameraZ.rotation;
					pitch = playerToSpectate->fps.vars.cameraY.rotation + (float)0.08;
				}
				break;
			}
			case MOBY_ID_HOVERSHIP: {
				distance = VEHICLE_DISTANCE[2 + isPassenger];
				elevation = VEHICLE_ELEVATION[2 + isPassenger];
				if (isPassenger) {
					yaw = playerToSpectate->fps.vars.cameraZ.rotation;
					pitch = playerToSpectate->fps.vars.cameraY.rotation;
				}
				break;
			}
			case MOBY_ID_TANK: {
				distance = VEHICLE_DISTANCE[4 + isPassenger];
				elevation = VEHICLE_ELEVATION[4 + isPassenger];
				if (isPassenger) {
					yaw = playerToSpectate->fps.vars.cameraZ.rotation;
					pitch = playerToSpectate->fps.vars.cameraY.rotation + (float)0.08;
				}
				break;
			}
		}

		// Interpolate camera rotation towards target player
		float lerpZ = lerpfAngle(spectateData->LastCameraZ, yaw, cameraT);
		spectateData->LastCameraZ = lerpZ;
		float lerpY = lerpfAngle(spectateData->LastCameraY, pitch, cameraT);
		spectateData->LastCameraY = lerpY;

		// Generate target based off distance and elevation
		VECTOR target;
		vector_copy(target, playerToSpectate->vehicle->pMoby->position);
		target[0] -= cosf(spectateData->LastCameraZ) * distance;
		target[1] -= sinf(spectateData->LastCameraZ) * distance;
		target[2] += (sinf(spectateData->LastCameraY) * distance) + elevation;

		// Interpolate camera towards target player
		vector_lerp(spectateData->LastCameraPos, spectateData->LastCameraPos, target, 1 - powf(MATH_E, -CAMERA_POSITION_SHARPNESS * MATH_DT));
	} else {
		vector_copy(spectateData->FollowOffset, playerToSpectate->fps.vars.positionOffset);
		cameraT = 1 - powf(MATH_E, -CAMERA_ROTATION_SHARPNESS * MATH_DT);

		// Interpolate camera rotation towards target player
		float playerToSpectateZ = lerpfAngle(spectateData->LastCameraZ, playerToSpectate->fps.vars.cameraZ.rotation, cameraT);
		spectateData->LastCameraZ = playerToSpectateZ;
		float playerToSpectateY = lerpfAngle(spectateData->LastCameraY, playerToSpectate->fps.vars.cameraY.rotation, cameraT);
		spectateData->LastCameraY = playerToSpectateY;

		// Interpolate camera towards target player
		vector_lerp(spectateData->LastCameraPos, spectateData->LastCameraPos, playerToSpectate->fps.cameraPos, 1 - powf(MATH_E, -CAMERA_POSITION_SHARPNESS * MATH_DT));
	}
}

static int findNextPlayerIndex(int currentPlayerIndex, int currentSpectateIndex, int direction)
{
	Player **players = playerGetAll();
	int index = currentSpectateIndex;
	int count;

	/* A bounded search also handles an empty roster or a disconnected target. */
	for (count = 0; count < GAME_MAX_PLAYERS; ++count) {
		index = (index + direction + GAME_MAX_PLAYERS) % GAME_MAX_PLAYERS;
		if (index != currentPlayerIndex && players[index])
			return index;
	}
	return -1;
}

static Player *spectateLocalPlayer(int localIndex)
{
	Player **players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
		Player *player = players[i];
		if (player && player->isLocal && player->pPad && player->pPad->port != 10 &&
			player->fps.vars.cam_slot == localIndex)
			return player;
	}
	return 0;
}

void runSpectate(void)
{
	Player **players;
	int localIndex;

	uiUpdateTipPopup();

	if (!gameGetSettings() || !isInGame()) {
		restoreSpectateHook();
		memset(SpectateData, 0, sizeof(SpectateData));
		return;
	}

	players = playerGetAll();
	for (localIndex = 0; localIndex < GAME_MAX_LOCALS; ++localIndex) {
		Player *player = spectateLocalPlayer(localIndex);
		playerSpectateData_t *data = &SpectateData[localIndex];
		int direction = 0;
		int index = data->Index;

		/* Simulated heroes have pads too; only actual local slots own a camera. */
		if (!player) {
			disableSpectate(data);
			continue;
		}
		if (!playerIsDead(player)) {
			disableSpectate(data);
			continue;
		}

		if (!data->HasShownHelp) {
			data->HasShownHelp = showSpectateHelp();
		}
		if (playerPadGetButtonDown(player, PAD_SQUARE) > 0) {
			if (data->Enabled) {
				disableSpectate(data);
				continue;
			}
			index = findNextPlayerIndex(player->mpIndex, player->mpIndex, 1);
			if (index < 0 || !installSpectateHook() || !installScoreboardHook(localIndex))
				continue;
			enableSpectate(data);
			data->Index = -1;
		}
		if (!data->Enabled)
			continue;

		if (index < 0 || index >= GAME_MAX_PLAYERS || !players[index])
			direction = 1;
		else if (playerPadGetButtonDown(player, PAD_R1) > 0)
			direction = 1;
		else if (playerPadGetButtonDown(player, PAD_L1) > 0)
			direction = -1;

		if (direction)
			index = findNextPlayerIndex(player->mpIndex, index, direction);
		if (index < 0) {
			disableSpectate(data);
			continue;
		}
		if (index != data->Index) {
			Vehicle *vehicle = players[index]->vehicle;
			if (vehicle && vehicle->pMoby)
				vector_copy(data->LastCameraPos, vehicle->pMoby->position);
			else
				vector_copy(data->LastCameraPos, players[index]->fps.cameraPos);
			data->LastCameraZ = players[index]->fps.vars.cameraZ.rotation;
			data->LastCameraY = players[index]->fps.vars.cameraY.rotation;
		}
		data->Index = index;
		spectate(player, players[index]);
	}
	for (localIndex = 0; localIndex < GAME_MAX_LOCALS; ++localIndex) {
		if (SpectateData[localIndex].Enabled)
			return;
	}
	restoreSpectateHook();
}
