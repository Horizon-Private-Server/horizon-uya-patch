#include <libuya/interop.h>

//================================================
//=============   runCameraSpeedPatch
//================================================
VariableAddress_t vaUpdateCameraSpeedIGFunc = {
#ifdef UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004be190,
    .Hoven = 0x004c02a8,
    .OutpostX12 = 0x004b5b80,
    .KorgonOutpost = 0x004b3318,
    .Metropolis = 0x004b2668,
    .BlackwaterCity = 0x004aff00,
    .CommandCenter = 0x004afef8,
    .BlackwaterDocks = 0x004b2778,
    .AquatosSewers = 0x004b1a78,
    .MarcadiaPalace = 0x004b13f8,
#else
    .Lobby = 0,
    .Bakisi = 0x004BBC78,
    .Hoven = 0x004BDCD0,
    .OutpostX12 = 0x004B35E8,
    .KorgonOutpost = 0x004B0E00,
    .Metropolis = 0x004B0150,
    .BlackwaterCity = 0x004AD968,
    .CommandCenter = 0x004ADB20,
    .BlackwaterDocks = 0x004B0360,
    .AquatosSewers = 0x004AF6A0,
    .MarcadiaPalace = 0x004AEFE0,
#endif
};
VariableAddress_t vaDrawCameraSpeedInputIGFunc = {
#ifdef UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004be3a4,
    .Hoven = 0x004c04bc,
    .OutpostX12 = 0x004b5d94,
    .KorgonOutpost = 0x004b352c,
    .Metropolis = 0x004b287c,
    .BlackwaterCity = 0x004b0114,
    .CommandCenter = 0x004b010c,
    .BlackwaterDocks = 0x004b298c,
    .AquatosSewers = 0x004b1c8c,
    .MarcadiaPalace = 0x004b160c,
#else
    .Lobby = 0,
    .Bakisi = 0x004BBE8C,
    .Hoven = 0x004BDEE4,
    .OutpostX12 = 0x004B37FC,
    .KorgonOutpost = 0x004B1014,
    .Metropolis = 0x004B0364,
    .BlackwaterCity = 0x004ADB7C,
    .CommandCenter = 0x004ADD34,
    .BlackwaterDocks = 0x004B0574,
    .AquatosSewers = 0x004AF8B4,
    .MarcadiaPalace = 0x004AF1F4,
#endif
};

//================================================
//=============   patchKillStealing
//================================================
VariableAddress_t vaWhoHitMeFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004f8890,
    .Hoven = 0x004fa9a8,
    .OutpostX12 = 0x004f0280,
    .KorgonOutpost = 0x004eda18,
    .Metropolis = 0x004ecd68,
    .BlackwaterCity = 0x004ea600,
    .CommandCenter = 0x004ea5c8,
    .BlackwaterDocks = 0x004ece48,
    .AquatosSewers = 0x004ec148,
    .MarcadiaPalace = 0x004ebac8,
#else
    .Lobby = 0,
    .Bakisi = 0x004f6110,
    .Hoven = 0x004f8168,
    .OutpostX12 = 0x004eda80,
    .KorgonOutpost = 0x004eb298,
    .Metropolis = 0x004ea5e8,
    .BlackwaterCity = 0x004e7e00,
    .CommandCenter = 0x004e7f88,
    .BlackwaterDocks = 0x004ea7c8,
    .AquatosSewers = 0x004e9b08,
    .MarcadiaPalace = 0x004e9448,
#endif
};
VariableAddress_t vaWhoHitMeHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004f95d8,
    .Hoven = 0x004fb6f0,
    .OutpostX12 = 0x004f0fc8,
    .KorgonOutpost = 0x004ee760,
    .Metropolis = 0x004edab0,
    .BlackwaterCity = 0x004eb348,
    .CommandCenter = 0x004eb310,
    .BlackwaterDocks = 0x004edb90,
    .AquatosSewers = 0x004ece90,
    .MarcadiaPalace = 0x004ec810,
#else
    .Lobby = 0,
    .Bakisi = 0x004f6e58,
    .Hoven = 0x004f8eb0,
    .OutpostX12 = 0x004ee7C8,
    .KorgonOutpost = 0x004ebfe0,
    .Metropolis = 0x004eb330,
    .BlackwaterCity = 0x004e8b48,
    .CommandCenter = 0x004e8cd0,
    .BlackwaterDocks = 0x004eb510,
    .AquatosSewers = 0x004ea850,
    .MarcadiaPalace = 0x004ea190,
#endif
};

//================================================
//=============   patchDeadShooting
//================================================
VariableAddress_t vaPatchDeadShooting_ShootingFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00495708,
    .Hoven = 0x00497820,
    .OutpostX12 = 0x0048d0f8,
    .KorgonOutpost = 0x0048a7c8,
    .Metropolis = 0x00489be0,
    .BlackwaterCity = 0x00487478,
    .CommandCenter = 0x00487470,
    .BlackwaterDocks = 0x00489cf0,
    .AquatosSewers = 0x00488ff0,
    .MarcadiaPalace = 0x00488970,
