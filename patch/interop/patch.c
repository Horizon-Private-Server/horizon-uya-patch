#include <libuya/interop.h>
#include <libuya/player.h>

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
//=============  hypershotEquipButton
//================================================
VariableAddress_t vaHypershotEquipButton_bits = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00510f54,
	.Hoven = 0x0051306c,
	.OutpostX12 = 0x00508944,
	.KorgonOutpost = 0x005060dc,
	.Metropolis = 0x0050542c,
	.BlackwaterCity = 0x00502cc4,
	.CommandCenter = 0x00502c8c,
	.BlackwaterDocks = 0x0050550c,
	.AquatosSewers = 0x0050480c,
	.MarcadiaPalace = 0x0050418c,
#else
	.Lobby = 0,
	.Bakisi = 0x0050e73c,
	.Hoven = 0x00510794,
	.OutpostX12 = 0x005060ac,
	.KorgonOutpost = 0x005038c4,
	.Metropolis = 0x00502c14,
	.BlackwaterCity = 0x0050042c,
	.CommandCenter = 0x018059e8,
	.BlackwaterDocks = 0x00502df4,
	.AquatosSewers = 0x00502134,
	.MarcadiaPalace = 0x00501a74,
#endif
};

//================================================
//=============  patchSideFlipJoystickVal
//================================================
VariableAddress_t vaSideFlipJoystickVal = {
#if UYA_PAL
    .Lobby = 0x0062d978,
    .Bakisi = 0x004ff870,
    .Hoven = 0x00501988,
    .OutpostX12 = 0x004f7260,
    .KorgonOutpost = 0x004f49f8,
    .Metropolis = 0x004f3d48,
    .BlackwaterCity = 0x004f15e0,
    .CommandCenter = 0x004f15a8,
    .BlackwaterDocks = 0x004f3e28,
    .AquatosSewers = 0x004f3128,
    .MarcadiaPalace = 0x004f2aa8,
#else
    .Lobby = 0x0062b1a0,
    .Bakisi = 0x004fd0f0,
    .Hoven = 0x004ff148,
    .OutpostX12 = 0x004f4a60,
    .KorgonOutpost = 0x004f2278,
    .Metropolis = 0x004f15c8,
    .BlackwaterCity = 0x004eede0,
    .CommandCenter = 0x004eef68,
    .BlackwaterDocks = 0x004f17a8,
    .AquatosSewers = 0x004f0ae8,
    .MarcadiaPalace = 0x004f0428,
#endif
};  

VariableAddress_t vaZRotDiffSpherical = {
#if UYA_PAL
    .Lobby = 0x00633c78,
    .Bakisi = 0x00505b70,
    .Hoven = 0x00507c88,
    .OutpostX12 = 0x004fd560,
    .KorgonOutpost = 0x004facf8,
    .Metropolis = 0x004fa048,
    .BlackwaterCity = 0x004f78e0,
    .CommandCenter = 0x004f78a8,
    .BlackwaterDocks = 0x004fa128,
    .AquatosSewers = 0x004f9428,
    .MarcadiaPalace = 0x004f8da8,
#else
    .Lobby = 0x00631430,
    .Bakisi = 0x00503380,
    .Hoven = 0x005053d8,
    .OutpostX12 = 0x004facf0,
    .KorgonOutpost = 0x004f8508,
    .Metropolis = 0x004f7858,
    .BlackwaterCity = 0x004f5070,
    .CommandCenter = 0x004f51f8,
    .BlackwaterDocks = 0x004f7a38,
    .AquatosSewers = 0x004f6d78,
    .MarcadiaPalace = 0x004f66b8,
#endif
};

// typedef void (*ZRotDiffSpherical_Func)(Player* player, VECTOR *angleVec, int turnDir);
// ZRotDiffSpherical_Func ZRotDiffSpherical = (ZRotDiffSpherical_Func)GetAddress(&vaZRotDiffSpherical);



//================================================
//=============  playerSync
//================================================

