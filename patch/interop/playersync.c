#include <libuya/interop.h>
#include "interop/playersync.h"

VariableAddress_t vaProcessGadgetState_Func = {
#if UYA_PAL
	.Lobby = 0x00641170,
	.Bakisi = 0x005130c0,
	.Hoven = 0x00515118,
	.OutpostX12 = 0x0050aa30,
	.KorgonOutpost = 0x00508248,
	.Metropolis = 0x00507598,
	.BlackwaterCity = 0x00504db0,
	.CommandCenter = 0x00504f38,
	.BlackwaterDocks = 0x00507778,
	.AquatosSewers = 0x00506ab8,
	.MarcadiaPalace = 0x0164b874,
#else
	.Lobby = 0x00641170,
	.Bakisi = 0x005130c0,
	.Hoven = 0x00515118,
	.OutpostX12 = 0x0050aa30,
	.KorgonOutpost = 0x00508248,
	.Metropolis = 0x00507598,
	.BlackwaterCity = 0x00504db0,
	.CommandCenter = 0x00504f38,
	.BlackwaterDocks = 0x00507778,
	.AquatosSewers = 0x00506ab8,
	.MarcadiaPalace = 0x0164b874,
#endif
};

VariableAddress_t vaProcessGadget_Func = {
#if UYA_PAL
	.Lobby = 0x0063c608,
	.Bakisi = 0x0050e558,
	.Hoven = 0x005105b0,
	.OutpostX12 = 0x00505ec8,
	.KorgonOutpost = 0x005036e0,
	.Metropolis = 0x00502a30,
	.BlackwaterCity = 0x00500248,
	.CommandCenter = 0x005003d0,
	.BlackwaterDocks = 0x00502c10,
	.AquatosSewers = 0x00501f50,
	.MarcadiaPalace = 0x00501890,
#else
	.Lobby = 0x0063c608,
	.Bakisi = 0x0050e558,
	.Hoven = 0x005105b0,
	.OutpostX12 = 0x00505ec8,
	.KorgonOutpost = 0x005036e0,
	.Metropolis = 0x00502a30,
	.BlackwaterCity = 0x00500248,
	.CommandCenter = 0x005003d0,
	.BlackwaterDocks = 0x00502c10,
	.AquatosSewers = 0x00501f50,
	.MarcadiaPalace = 0x00501890,
#endif
};


VariableAddress_t vaPlayerStateUpdate_Hook = {
#if UYA_PAL
	.Lobby = 0x0065fd34,
	.Bakisi = 0x00531e34,
	.Hoven = 0x00533f4c,
	.OutpostX12 = 0x00529824,
	.KorgonOutpost = 0x00526fbc,
	.Metropolis = 0x0052630c,
	.BlackwaterCity = 0x00523ba4,
	.CommandCenter = 0x00523964,
	.BlackwaterDocks = 0x005261e4,
	.AquatosSewers = 0x005254e4,
	.MarcadiaPalace = 0x00524e64,
#else
	.Lobby = 0x0065d45c,
	.Bakisi = 0x0052f5b4,
	.Hoven = 0x0053160c,
	.OutpostX12 = 0x00526f24,
	.KorgonOutpost = 0x0052473c,
	.Metropolis = 0x00523a8c,
	.BlackwaterCity = 0x005212a4,
	.CommandCenter = 0x00521224,
	.BlackwaterDocks = 0x00523a64,
	.AquatosSewers = 0x00522da4,
	.MarcadiaPalace = 0x005226e4,
#endif
};

VariableAddress_t vaPlayerPnetUpdates_Hook = {
#if UYA_PAL
	.Lobby = 0x006617d0,
	.Bakisi = 0x00533928,
	.Hoven = 0x00535980,
	.OutpostX12 = 0x0052b298,
	.KorgonOutpost = 0x00528ab0,
	.Metropolis = 0x00527e00,
	.BlackwaterCity = 0x00525618,
	.CommandCenter = 0x00525598,
	.BlackwaterDocks = 0x00527dd8,
	.AquatosSewers = 0x00527118,
	.MarcadiaPalace = 0x00526a58,
#else
	.Lobby = 0x006617d0,
	.Bakisi = 0x00533928,
	.Hoven = 0x00535980,
	.OutpostX12 = 0x0052b298,
	.KorgonOutpost = 0x00528ab0,
	.Metropolis = 0x00527e00,
	.BlackwaterCity = 0x00525618,
	.CommandCenter = 0x00525598,
	.BlackwaterDocks = 0x00527dd8,
	.AquatosSewers = 0x00527118,
	.MarcadiaPalace = 0x00526a58,
#endif
};