#else
    .Lobby = 0,
    .Bakisi = 0x00493718,
    .Hoven = 0x00495770,
    .OutpostX12 = 0x0048b088,
    .KorgonOutpost = 0x004887d8,
    .Metropolis = 0x00487bf0,
    .BlackwaterCity = 0x00485408,
    .CommandCenter = 0x004855c0,
    .BlackwaterDocks = 0x00487e00,
    .AquatosSewers = 0x00487140,
    .MarcadiaPalace = 0x00486a80,
#endif
};
VariableAddress_t vaPatchDeadShooting_ShootingHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00532a0c,
    .Hoven = 0x00534b24,
    .OutpostX12 = 0x0052a3fc,
    .KorgonOutpost = 0x00527b94,
    .Metropolis = 0x00526ee4,
    .BlackwaterCity = 0x0052477c,
    .CommandCenter = 0x0052453c,
    .BlackwaterDocks = 0x00526dbc,
    .AquatosSewers = 0x005260bc,
    .MarcadiaPalace = 0x00525a3c,
#else
    .Lobby = 0,
    .Bakisi = 0x0053018c,
    .Hoven = 0x005321e4,
    .OutpostX12 = 0x00527afc,
    .KorgonOutpost = 0x00525314,
    .Metropolis = 0x00524664,
    .BlackwaterCity = 0x00521e7c,
    .CommandCenter = 0x00521dfc,
    .BlackwaterDocks = 0x0052463c,
    .AquatosSewers = 0x0052397c,
    .MarcadiaPalace = 0x005232bc,
#endif
};

//================================================
//=============   patchSniperWallSniping
//================================================
VariableAddress_t vaSniperShotCollLineFixHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00408bc0,
    .Hoven = 0x00408528,
    .OutpostX12 = 0x00400420,
    .KorgonOutpost = 0x003ff800,
    .Metropolis = 0x003fe3c0,
    .BlackwaterCity = 0x003fb1a8,
    .CommandCenter = 0x004095b0,
    .BlackwaterDocks = 0x0040b510,
    .AquatosSewers = 0x0040b118,
    .MarcadiaPalace = 0x0040a190,
#else
    .Lobby = 0,
    .Bakisi = 0x00408540,
    .Hoven = 0x00407e28,
    .OutpostX12 = 0x003ffd20,
    .KorgonOutpost = 0x003ff160,
    .Metropolis = 0x003fdd40,
    .BlackwaterCity = 0x003faac8,
    .CommandCenter = 0x00408f18,
    .BlackwaterDocks = 0x0040ae78,
    .AquatosSewers = 0x0040aa80,
    .MarcadiaPalace = 0x00409af8,
#endif
};
VariableAddress_t vaSniperShotCreatedHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00408508,
    .Hoven = 0x00407e70,
    .OutpostX12 = 0x003ffd68,
    .KorgonOutpost = 0x003ff148,
    .Metropolis = 0x003fdd08,
    .BlackwaterCity = 0x003faaf0,
    .CommandCenter = 0x00408ef8,
    .BlackwaterDocks = 0x0040ae58,
    .AquatosSewers = 0x0040aa60,
    .MarcadiaPalace = 0x00409ad8,
#else
    .Lobby = 0,
    .Bakisi = 0x00407e90,
    .Hoven = 0x00407778,
    .OutpostX12 = 0x003ff670,
    .KorgonOutpost = 0x003feab0,
    .Metropolis = 0x003fd690,
    .BlackwaterCity = 0x003fa418,
    .CommandCenter = 0x00408868,
    .BlackwaterDocks = 0x0040a7c8,
    .AquatosSewers = 0x0040a3d0,
    .MarcadiaPalace = 0x00409448,
#endif
};

//================================================
//=============  patchSniperNiking
//================================================
VariableAddress_t vaGetSniperShotDirectionFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0045d9d0,
    .Hoven = 0x0045f580,
    .OutpostX12 = 0x00456380,
    .KorgonOutpost = 0x00453f10,
    .Metropolis = 0x00453250,
    .BlackwaterCity = 0x00450a80,
    .CommandCenter = 0x00451378,
    .BlackwaterDocks = 0x00453bf8,
    .AquatosSewers = 0x00452ef8,
    .MarcadiaPalace = 0x00452878,
#else
    .Lobby = 0,
    .Bakisi = 0x0045c920,
    .Hoven = 0x0045e410,
    .OutpostX12 = 0x00455250,
    .KorgonOutpost = 0x00452e60,
    .Metropolis = 0x004521a0,
    .BlackwaterCity = 0x0044f950,
    .CommandCenter = 0x00450408,
    .BlackwaterDocks = 0x00452c48,
    .AquatosSewers = 0x00451f88,
    .MarcadiaPalace = 0x004518c8,