VariableAddress_t vaOnMobyUpdate_Func = {
#if UYA_PAL
	.Lobby = 0x0052bca8,
	.Bakisi = 0x003db0e8,
	.Hoven = 0x003dad08,
	.OutpostX12 = 0x003d2c08,
	.KorgonOutpost = 0x003d3020,
	.Metropolis = 0x003d0ba8,
	.BlackwaterCity = 0x003d2908,
	.CommandCenter = 0x003dec18,
	.BlackwaterDocks = 0x003e1098,
	.AquatosSewers = 0x003e0780,
	.MarcadiaPalace = 0x003ded18,
#else
	.Lobby = 0x0052b740,
	.Bakisi = 0x003daec0,
	.Hoven = 0x003daa60,
	.OutpostX12 = 0x003d2960,
	.KorgonOutpost = 0x003d2dd8,
	.Metropolis = 0x003d0980,
	.BlackwaterCity = 0x003d2660,
	.CommandCenter = 0x003de9b0,
	.BlackwaterDocks = 0x003e0e30,
	.AquatosSewers = 0x003e0518,
	.MarcadiaPalace = 0x003deab0,
#endif
};

VariableAddress_t vaPlayerSyncFixOverflow1 = {
#if UYA_PAL
	.Lobby = 0x005ac8b8,
	.Bakisi = 0x00479170,
	.Hoven = 0x0047ad30,
	.OutpostX12 = 0x00471b30,
	.KorgonOutpost = 0x0046f6b0,
	.Metropolis = 0x0046e9f0,
	.BlackwaterCity = 0x0046c230,
	.CommandCenter = 0x0046caf0,
	.BlackwaterDocks = 0x0046f370,
	.AquatosSewers = 0x0046e670,
	.MarcadiaPalace = 0x0046dff0,
#else
	.Lobby = 0x005aa9b8,
	.Bakisi = 0x004773b0,
	.Hoven = 0x00478eb0,
	.OutpostX12 = 0x0046fcf0,
	.KorgonOutpost = 0x0046d8f0,
	.Metropolis = 0x0046cc30,
	.BlackwaterCity = 0x0046a3f0,
	.CommandCenter = 0x0046ae70,
	.BlackwaterDocks = 0x0046d6b0,
	.AquatosSewers = 0x0046c9f0,
	.MarcadiaPalace = 0x0046c330,
#endif
};

VariableAddress_t vaPlayerSyncFixOverflow2 = {
#if UYA_PAL
	.Lobby = 0x00580418,
	.Bakisi = 0x0044d330,
	.Hoven = 0x0044eeb0,
	.OutpostX12 = 0x00445cb0,
	.KorgonOutpost = 0x00443870,
	.Metropolis = 0x00442bb0,
	.BlackwaterCity = 0x004403b0,
	.CommandCenter = 0x00441030,
	.BlackwaterDocks = 0x004438b0,
	.AquatosSewers = 0x00442bb0,
	.MarcadiaPalace = 0x00442530,
#else
	.Lobby = 0x0057f470,
	.Bakisi = 0x0044c488,
	.Hoven = 0x0044df48,
	.OutpostX12 = 0x00444d88,
	.KorgonOutpost = 0x004429c8,
	.Metropolis = 0x00441d08,
	.BlackwaterCity = 0x0043f488,
	.CommandCenter = 0x004402c8,
	.BlackwaterDocks = 0x00442b08,
	.AquatosSewers = 0x00441e48,
	.MarcadiaPalace = 0x00441788,
#endif
};

VariableAddress_t vaPlayerSyncFixOverflow3 = {
#if UYA_PAL
	.Lobby = 0x00580424,
	.Bakisi = 0x0044d33c,
	.Hoven = 0x0044eebc,
	.OutpostX12 = 0x00445cbc,
	.KorgonOutpost = 0x0044387c,
	.Metropolis = 0x00442bbc,
	.BlackwaterCity = 0x004403bc,
	.CommandCenter = 0x0044103c,
	.BlackwaterDocks = 0x004438bc,
	.AquatosSewers = 0x00442bbc,
	.MarcadiaPalace = 0x0044253c,
#else
	.Lobby = 0x0057f47c,
	.Bakisi = 0x0044c494,
	.Hoven = 0x0044df54,
	.OutpostX12 = 0x00444d94,
	.KorgonOutpost = 0x004429d4,
	.Metropolis = 0x00441d14,
	.BlackwaterCity = 0x0043f494,
	.CommandCenter = 0x004402d4,
	.BlackwaterDocks = 0x00442b14,
	.AquatosSewers = 0x00441e54,
	.MarcadiaPalace = 0x00441794,
#endif
};

