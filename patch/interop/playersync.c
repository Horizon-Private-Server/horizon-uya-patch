#include <libuya/interop.h>
#include "interop/playersync.h"

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

VariableAddress_t vaHandlePlayerPad_Hook = {
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
	.BlackwaterCity = 0x00518834,
	.CommandCenter = 0x005187b4,
	.BlackwaterDocks = 0x0051aff4,
	.AquatosSewers = 0x0051a334,
	.MarcadiaPalace = 0x00519c74,
#endif
};

VariableAddress_t vaTransAnim_Hook = {
#if UYA_PAL
	.Lobby = 0x0062b4f4,
	.Bakisi = 0x004fd3ec,
	.Hoven = 0x004ff504,
	.OutpostX12 = 0x004f4ddc,
	.KorgonOutpost = 0x004f2574,
	.Metropolis = 0x004f18c4,
	.BlackwaterCity = 0x004ef15c,
	.CommandCenter = 0x004ef124,
	.BlackwaterDocks = 0x004f19a4,
	.AquatosSewers = 0x004f0ca4,
	.MarcadiaPalace = 0x004f0624,
#else
	.Lobby = 0x00628d1c,
	.Bakisi = 0x004fac6c,
	.Hoven = 0x004fccc4,
	.OutpostX12 = 0x004f25dc,
	.KorgonOutpost = 0x004efdf4,
	.Metropolis = 0x004ef144,
	.BlackwaterCity = 0x004ec95c,
	.CommandCenter = 0x004ecae4,
	.BlackwaterDocks = 0x004ef324,
	.AquatosSewers = 0x004ee664,
	.MarcadiaPalace = 0x004edfa4,
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
	.KorgonOutpost = 0x0052641c,
	.Metropolis = 0x0052576c,
	.BlackwaterCity = 0x00522f84,
	.CommandCenter = 0x00522f04,
	.BlackwaterDocks = 0x00525744,
	.AquatosSewers = 0x00524a84,
	.MarcadiaPalace = 0x005243c4,
#endif
};