#endif
};
VariableAddress_t vaGetSniperShotDirectionHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00407394,
    .Hoven = 0x00406cfc,
    .OutpostX12 = 0x003febf4,
    .KorgonOutpost = 0x003fdfd4,
    .Metropolis = 0x003fcb94,
    .BlackwaterCity = 0x003f997c,
    .CommandCenter = 0x00407d84,
    .BlackwaterDocks = 0x00409ce4,
    .AquatosSewers = 0x004098ec,
    .MarcadiaPalace = 0x00408964,
#else
    .Lobby = 0,
    .Bakisi = 0x00406d2c,
    .Hoven = 0x00406614,
    .OutpostX12 = 0x003fe50c,
    .KorgonOutpost = 0x003fd94c,
    .Metropolis = 0x003fc52c,
    .BlackwaterCity = 0x003f92b4,
    .CommandCenter = 0x00407704,
    .BlackwaterDocks = 0x00409664,
    .AquatosSewers = 0x0040926c,
    .MarcadiaPalace = 0x004082e4,
#endif
};

//================================================
//=============  patchWeaponShotLag
//================================================
VariableAddress_t vaAllWeaponsUDPtoTCP = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00546760,
    .Hoven = 0x00548928,
    .OutpostX12 = 0x0053e200,
    .KorgonOutpost = 0x0053b8e8,
    .Metropolis = 0x0053ace8,
    .BlackwaterCity = 0x005384d0,
    .CommandCenter = 0x00537d28,
    .BlackwaterDocks = 0x0053a5a8,
    .AquatosSewers = 0x005398a8,
    .MarcadiaPalace = 0x00539228,
#else
    .Lobby = 0,
    .Bakisi = 0x00543e54,
    .Hoven = 0x00545f5c,
    .OutpostX12 = 0x0053b874,
    .KorgonOutpost = 0x00538fdc,
    .Metropolis = 0x005383dc,
    .BlackwaterCity = 0x00535b44,
    .CommandCenter = 0x00535574,
    .BlackwaterDocks = 0x00537db4,
    .AquatosSewers = 0x005370f4,
    .MarcadiaPalace = 0x00536a34,
#endif
};
VariableAddress_t vaFluxUDPtoTCP = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0040931c,
    .Hoven = 0x00408c84,
    .OutpostX12 = 0x00400b7c,
    .KorgonOutpost = 0x003fff5c,
    .Metropolis = 0x003feb1c,
    .BlackwaterCity = 0x003fb904,
    .CommandCenter = 0x00409d0c,
    .BlackwaterDocks = 0x0040bc6c,
    .AquatosSewers = 0x0040b874,
    .MarcadiaPalace = 0x0040a8ec,
#else
    .Lobby = 0,
    .Bakisi = 0x00408c7c,
    .Hoven = 0x00408564,
    .OutpostX12 = 0x0040045c,
    .KorgonOutpost = 0x003ff89c,
    .Metropolis = 0x003fe47c,
    .BlackwaterCity = 0x003fb204,
    .CommandCenter = 0x00409654,
    .BlackwaterDocks = 0x0040b5b4,
    .AquatosSewers = 0x0040b1bc,
    .MarcadiaPalace = 0x0040a234,
#endif
};

//================================================
//=============  patchGadgetEvents
//================================================
VariableAddress_t vaGadgetEventFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00546510,
    .Hoven = 0x005486d8,
    .OutpostX12 = 0x0053dfb0,
    .KorgonOutpost = 0x0053b698,
    .Metropolis = 0x0053aa98,
    .BlackwaterCity = 0x00538280,
    .CommandCenter = 0x00537ad8,
    .BlackwaterDocks = 0x0053a358,
    .AquatosSewers = 0x00539658,
    .MarcadiaPalace = 0x00538fd8,
#else
    .Lobby = 0,
    .Bakisi = 0x00543c00,
    .Hoven = 0x00545d08,
    .OutpostX12 = 0x0053b620,
    .KorgonOutpost = 0x00538d88,
    .Metropolis = 0x00538188,
    .BlackwaterCity = 0x005358f0,
    .CommandCenter = 0x00535320,
    .BlackwaterDocks = 0x00537b60,
    .AquatosSewers = 0x00536ea0,
    .MarcadiaPalace = 0x005367e0,
#endif
};
VariableAddress_t vaGadgetEventHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0054b23c,
    .Hoven = 0x0054d404,
    .OutpostX12 = 0x00542cdc,
    .KorgonOutpost = 0x005403c4,
    .Metropolis = 0x0053f7c4,
    .BlackwaterCity = 0x0053cfac,
    .CommandCenter = 0x0053c804,
    .BlackwaterDocks = 0x0053f084,
    .AquatosSewers = 0x0053e384,
    .MarcadiaPalace = 0x0053dd04,
#else
    .Lobby = 0,
    .Bakisi = 0x00548894,
    .Hoven = 0x0054a99c,
    .OutpostX12 = 0x005402b4,
    .KorgonOutpost = 0x0053da1c,
    .Metropolis = 0x0053ce1c,
    .BlackwaterCity = 0x0053a584,
    .CommandCenter = 0x00539fb4,
    .BlackwaterDocks = 0x0053c7f4,
    .AquatosSewers = 0x0053bb34,
    .MarcadiaPalace = 0x0053b474,