VariableAddress_t vaEnableTurretSync_Addr = {
#if UYA_PAL
  .Lobby = 0,
	.Bakisi = 0x00424428,
	.Hoven = 0x00425db8,
	.OutpostX12 = 0x0041cd00,
	.KorgonOutpost = 0x0041a418,
	.Metropolis = 0x00419cb0,
	.BlackwaterCity = 0x00415ad0,
	.CommandCenter = 0,
	.BlackwaterDocks = 0,
	.AquatosSewers = 0,
	.MarcadiaPalace = 0,
#else
  .Lobby = 0,
	.Bakisi = 0x00424428,
	.Hoven = 0x00425db8,
	.OutpostX12 = 0x0041cd00,
	.KorgonOutpost = 0x0041a418,
	.Metropolis = 0x00419cb0,
	.BlackwaterCity = 0x00415ad0,
	.CommandCenter = 0,
	.BlackwaterDocks = 0,
	.AquatosSewers = 0,
	.MarcadiaPalace = 0,
#endif
};

VariableAddress_t vaTNW_PlayerData_GadgetIdUpdate = {
#if UYA_PAL
	.Lobby = 0x00660314,
	.Bakisi = 0x00532414,
	.Hoven = 0x0053452c,
	.OutpostX12 = 0x00529e04,
	.KorgonOutpost = 0x0052759c,
	.Metropolis = 0x005268ec,
	.BlackwaterCity = 0x00524184,
	.CommandCenter = 0x00523f44,
	.BlackwaterDocks = 0x005267c4,
	.AquatosSewers = 0x00525ac4,
	.MarcadiaPalace = 0x00525444,
#else
	.Lobby = 0x0065da3c,
	.Bakisi = 0x0052fb94,
	.Hoven = 0x00531bec,
	.OutpostX12 = 0x00527504,
	.KorgonOutpost = 0x00524d1c,
	.Metropolis = 0x0052406c,
	.BlackwaterCity = 0x00521884,
	.CommandCenter = 0x00521804,
	.BlackwaterDocks = 0x00524044,
	.AquatosSewers = 0x00523384,
	.MarcadiaPalace = 0x00522cc4,
#endif
};

VariableAddress_t vaTNW_PlayerData_TimeUpdate = {
#if UYA_PAL
	.Lobby = 0x00661a14,
	.Bakisi = 0x00533b14,
	.Hoven = 0x00535c2c,
	.OutpostX12 = 0x0052b504,
	.KorgonOutpost = 0x00528c9c,
	.Metropolis = 0x00527fec,
	.BlackwaterCity = 0x00525884,
	.CommandCenter = 0x00525644,
	.BlackwaterDocks = 0x00527ec4,
	.AquatosSewers = 0x005271c4,
	.MarcadiaPalace = 0x00526b44,
#else
	.Lobby = 0x0065f13c,
	.Bakisi = 0x00531294,
	.Hoven = 0x005332ec,
	.OutpostX12 = 0x00528c04,
	.KorgonOutpost = 0x0052641c,
	.Metropolis = 0x0052576c,
	.BlackwaterCity = 0x00522f84,
	.CommandCenter = 0x00522f04,
	.BlackwaterDocks = 0x00525744,
	.AquatosSewers = 0x00524a84,
	.MarcadiaPalace = 0x005243c4,
#endif
};

VariableAddress_t vaForceGadgetMobyCreation_Addr = {
#if UYA_PAL
	.Lobby = 0x0063d584,
	.Bakisi = 0x0050f4d4,
	.Hoven = 0x0051152c,
	.OutpostX12 = 0x00506e44,
	.KorgonOutpost = 0x0050465c,
	.Metropolis = 0x005039ac,
	.BlackwaterCity = 0x005011c4,
	.CommandCenter = 0x0050134c,
	.BlackwaterDocks = 0x00503b8c,
	.AquatosSewers = 0x00502ecc,
	.MarcadiaPalace = 0x0050280c,
#else
	.Lobby = 0x0063d584,
	.Bakisi = 0x0050f4d4,
	.Hoven = 0x0051152c,
	.OutpostX12 = 0x00506e44,
	.KorgonOutpost = 0x0050465c,
	.Metropolis = 0x005039ac,
	.BlackwaterCity = 0x005011c4,
	.CommandCenter = 0x0050134c,
	.BlackwaterDocks = 0x00503b8c,
	.AquatosSewers = 0x00502ecc,
	.MarcadiaPalace = 0x0050280c,
#endif
};