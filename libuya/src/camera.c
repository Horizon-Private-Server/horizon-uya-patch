#include "string.h"
#include "player.h"
#include "team.h"
#include "camera.h"
#include "interop.h"

#define CAMERA								((Camera_t*)GetAddress(&vaCAMERA));
#define GAME_CAMERA                         ((GameCamera*)GetAddress(&vaGAME_CAMERA))

VariableAddress_t vaGAME_CAMERA = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00249fc0,
	.Hoven = 0x0024a100,
	.OutpostX12 = 0x0024a000,
    .KorgonOutpost = 0x00249e80,
	.Metropolis = 0x00249ec0,
	.BlackwaterCity = 0x00249e80,
	.CommandCenter = 0x00249a40,
    .BlackwaterDocks = 0x00249b80,
    .AquatosSewers = 0x00249bc0,
    .MarcadiaPalace = 0x00249b40,
#else
	.Lobby = 0,
	.Bakisi = 0x0024a140,
	.Hoven = 0x0024a280,
	.OutpostX12 = 0x0024a180,
    .KorgonOutpost = 0x0024a000,
	.Metropolis = 0x0024a040,
	.BlackwaterCity = 0x0024a000,
	.CommandCenter = 0x00249bc0,
    .BlackwaterDocks = 0x00249d00,
    .AquatosSewers = 0x00249d40,
    .MarcadiaPalace = 0x00249cc0,
#endif
};

VariableAddress_t vaCAMERA = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0,
	.Hoven = 0,
	.OutpostX12 = 0,
	.KorgonOutpost = 0,
	.Metropolis = 0,
	.BlackwaterCity = 0,
	.CommandCenter = 0,
	.BlackwaterDocks = 0,
	.AquatosSewers = 0,
	.MarcadiaPalace = 0,
#else
	.Lobby = 0,
	.Bakisi = 0x00249f80,
	.Hoven = 0,
	.OutpostX12 = 0,
	.KorgonOutpost = 0,
	.Metropolis = 0,
	.BlackwaterCity = 0,
	.CommandCenter = 0,
	.BlackwaterDocks = 0,
	.AquatosSewers = 0,
	.MarcadiaPalace = 0,
#endif
};

//--------------------------------------------------------------------------------
Camera_t *cameraGetCamera()
{
	return CAMERA;
}

GameCamera* cameraGetGameCamera(int cameraIndex)
{
    return GAME_CAMERA + cameraIndex;
}