#endif
};

//================================================
//=============  patchLevelOfDetail
//================================================
VariableAddress_t vaLevelOfDetail_Hook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004c9018,
    .Hoven = 0x004cb130,
    .OutpostX12 = 0x004c0a08,
    .KorgonOutpost = 0x004be1a0,
    .Metropolis = 0x004bd4f0,
    .BlackwaterCity = 0x004bad88,
    .CommandCenter = 0x004bad80,
    .BlackwaterDocks = 0x004bd600,
    .AquatosSewers = 0x004bc900,
    .MarcadiaPalace = 0x004bc280,
#else
    .Lobby = 0,
    .Bakisi = 0x004c68d8,
    .Hoven = 0x004c8930,
    .OutpostX12 = 0x004be248,
    .KorgonOutpost = 0x004bba60,
    .Metropolis = 0x004badb0,
    .BlackwaterCity = 0x004b85c8,
    .CommandCenter = 0x004b8780,
    .BlackwaterDocks = 0x004bafc0,
    .AquatosSewers = 0x004ba300,
    .MarcadiaPalace = 0x004b9c40,
#endif
};
VariableAddress_t vaLevelOfDetail_Shrubs = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00248e18,
    .Hoven = 0x00249018,
    .OutpostX12 = 0x00248f08,
    .KorgonOutpost = 0x00248d88,
    .Metropolis = 0x00248e08,
    .BlackwaterCity = 0x00248d88,
    .CommandCenter = 0x00248988,
    .BlackwaterDocks = 0x00248a88,
    .AquatosSewers = 0x00248a88,
    .MarcadiaPalace = 0x00248a88,
#else
    .Lobby = 0,
    .Bakisi = 0x00248f98,
    .Hoven = 0x00249198,
    .OutpostX12 = 0x00249088,
    .KorgonOutpost = 0x00248f08,
    .Metropolis = 0x00248f88,
    .BlackwaterCity = 0x00248f08,
    .CommandCenter = 0x00248b08,
    .BlackwaterDocks = 0x00248c08,
    .AquatosSewers = 0x00248c08,
    .MarcadiaPalace = 0x00248c08,
#endif
};
VariableAddress_t vaLevelOfDetail_Terrain = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00248ee0,
    .Hoven = 0x002490e0,
    .OutpostX12 = 0x00248fd0,
    .KorgonOutpost = 0x00248e50,
    .Metropolis = 0x00248ed0,
    .BlackwaterCity = 0x00248e50,
    .CommandCenter = 0x00248a50,
    .BlackwaterDocks = 0x00248b50,
    .AquatosSewers = 0x00248b50,
    .MarcadiaPalace = 0x00248b50,
#else
    .Lobby = 0,
    .Bakisi = 0x00249060,
    .Hoven = 0x00249260,
    .OutpostX12 = 0x00249150,
    .KorgonOutpost = 0x00248fd0,
    .Metropolis = 0x00249050,
    .BlackwaterCity = 0x00248fd0,
    .CommandCenter = 0x00248bd0,
    .BlackwaterDocks = 0x00248cd0,
    .AquatosSewers = 0x00248cd0,
    .MarcadiaPalace = 0x00248cd0,
#endif
};

VariableAddress_t vaLevelOfDetail_Ties = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00248f44,
    .Hoven = 0x00249144,
    .OutpostX12 = 0x00249034,
    .KorgonOutpost = 0x00248eb4,
    .Metropolis = 0x00248f34,
    .BlackwaterCity = 0x00248eb4,
    .CommandCenter = 0x00248ab4,
    .BlackwaterDocks = 0x00248bb4,
    .AquatosSewers = 0x00248bb4,
    .MarcadiaPalace = 0x00248bb4,
#else
    .Lobby = 0,
    .Bakisi = 0x002490c4,
    .Hoven = 0x002492c4,
    .OutpostX12 = 0x002491b4,
    .KorgonOutpost = 0x00249034,
    .Metropolis = 0x002490b4,
    .BlackwaterCity = 0x00249034,
    .CommandCenter = 0x00248c34,
    .BlackwaterDocks = 0x00248d34,
    .AquatosSewers = 0x00248d34,
    .MarcadiaPalace = 0x00248d34,
#endif
};