VariableAddress_t vaPlayerSyncFixOverflow4 = {
#if UYA_PAL
	.Lobby = 0x00580440,
	.Bakisi = 0x0044d358,
	.Hoven = 0x0044eed8,
	.OutpostX12 = 0x00445cd8,
	.KorgonOutpost = 0x00443898,
	.Metropolis = 0x00442bd8,
	.BlackwaterCity = 0x004403d8,
	.CommandCenter = 0x00441058,
	.BlackwaterDocks = 0x004438d8,
	.AquatosSewers = 0x00442bd8,
	.MarcadiaPalace = 0x00442558,
#else
	.Lobby = 0x0057f498,
	.Bakisi = 0x0044c4b0,
	.Hoven = 0x0044df70,
	.OutpostX12 = 0x00444db0,
	.KorgonOutpost = 0x004429f0,
	.Metropolis = 0x00441d30,
	.BlackwaterCity = 0x0043f4b0,
	.CommandCenter = 0x004402f0,
	.BlackwaterDocks = 0x00442b30,
	.AquatosSewers = 0x00441e70,
	.MarcadiaPalace = 0x004417b0,
#endif
};

VariableAddress_t vaPlayerSyncFixLagout1 = {
#if UYA_PAL
	.Lobby = 0x00666240,
	.Bakisi = 0x00538340,
	.Hoven = 0x0053a458,
	.OutpostX12 = 0x0052fd30,
	.KorgonOutpost = 0x0052d4c8,
	.Metropolis = 0x0052c818,
	.BlackwaterCity = 0x0052a0b0,
	.CommandCenter = 0x00529e70,
	.BlackwaterDocks = 0x0052c6f0,
	.AquatosSewers = 0x0052b9f0,
	.MarcadiaPalace = 0x0052b370,
#else
	.Lobby = 0x00663968,
	.Bakisi = 0x00535ac0,
	.Hoven = 0x00537b18,
	.OutpostX12 = 0x0052d430,
	.KorgonOutpost = 0x0052ac48,
	.Metropolis = 0x00529f98,
	.BlackwaterCity = 0x005277b0,
	.CommandCenter = 0x00527730,
	.BlackwaterDocks = 0x00529f70,
	.AquatosSewers = 0x005292b0,
	.MarcadiaPalace = 0x00528bf0,
#endif
};

VariableAddress_t vaPlayerSyncFixTurretDelay = {
#if UYA_PAL
	.Lobby = 0x00648204,
	.Bakisi = 0x0051a304,
	.Hoven = 0x0051c41c,
	.OutpostX12 = 0x00511cf4,
	.KorgonOutpost = 0x0050f48c,
	.Metropolis = 0x0050e7dc,
	.BlackwaterCity = 0x0050c074,
	.CommandCenter = 0x0050be34,
	.BlackwaterDocks = 0x0050e6b4,
	.AquatosSewers = 0x0050d9b4,
	.MarcadiaPalace = 0x0050d334,
#else
	.Lobby = 0x006459a4,
	.Bakisi = 0x00517afc,
	.Hoven = 0x00519b54,
	.OutpostX12 = 0x0050f46c,
	.KorgonOutpost = 0x0050cc84,
	.Metropolis = 0x0050bfd4,
	.BlackwaterCity = 0x005097ec,
	.CommandCenter = 0x0050976c,
	.BlackwaterDocks = 0x016aa3c4,
	.AquatosSewers = 0x0050b2ec,
	.MarcadiaPalace = 0x0050ac2c,
#endif
};

