#include <libuya/interop.h>
#include "interop/playersync.h"

VariableAddress_t vaProcessGadgetState_Func = {
#if UYA_PAL
	.Lobby = 0x006439e0,
	.Bakisi = 0x005158d8,
	.Hoven = 0x005179f0,
	.OutpostX12 = 0x0050d2c8,
	.KorgonOutpost = 0x0050aa60,
	.Metropolis = 0x00509db0,
	.BlackwaterCity = 0x00507648,
	.CommandCenter = 0x00507610,
	.BlackwaterDocks = 0x00509e90,
	.AquatosSewers = 0x00509190,
	.MarcadiaPalace = 0x00508b10,
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
	.Lobby = 0x0063ee78,
	.Bakisi = 0x00510d70,
	.Hoven = 0x00512e88,
	.OutpostX12 = 0x00508760,
	.KorgonOutpost = 0x00505ef8,
	.Metropolis = 0x00505248,
	.BlackwaterCity = 0x00502ae0,
	.CommandCenter = 0x00502aa8,
	.BlackwaterDocks = 0x00505328,
	.AquatosSewers = 0x00504628,
	.MarcadiaPalace = 0x00503fa8,
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
	.Lobby = 0x006640a8,
	.Bakisi = 0x005361a8,
	.Hoven = 0x005382c0,
	.OutpostX12 = 0x0052db98,
	.KorgonOutpost = 0x0052b330,
	.Metropolis = 0x0052a680,
	.BlackwaterCity = 0x00527f18,
	.CommandCenter = 0x00527cd8,
	.BlackwaterDocks = 0x0052a558,
	.AquatosSewers = 0x00529858,
	.MarcadiaPalace = 0x005291d8,
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
	.Bakisi = 0x00424d20,
	.Hoven = 0x00426778,
	.OutpostX12 = 0x0041d6a8,
	.KorgonOutpost = 0x0041ad18,
	.Metropolis = 0x0041a5a8,
	.BlackwaterCity = 0x00416410,
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
	.Lobby = 0x0063fdf4,
	.Bakisi = 0x00511cec,
	.Hoven = 0x00513e04,
	.OutpostX12 = 0x005096dc,
	.KorgonOutpost = 0x00506e74,
	.Metropolis = 0x005061c4,
	.BlackwaterCity = 0x00503a5c,
	.CommandCenter = 0x00503a24,
	.BlackwaterDocks = 0x005062a4,
	.AquatosSewers = 0x005055a4,
	.MarcadiaPalace = 0x00504f24,
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

VariableAddress_t vaRemoteProcessPadInput_Addr = {
#if UYA_PAL
	.Lobby = 0x006637a4,
	.Bakisi = 0x005358a4,
	.Hoven = 0x005379bc,
	.OutpostX12 = 0x0052d294,
	.KorgonOutpost = 0x0052aa2c,
	.Metropolis = 0x00529d7c,
	.BlackwaterCity = 0x00527614,
	.CommandCenter = 0x005273d4,
	.BlackwaterDocks = 0x00529c54,
	.AquatosSewers = 0x00528f54,
	.MarcadiaPalace = 0x005288d4,
#else
	.Lobby = 0x00660ed8,
	.Bakisi = 0x00533030,
	.Hoven = 0x00535088,
	.OutpostX12 = 0x0052a9a0,
	.KorgonOutpost = 0x005281b8,
	.Metropolis = 0x00527508,
	.BlackwaterCity = 0x00524d20,
	.CommandCenter = 0x00524ca0,
	.BlackwaterDocks = 0x005274e0,
	.AquatosSewers = 0x00526820,
	.MarcadiaPalace = 0x00526160,
#endif
};

VariableAddress_t vaUpdatePadAddr = {
#if UYA_PAL
	.Lobby = 0x005c3ea0,
	.Bakisi = 0x00496658,
	.Hoven = 0x00498770,
	.OutpostX12 = 0x0048e048,
	.KorgonOutpost = 0x0048b718,
	.Metropolis = 0x0048ab30,
	.BlackwaterCity = 0x004883c8,
	.CommandCenter = 0x004883c0,
	.BlackwaterDocks = 0x0048ac40,
	.AquatosSewers = 0x00489f40,
	.MarcadiaPalace = 0x004898c0,
#else
	.Lobby = 0x005c1c50,
	.Bakisi = 0x00494460,
	.Hoven = 0x004964b8,
	.OutpostX12 = 0x0048bdd0,
	.KorgonOutpost = 0x00489520,
	.Metropolis = 0x00488938,
	.BlackwaterCity = 0x00486150,
	.CommandCenter = 0x00486308,
	.BlackwaterDocks = 0x00488b48,
	.AquatosSewers = 0x00487e88,
	.MarcadiaPalace = 0x004877c8,
#endif
};

VariableAddress_t vaProcessPadInputAddr = {
#if UYA_PAL
	.Lobby = 0x005c34a8,
	.Bakisi = 0x00495c60,
	.Hoven = 0x00497d78,
	.OutpostX12 = 0x0048d650,
	.KorgonOutpost = 0x0048ad20,
	.Metropolis = 0x0048a138,
	.BlackwaterCity = 0x004879d0,
	.CommandCenter = 0x004879c8,
	.BlackwaterDocks = 0x0048a248,
	.AquatosSewers = 0x00489548,
	.MarcadiaPalace = 0x00488ec8,
#else
	.Lobby = 0x005c1258,
	.Bakisi = 0x00493a68,
	.Hoven = 0x00495ac0,
	.OutpostX12 = 0x0048b3d8,
	.KorgonOutpost = 0x00488b28,
	.Metropolis = 0x00487f40,
	.BlackwaterCity = 0x00485758,
	.CommandCenter = 0x00485910,
	.BlackwaterDocks = 0x00488150,
	.AquatosSewers = 0x00487490,
	.MarcadiaPalace = 0x00486dd0,
#endif
};

VariableAddress_t vaGadgetTransitions_Func = {
#if UYA_PAL
	.Lobby = 0x0065d0a0,
	.Bakisi = 0x0052f1a0,
	.Hoven = 0x005312b8,
	.OutpostX12 = 0x00526b90,
	.KorgonOutpost = 0x00524328,
	.Metropolis = 0x00523678,
	.BlackwaterCity = 0x00520f10,
	.CommandCenter = 0x00520cd0,
	.BlackwaterDocks = 0x00523550,
	.AquatosSewers = 0x00522850,
	.MarcadiaPalace = 0x005221d0,
#else
	.Lobby = 0x0065a7c8,
	.Bakisi = 0x0052c920,
	.Hoven = 0x0052e978,
	.OutpostX12 = 0x00524290,
	.KorgonOutpost = 0x00521aa8,
	.Metropolis = 0x00520df8,
	.BlackwaterCity = 0x0051e610,
	.CommandCenter = 0x0051e590,
	.BlackwaterDocks = 0x00520dd0,
	.AquatosSewers = 0x00520110,
	.MarcadiaPalace = 0x0051fa50,
#endif
};

VariableAddress_t vaGadgetTransitions_Hook = {
#if UYA_PAL
	.Lobby = 0x006572c4,
	.Bakisi = 0x005293c4,
	.Hoven = 0x0052b4dc,
	.OutpostX12 = 0x00520db4,
	.KorgonOutpost = 0x0051e54c,
	.Metropolis = 0x0051d89c,
	.BlackwaterCity = 0x0051b134,
	.CommandCenter = 0x0051aef4,
	.BlackwaterDocks = 0x0051d774,
	.AquatosSewers = 0x0051ca74,
	.MarcadiaPalace = 0x0051c3f4,
#else
	.Lobby = 0x006549ec,
	.Bakisi = 0x00526b44,
	.Hoven = 0x00528b9c,
	.OutpostX12 = 0x0051e4b4,
	.KorgonOutpost = 0x0051bccc,
	.Metropolis = 0x0051b01c,
	.BlackwaterCity = 0x00518834,
	.CommandCenter = 0x005187b4,
	.BlackwaterDocks = 0x0051aff4,
	.AquatosSewers = 0x0051a334,
	.MarcadiaPalace = 0x00519d8c,
#endif
};