//================================================
//=============  runFpsCounter
//================================================
VariableAddress_t vaFpsCounter_DrawFunc = {
#if UYA_PAL
	.Lobby = 0x00589410,
	.Bakisi = 0x00456328,
	.Hoven = 0x00457ea8,
	.OutpostX12 = 0x0044eca8,
	.KorgonOutpost = 0x0044c868,
	.Metropolis = 0x0044bba8,
	.BlackwaterCity = 0x004493a8,
	.CommandCenter = 0x0044a028,
	.BlackwaterDocks = 0x0044c8a8,
	.AquatosSewers = 0x0044bba8,
	.MarcadiaPalace = 0x0044b528,
#else
	.Lobby = 0x005882a0,
	.Bakisi = 0x004552b8,
	.Hoven = 0x00456d78,
	.OutpostX12 = 0x0044dbb8,
	.KorgonOutpost = 0x0044b7f8,
	.Metropolis = 0x0044ab38,
	.BlackwaterCity = 0x004482b8,
	.CommandCenter = 0x004490f8,
	.BlackwaterDocks = 0x0044b938,
	.AquatosSewers = 0x0044ac78,
	.MarcadiaPalace = 0x0044a5b8,
#endif
};
VariableAddress_t vaFpsCounter_UpdateFunc = {
#if UYA_PAL
	.Lobby = 0x005fbf18,
	.Bakisi = 0x004ce660,
	.Hoven = 0x004d0778,
	.OutpostX12 = 0x004c6050,
	.KorgonOutpost = 0x004c37e8,
	.Metropolis = 0x004c2b38,
	.BlackwaterCity = 0x004c03d0,
	.CommandCenter = 0x004c03c8,
	.BlackwaterDocks = 0x004c2c48,
	.AquatosSewers = 0x004c1f48,
	.MarcadiaPalace = 0x004c18c8,
#else
	.Lobby = 0x005f9780,
	.Bakisi = 0x004cbf20,
	.Hoven = 0x004cdf78,
	.OutpostX12 = 0x004c3890,
	.KorgonOutpost = 0x004c10a8,
	.Metropolis = 0x004c03f8,
	.BlackwaterCity = 0x004bdc10,
	.CommandCenter = 0x004bddc8,
	.BlackwaterDocks = 0x004c0608,
	.AquatosSewers = 0x004bf948,
	.MarcadiaPalace = 0x004bf288,
#endif
};
VariableAddress_t vaFpsCounter_Hooks = {
#if UYA_PAL
	.Lobby = 0x00575fc8,
	.Bakisi = 0x004428c8,
	.Hoven = 0x00444448,
	.OutpostX12 = 0x0043b248,
	.KorgonOutpost = 0x00438e08,
	.Metropolis = 0x00438148,
	.BlackwaterCity = 0x00435948,
	.CommandCenter = 0x004365c8,
	.BlackwaterDocks = 0x00438e48,
	.AquatosSewers = 0x00438148,
	.MarcadiaPalace = 0x00437ac8,
#else
	.Lobby = 0x00575288,
	.Bakisi = 0x00441c88,
	.Hoven = 0x00443748,
	.OutpostX12 = 0x0043a588,
	.KorgonOutpost = 0x004381c8,
	.Metropolis = 0x00437508,
	.BlackwaterCity = 0x00434c88,
	.CommandCenter = 0x00435ac8,
	.BlackwaterDocks = 0x00438308,
	.AquatosSewers = 0x00437648,
	.MarcadiaPalace = 0x00436f88,
#endif
};

//================================================
//=============  patchFov
//================================================
VariableAddress_t vaFieldOfView_FluxRA = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0040682c,
	.Hoven = 0x00406194,
	.OutpostX12 = 0x003fe08c,
    .KorgonOutpost = 0x003fd46c,
	.Metropolis = 0x003fc02c,
	.BlackwaterCity = 0x003f8e14,
	.CommandCenter = 0x0040721c,
    .BlackwaterDocks = 0x0040917c,
    .AquatosSewers = 0x00408d84,
    .MarcadiaPalace = 0x00407dfc,
#else
	.Lobby = 0,
	.Bakisi = 0x004061c4,
	.Hoven = 0x00405aac,
	.OutpostX12 = 0x003fd9a4,
    .KorgonOutpost = 0x003fcde4,
	.Metropolis = 0x003fb9c4,
	.BlackwaterCity = 0x003f874c,
	.CommandCenter = 0x00406b9c,
    .BlackwaterDocks = 0x00408afc,
    .AquatosSewers = 0x00408704,
    .MarcadiaPalace = 0x0040777c,
#endif
};
VariableAddress_t vaFieldOfView_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004452e0,
	.Hoven = 0x00446e60,
	.OutpostX12 = 0x0043dc60,
    .KorgonOutpost = 0x0043b820,
	.Metropolis = 0x0043ab60,
	.BlackwaterCity = 0x00438360,
	.CommandCenter = 0x00438fe0,
    .BlackwaterDocks = 0x0043b860,
    .AquatosSewers = 0x0043ab60,
    .MarcadiaPalace = 0x0043a4e0,
#else
	.Lobby = 0,
	.Bakisi = 0x00444468,
	.Hoven = 0x00445f28,
	.OutpostX12 = 0x0043cd68,
    .KorgonOutpost = 0x0043a9a8,
	.Metropolis = 0x00439ce8,
	.BlackwaterCity = 0x00437468,
	.CommandCenter = 0x004382a8,
    .BlackwaterDocks = 0x0043aae8,
    .AquatosSewers = 0x00439e28,
    .MarcadiaPalace = 0x00439768,