VariableAddress_t vaPlayerDisableAimAssist = {
#if UYA_PAL
	.Lobby = 0x00648638,
	.Bakisi = 0x0051a738,
	.Hoven = 0x0051c850,
	.OutpostX12 = 0x00512128,
	.KorgonOutpost = 0x0050f8c0,
	.Metropolis = 0x0050ec10,
	.BlackwaterCity = 0x0050c4a8,
	.CommandCenter = 0x0050c268,
	.BlackwaterDocks = 0x0050eae8,
	.AquatosSewers = 0x0050dde8,
	.MarcadiaPalace = 0x0050d768,
#else
	.Lobby = 0x00645dd8,
	.Bakisi = 0x00517f30,
	.Hoven = 0x00519f88,
	.OutpostX12 = 0x0050f8a0,
	.KorgonOutpost = 0x0050d0b8,
	.Metropolis = 0x0050c408,
	.BlackwaterCity = 0x00509c20,
	.CommandCenter = 0x00509ba0,
	.BlackwaterDocks = 0x0050c3e0,
	.AquatosSewers = 0x0050b720,
	.MarcadiaPalace = 0x0050b060,
#endif
};



VariableAddress_t vaNWUpdate_Func = {
#if UYA_PAL
	.Lobby = 0x001959f8,
	.Bakisi = 0x001959f8,
	.Hoven = 0x001959f8,
	.OutpostX12 = 0x001959f8,
	.KorgonOutpost = 0x001959f8,
	.Metropolis = 0x001959f8,
	.BlackwaterCity = 0x001959f8,
	.CommandCenter = 0x001959f8,
	.BlackwaterDocks = 0x001959f8,
	.AquatosSewers = 0x001959f8,
	.MarcadiaPalace = 0x001959f8,
#else
	.Lobby = 0x00195ae8,
	.Bakisi = 0x00195ae8,
	.Hoven = 0x00195ae8,
	.OutpostX12 = 0x00195ae8,
	.KorgonOutpost = 0x00195ae8,
	.Metropolis = 0x00195ae8,
	.BlackwaterCity = 0x00195ae8,
	.CommandCenter = 0x00195ae8,
	.BlackwaterDocks = 0x00195ae8,
	.AquatosSewers = 0x00195ae8,
	.MarcadiaPalace = 0x00195ae8,
#endif
};

VariableAddress_t vaGameTimeUpdate_Hook = { // unsure
#if UYA_PAL
	.Lobby = 0x005765ac,
	.Bakisi = 0x00442eac,
	.Hoven = 0x00444a2c,
	.OutpostX12 = 0x0043b82c,
	.KorgonOutpost = 0x004393ec,
	.Metropolis = 0x0043872c,
	.BlackwaterCity = 0x00435f2c,
	.CommandCenter = 0x00436bac,
	.BlackwaterDocks = 0x0043942c,
	.AquatosSewers = 0x0043872c,
	.MarcadiaPalace = 0x004380ac,
#else
	.Lobby = 0x0057586c,
	.Bakisi = 0x0044226c,
	.Hoven = 0x00443d2c,
	.OutpostX12 = 0x0043ab6c,
	.KorgonOutpost = 0x004387ac,
	.Metropolis = 0x00437aec,
	.BlackwaterCity = 0x0043526c,
	.CommandCenter = 0x004360ac,
	.BlackwaterDocks = 0x004388ec,
	.AquatosSewers = 0x00437c2c,
	.MarcadiaPalace = 0x0043756c,
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

VariableAddress_t vaWaitingForResponse_Addr = {
#if UYA_PAL
	.Lobby = 0x0065f554,
	.Bakisi = 0x00531654,
	.Hoven = 0x0053376c,
	.OutpostX12 = 0x00529044,
	.KorgonOutpost = 0x005267dc,
	.Metropolis = 0x00525b2c,
	.BlackwaterCity = 0x005233c4,
	.CommandCenter = 0x00523184,
	.BlackwaterDocks = 0x00525a04,
	.AquatosSewers = 0x00524d04,
	.MarcadiaPalace = 0x00524684,
#else
	.Lobby = 0x0065cc7c,
	.Bakisi = 0x0052edd4,
	.Hoven = 0x00530e2c,
	.OutpostX12 = 0x00526744,
	.KorgonOutpost = 0x00523f5c,
	.Metropolis = 0x005232ac,
	.BlackwaterCity = 0x00520ac4,
	.CommandCenter = 0x00520a44,
	.BlackwaterDocks = 0x00523284,
	.AquatosSewers = 0x005225c4,
	.MarcadiaPalace = 0x00521f04,
#endif
};