#endif
};
VariableAddress_t vaSetPOSRot_fovChange_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0050ca54,
	.Hoven = 0x0050eb6c,
	.OutpostX12 = 0x00504444,
    .KorgonOutpost = 0x00501bdc,
	.Metropolis = 0x00500f2c,
	.BlackwaterCity = 0x004fe7c4,
	.CommandCenter = 0x004fe78c,
    .BlackwaterDocks = 0x0050100c,
    .AquatosSewers = 0x0050030c,
    .MarcadiaPalace = 0x004ffc8c,
#else
	.Lobby = 0,
	.Bakisi = 0x0050a264,
	.Hoven = 0x0050c2bc,
	.OutpostX12 = 0x00501bd4,
    .KorgonOutpost = 0x004ff3ec,
	.Metropolis = 0x004fe73c,
	.BlackwaterCity = 0x004fbf54,
	.CommandCenter = 0x004fc0dc,
    .BlackwaterDocks = 0x004fe91c,
    .AquatosSewers = 0x004fdc5c,
    .MarcadiaPalace = 0x004fd59c,
#endif
};
VariableAddress_t vaSetPOSRot_fovChange_Func = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00462460,
	.Hoven = 0x00464010,
	.OutpostX12 = 0x0045ae10,
    .KorgonOutpost = 0x004589a0,
	.Metropolis = 0x00457ce0,
	.BlackwaterCity = 0x00455510,
	.CommandCenter = 0x00455e08,
    .BlackwaterDocks = 0x00458688,
    .AquatosSewers = 0x00457988,
    .MarcadiaPalace = 0x00457308,
#else
	.Lobby = 0,
	.Bakisi = 0x004612b0,
	.Hoven = 0x00462da0,
	.OutpostX12 = 0x00459be0,
    .KorgonOutpost = 0x004577f0,
	.Metropolis = 0x00456b30,
	.BlackwaterCity = 0x004542e0,
	.CommandCenter = 0x00454d98,
    .BlackwaterDocks = 0x004575d8,
    .AquatosSewers = 0x00456918,
    .MarcadiaPalace = 0x00456258,
#endif
};

//================================================
//=============  setupPatchConfigInGame
//================================================
VariableAddress_t vaPauseMenuAddr = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x003c073c,
    .Hoven = 0x003bfd7c,
    .OutpostX12 = 0x003b7c7c,
    .KorgonOutpost = 0x003b7a3c,
    .Metropolis = 0x003b77bc,
    .BlackwaterCity = 0x003b79fc,
    .CommandCenter = 0x003c7f7c,
    .BlackwaterDocks = 0x003c837c,
    .AquatosSewers = 0x003c8afc,
    .MarcadiaPalace = 0x003c807c,
#else
    .Lobby = 0,
    .Bakisi = 0x003c087c,
    .Hoven = 0x003bfebc,
    .OutpostX12 = 0x003b7dbc,
    .KorgonOutpost = 0x003b7b7c,
    .Metropolis = 0x003b78fc,
    .BlackwaterCity = 0x003b7b3c,
    .CommandCenter = 0x003c80bc,
    .BlackwaterDocks = 0x003c84bc,
    .AquatosSewers = 0x003c8c3c,
    .MarcadiaPalace = 0x003c81bc,
#endif
};

//================================================
//=============  GameplayHook
//================================================
VariableAddress_t vaGameplayHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0046db20,
    .Hoven = 0x0046f6d0,
    .OutpostX12 = 0x004664d0,
    .KorgonOutpost = 0x00464060,
    .Metropolis = 0x004633a0,
    .BlackwaterCity = 0x00460bd0,
    .CommandCenter = 0x004614c8,
    .BlackwaterDocks = 0x00463d48,
    .AquatosSewers = 0x00463048,
    .MarcadiaPalace = 0x004629c8,
#else
    .Lobby = 0,
    .Bakisi = 0x0046bfb8,
    .Hoven = 0x0046daa8,
    .OutpostX12 = 0x004648e8,
    .KorgonOutpost = 0x004624f8,
    .Metropolis = 0x00461838,
    .BlackwaterCity = 0x0045efe8,
    .CommandCenter = 0x0045faa0,
    .BlackwaterDocks = 0x004622e0,
    .AquatosSewers = 0x00461620,
    .MarcadiaPalace = 0x00460f60,
#endif
};

//================================================
//=============  patchAlwaysShowHealthBar
//================================================
VariableAddress_t vaHealthBarTimerSaveZero = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00548020,
	.Hoven = 0x0054a1e8,
	.OutpostX12 = 0x0053fac0,
    .KorgonOutpost = 0x0053c5a8,
	.Metropolis = 0x0053c5a8,
	.BlackwaterCity = 0x00539d90,
	.CommandCenter = 0x005395e8,
    .BlackwaterDocks = 0x0053be68,
    .AquatosSewers = 0x0053b168,
    .MarcadiaPalace = 0x0053aae8,
#else
	.Lobby = 0,
	.Bakisi = 0x00545718,
	.Hoven = 0x00547820,
	.OutpostX12 = 0x0053d138,
    .KorgonOutpost = 0x0053a8a0,
	.Metropolis = 0x00539ca0,
	.BlackwaterCity = 0x00537408,
	.CommandCenter = 0x00536e38,
    .BlackwaterDocks = 0x00539678,
    .AquatosSewers = 0x005389b8,
    .MarcadiaPalace = 0x005382f8,
#endif
};

//================================================
//=============  patchMapAndScoreboardToggle
//================================================
VariableAddress_t vaMapScore_SelectBtn_Addr = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x005480fc,
    .Hoven = 0x0054a2c4,
    .OutpostX12 = 0x0053fb9c,
    .KorgonOutpost = 0x0053d284,
    .Metropolis = 0x0053c684,
    .BlackwaterCity = 0x00539e6c,
    .CommandCenter = 0x005396c4,
    .BlackwaterDocks = 0x0053bf44,
    .AquatosSewers = 0x0053b244,
    .MarcadiaPalace = 0x0053abc4,
#else
    .Lobby = 0,
    .Bakisi = 0x005457f4,
    .Hoven = 0x005478fc,
    .OutpostX12 = 0x0053d214,
    .KorgonOutpost = 0x0053a97c,
    .Metropolis = 0x00539d7c,
    .BlackwaterCity = 0x005374e4,
    .CommandCenter = 0x00536f14,
    .BlackwaterDocks = 0x00539754,
    .AquatosSewers = 0x00538a94,
    .MarcadiaPalace = 0x005383d4,
#endif
};
VariableAddress_t vaMapScore_SelectBtn_Val = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0c152ac6,
    .Hoven = 0x0c153338,
    .OutpostX12 = 0x0c15096e,
    .KorgonOutpost = 0x0c14ff28,
    .Metropolis = 0x0c14fc28,
    .BlackwaterCity = 0x0c14f222,
    .CommandCenter = 0x0c14f038,
    .BlackwaterDocks = 0x0c14fa58,
    .AquatosSewers = 0x0c14f718,
    .MarcadiaPalace = 0x0c14f578,
#else
    .Lobby = 0,
    .Bakisi = 0x0c15205c,
    .Hoven = 0x0c15289e,
    .OutpostX12 = 0x0c14fee4,
    .KorgonOutpost = 0x0c14f4be,
    .Metropolis = 0x0c14f1be,
    .BlackwaterCity = 0x0c14e798,
    .CommandCenter = 0x0c14e624,
    .BlackwaterDocks = 0x0c14f034,
    .AquatosSewers = 0x0c14ed04,
    .MarcadiaPalace = 0x0c14eb54,
#endif
};
VariableAddress_t vaMapScore_SeigeCTFMap_AlwaysRun = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004ae978,
    .Hoven = 0x004b0a90,
    .OutpostX12 = 0x004a6368,
    .KorgonOutpost = 0x004a3b00,
    .Metropolis = 0x004a2e50,
    .BlackwaterCity = 0x004a06e8,
    .CommandCenter = 0x004a06e0,
    .BlackwaterDocks = 0x004a2f60,
    .AquatosSewers = 0x004a2260,
    .MarcadiaPalace = 0x004a1be0,
#else
    .Lobby = 0,
    .Bakisi = 0x004ac428,
    .Hoven = 0x004ae480,
    .OutpostX12 = 0x004a3d98,
    .KorgonOutpost = 0x004a15b0,
    .Metropolis = 0x004a0900,
    .BlackwaterCity = 0x0049e118,
    .CommandCenter = 0x0049e2d0,
    .BlackwaterDocks = 0x004a0b10,
    .AquatosSewers = 0x0049fe50,
    .MarcadiaPalace = 0x0049f790,
#endif
};
VariableAddress_t vaMapScore_SeigeCTFScoreboard_AlwaysRun = {
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
VariableAddress_t vaMapScore_MapToggle = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004ae960,
    .Hoven = 0x004b0a78,
    .OutpostX12 = 0x004a6350,
    .KorgonOutpost = 0x004a3ae8,
    .Metropolis = 0x004a2e38,
    .BlackwaterCity = 0x004a06d0,
    .CommandCenter = 0x004a06c8,
    .BlackwaterDocks = 0x004a2f48,
    .AquatosSewers = 0x004a2248,
    .MarcadiaPalace = 0x004a1bc8,
#else
    .Lobby = 0,
    .Bakisi = 0x004ac410,
    .Hoven = 0x004ae468,
    .OutpostX12 = 0x004a3d80,
    .KorgonOutpost = 0x004a1598,
    .Metropolis = 0x004a08e8,
    .BlackwaterCity = 0x0049e100,
    .CommandCenter = 0x0049e2b8,
    .BlackwaterDocks = 0x004a0af8,
    .AquatosSewers = 0x0049fe38,
    .MarcadiaPalace = 0x0049f778,
#endif
};
VariableAddress_t vaMapScore_ScoreboardToggle = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004b1690,
    .Hoven = 0x004b37a8,
    .OutpostX12 = 0x004a9080,
    .KorgonOutpost = 0x004a6818,
    .Metropolis = 0x004a5b68,
    .BlackwaterCity = 0x004a3400,
    .CommandCenter = 0x004a33f8,
    .BlackwaterDocks = 0x004a5c78,
    .AquatosSewers = 0x004a4f78,
    .MarcadiaPalace = 0x004a48f8,
#else
    .Lobby = 0,
    .Bakisi = 0x004af140,
    .Hoven = 0x004b1198,
    .OutpostX12 = 0x004a6ab0,
    .KorgonOutpost = 0x004a42c8,
    .Metropolis = 0x004a3618,
    .BlackwaterCity = 0x004a0e30,
    .CommandCenter = 0x004a0fe8,
    .BlackwaterDocks = 0x004a3828,
    .AquatosSewers = 0x004a2b68,
    .MarcadiaPalace = 0x004a24a8,
#endif
};

//================================================
//=============  patchQuickSelectTimer
//================================================
VariableAddress_t vaQuickSelectCheck_Hook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00549170,
    .Hoven = 0x0054b338,
    .OutpostX12 = 0x00540c10,
    .KorgonOutpost = 0x0053e2f8,
    .Metropolis = 0x0053d6f8,
    .BlackwaterCity = 0x0053aee0,
    .CommandCenter = 0x0053a738,
    .BlackwaterDocks = 0x0053cfb8,
    .AquatosSewers = 0x0053c2b8,
    .MarcadiaPalace = 0x0053bc38,
#else
    .Lobby = 0,
    .Bakisi = 0x00546818,
    .Hoven = 0x00548920,
    .OutpostX12 = 0x0053e238,
    .KorgonOutpost = 0x0053b9a0,
    .Metropolis = 0x0053ada0,
    .BlackwaterCity = 0x00538508,
    .CommandCenter = 0x00537f38,
    .BlackwaterDocks = 0x0053a778,
    .AquatosSewers = 0x00539ab8,
    .MarcadiaPalace = 0x005393f8,
#endif
};
VariableAddress_t vaQuickSelectCheck_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0054a8a8,
    .Hoven = 0x0054ca70,
    .OutpostX12 = 0x00542348,
    .KorgonOutpost = 0x0053fa30,
    .Metropolis = 0x0053ee30,
    .BlackwaterCity = 0x0053c618,
    .CommandCenter = 0x0053be70,
    .BlackwaterDocks = 0x0053e6f0,
    .AquatosSewers = 0x0053d9f0,
    .MarcadiaPalace = 0x0053d370,
#else
    .Lobby = 0,
    .Bakisi = 0x00547f00,
    .Hoven = 0x0054a008,
    .OutpostX12 = 0x0053f920,
    .KorgonOutpost = 0x0053d088,
    .Metropolis = 0x0053c488,
    .BlackwaterCity = 0x00539bf0,
    .CommandCenter = 0x00539620,
    .BlackwaterDocks = 0x0053be60,
    .AquatosSewers = 0x0053b1a0,
    .MarcadiaPalace = 0x0053aae0,
#endif
};

//================================================
//=============  playerInvicibleTimer
//================================================
VariableAddress_t vaPlayerInvicibleTimer_Hook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0052916c,
    .Hoven = 0x0052b284,
    .OutpostX12 = 0x00520b5c,
    .KorgonOutpost = 0x0051e2f4,
    .Metropolis = 0x0051d644,
    .BlackwaterCity = 0x0051aedc,
    .CommandCenter = 0x0051ac9c,
    .BlackwaterDocks = 0x0051d51c,
    .AquatosSewers = 0x0051c81c,
    .MarcadiaPalace = 0x0051c19c,
#else
    .Lobby = 0,
    .Bakisi = 0x005268ec,
    .Hoven = 0x00528944,
    .OutpostX12 = 0x0051e25c,
    .KorgonOutpost = 0x0051ba74,
    .Metropolis = 0x0051adc4,
    .BlackwaterCity = 0x005185dc,
    .CommandCenter = 0x0051855c,
    .BlackwaterDocks = 0x0051ad9c,
    .AquatosSewers = 0x0051a0dc,
    .MarcadiaPalace = 0x00519a1c,
#endif
};
VariableAddress_t vaPlayerInvicibleTimer_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004f8c30,
    .Hoven = 0x004fad48,
    .OutpostX12 = 0x004f0620,
    .KorgonOutpost = 0x004eddb8,
    .Metropolis = 0x004ed108,
    .BlackwaterCity = 0x004ea9a0,
    .CommandCenter = 0x004ea968,
    .BlackwaterDocks = 0x004ed1e8,
    .AquatosSewers = 0x004ec4e8,
    .MarcadiaPalace = 0x004ebe68,
#else
    .Lobby = 0,
    .Bakisi = 0x004f64b0,
    .Hoven = 0x004f8508,
    .OutpostX12 = 0x004ede20,
    .KorgonOutpost = 0x004eb638,
    .Metropolis = 0x004ea988,
    .BlackwaterCity = 0x004e81a0,
    .CommandCenter = 0x004e8328,
    .BlackwaterDocks = 0x004eab68,
    .AquatosSewers = 0x004e9ea8,
    .MarcadiaPalace = 0x004e97e8,
#endif
};