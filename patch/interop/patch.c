#include <libuya/interop.h>
#include <libuya/player.h>

//================================================
//=============   runCameraSpeedPatch
//================================================
variableAddressCompact_t vaUpdateCameraSpeedIGFunc = {
#ifdef UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004be190),
	.Hoven = VA_COMPACT_OFFSET(0x004be190, 0x004c02a8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004be190, 0x004b5b80),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004be190, 0x004b3318),
	.Metropolis = VA_COMPACT_OFFSET(0x004be190, 0x004b2668),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004be190, 0x004aff00),
	.CommandCenter = VA_COMPACT_OFFSET(0x004be190, 0x004afef8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004be190, 0x004b2778),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004be190, 0x004b1a78),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004be190, 0x004b13f8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004bbc78),
	.Hoven = VA_COMPACT_OFFSET(0x004bbc78, 0x004bdcd0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004bbc78, 0x004b35e8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004bbc78, 0x004b0e00),
	.Metropolis = VA_COMPACT_OFFSET(0x004bbc78, 0x004b0150),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004bbc78, 0x004ad968),
	.CommandCenter = VA_COMPACT_OFFSET(0x004bbc78, 0x004adb20),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004bbc78, 0x004b0360),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004bbc78, 0x004af6a0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004bbc78, 0x004aefe0),
#endif
};
variableAddressCompact_t vaDrawCameraSpeedInputIGFunc = {
#ifdef UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004be3a4),
	.Hoven = VA_COMPACT_OFFSET(0x004be3a4, 0x004c04bc),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004be3a4, 0x004b5d94),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004be3a4, 0x004b352c),
	.Metropolis = VA_COMPACT_OFFSET(0x004be3a4, 0x004b287c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004be3a4, 0x004b0114),
	.CommandCenter = VA_COMPACT_OFFSET(0x004be3a4, 0x004b010c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004be3a4, 0x004b298c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004be3a4, 0x004b1c8c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004be3a4, 0x004b160c),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004bbe8c),
	.Hoven = VA_COMPACT_OFFSET(0x004bbe8c, 0x004bdee4),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004bbe8c, 0x004b37fc),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004bbe8c, 0x004b1014),
	.Metropolis = VA_COMPACT_OFFSET(0x004bbe8c, 0x004b0364),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004bbe8c, 0x004adb7c),
	.CommandCenter = VA_COMPACT_OFFSET(0x004bbe8c, 0x004add34),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004bbe8c, 0x004b0574),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004bbe8c, 0x004af8b4),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004bbe8c, 0x004af1f4),
#endif
};

//================================================
//=============   patchKillStealing
//================================================
variableAddressCompact_t vaWhoHitMeFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004f8890),
	.Hoven = VA_COMPACT_OFFSET(0x004f8890, 0x004fa9a8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f8890, 0x004f0280),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f8890, 0x004eda18),
	.Metropolis = VA_COMPACT_OFFSET(0x004f8890, 0x004ecd68),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f8890, 0x004ea600),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f8890, 0x004ea5c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f8890, 0x004ece48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f8890, 0x004ec148),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f8890, 0x004ebac8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004f6110),
	.Hoven = VA_COMPACT_OFFSET(0x004f6110, 0x004f8168),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f6110, 0x004eda80),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f6110, 0x004eb298),
	.Metropolis = VA_COMPACT_OFFSET(0x004f6110, 0x004ea5e8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f6110, 0x004e7e00),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f6110, 0x004e7f88),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f6110, 0x004ea7c8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f6110, 0x004e9b08),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f6110, 0x004e9448),
#endif
};
variableAddressCompact_t vaWhoHitMeHook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004f95d8),
	.Hoven = VA_COMPACT_OFFSET(0x004f95d8, 0x004fb6f0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f95d8, 0x004f0fc8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f95d8, 0x004ee760),
	.Metropolis = VA_COMPACT_OFFSET(0x004f95d8, 0x004edab0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f95d8, 0x004eb348),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f95d8, 0x004eb310),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f95d8, 0x004edb90),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f95d8, 0x004ece90),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f95d8, 0x004ec810),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004f6e58),
	.Hoven = VA_COMPACT_OFFSET(0x004f6e58, 0x004f8eb0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f6e58, 0x004ee7c8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f6e58, 0x004ebfe0),
	.Metropolis = VA_COMPACT_OFFSET(0x004f6e58, 0x004eb330),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f6e58, 0x004e8b48),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f6e58, 0x004e8cd0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f6e58, 0x004eb510),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f6e58, 0x004ea850),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f6e58, 0x004ea190),
#endif
};

//================================================
//=============   patchDeadShooting
//================================================
variableAddressCompact_t vaPatchDeadShooting_ShootingFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00495708),
	.Hoven = VA_COMPACT_OFFSET(0x00495708, 0x00497820),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00495708, 0x0048d0f8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00495708, 0x0048a7c8),
	.Metropolis = VA_COMPACT_OFFSET(0x00495708, 0x00489be0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00495708, 0x00487478),
	.CommandCenter = VA_COMPACT_OFFSET(0x00495708, 0x00487470),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00495708, 0x00489cf0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00495708, 0x00488ff0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00495708, 0x00488970),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00493718),
	.Hoven = VA_COMPACT_OFFSET(0x00493718, 0x00495770),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00493718, 0x0048b088),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00493718, 0x004887d8),
	.Metropolis = VA_COMPACT_OFFSET(0x00493718, 0x00487bf0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00493718, 0x00485408),
	.CommandCenter = VA_COMPACT_OFFSET(0x00493718, 0x004855c0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00493718, 0x00487e00),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00493718, 0x00487140),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00493718, 0x00486a80),
#endif
};
variableAddressCompact_t vaPatchDeadShooting_ShootingHook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00532a0c),
	.Hoven = VA_COMPACT_OFFSET(0x00532a0c, 0x00534b24),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00532a0c, 0x0052a3fc),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00532a0c, 0x00527b94),
	.Metropolis = VA_COMPACT_OFFSET(0x00532a0c, 0x00526ee4),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00532a0c, 0x0052477c),
	.CommandCenter = VA_COMPACT_OFFSET(0x00532a0c, 0x0052453c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00532a0c, 0x00526dbc),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00532a0c, 0x005260bc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00532a0c, 0x00525a3c),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0053018c),
	.Hoven = VA_COMPACT_OFFSET(0x0053018c, 0x005321e4),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0053018c, 0x00527afc),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0053018c, 0x00525314),
	.Metropolis = VA_COMPACT_OFFSET(0x0053018c, 0x00524664),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0053018c, 0x00521e7c),
	.CommandCenter = VA_COMPACT_OFFSET(0x0053018c, 0x00521dfc),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0053018c, 0x0052463c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0053018c, 0x0052397c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0053018c, 0x005232bc),
#endif
};

//================================================
//=============   patchSniperWallSniping
//================================================
variableAddressCompact_t vaSniperShotCollLineFixHook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00408bc0),
	.Hoven = VA_COMPACT_OFFSET(0x00408bc0, 0x00408528),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00408bc0, 0x00400420),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00408bc0, 0x003ff800),
	.Metropolis = VA_COMPACT_OFFSET(0x00408bc0, 0x003fe3c0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00408bc0, 0x003fb1a8),
	.CommandCenter = VA_COMPACT_OFFSET(0x00408bc0, 0x004095b0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00408bc0, 0x0040b510),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00408bc0, 0x0040b118),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00408bc0, 0x0040a190),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00408540),
	.Hoven = VA_COMPACT_OFFSET(0x00408540, 0x00407e28),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00408540, 0x003ffd20),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00408540, 0x003ff160),
	.Metropolis = VA_COMPACT_OFFSET(0x00408540, 0x003fdd40),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00408540, 0x003faac8),
	.CommandCenter = VA_COMPACT_OFFSET(0x00408540, 0x00408f18),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00408540, 0x0040ae78),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00408540, 0x0040aa80),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00408540, 0x00409af8),
#endif
};
variableAddressCompact_t vaSniperShotCreatedHook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00408508),
	.Hoven = VA_COMPACT_OFFSET(0x00408508, 0x00407e70),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00408508, 0x003ffd68),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00408508, 0x003ff148),
	.Metropolis = VA_COMPACT_OFFSET(0x00408508, 0x003fdd08),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00408508, 0x003faaf0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00408508, 0x00408ef8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00408508, 0x0040ae58),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00408508, 0x0040aa60),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00408508, 0x00409ad8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00407e90),
	.Hoven = VA_COMPACT_OFFSET(0x00407e90, 0x00407778),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00407e90, 0x003ff670),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00407e90, 0x003feab0),
	.Metropolis = VA_COMPACT_OFFSET(0x00407e90, 0x003fd690),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00407e90, 0x003fa418),
	.CommandCenter = VA_COMPACT_OFFSET(0x00407e90, 0x00408868),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00407e90, 0x0040a7c8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00407e90, 0x0040a3d0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00407e90, 0x00409448),
#endif
};

//================================================
//=============  patchSniperNiking
//================================================
variableAddressCompact_t vaGetSniperShotDirectionFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0045d9d0),
	.Hoven = VA_COMPACT_OFFSET(0x0045d9d0, 0x0045f580),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045d9d0, 0x00456380),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045d9d0, 0x00453f10),
	.Metropolis = VA_COMPACT_OFFSET(0x0045d9d0, 0x00453250),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045d9d0, 0x00450a80),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045d9d0, 0x00451378),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045d9d0, 0x00453bf8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045d9d0, 0x00452ef8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045d9d0, 0x00452878),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0045c920),
	.Hoven = VA_COMPACT_OFFSET(0x0045c920, 0x0045e410),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045c920, 0x00455250),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045c920, 0x00452e60),
	.Metropolis = VA_COMPACT_OFFSET(0x0045c920, 0x004521a0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045c920, 0x0044f950),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045c920, 0x00450408),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045c920, 0x00452c48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045c920, 0x00451f88),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045c920, 0x004518c8),
#endif
};
variableAddressCompact_t vaGetSniperShotDirectionHook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00407394),
	.Hoven = VA_COMPACT_OFFSET(0x00407394, 0x00406cfc),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00407394, 0x003febf4),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00407394, 0x003fdfd4),
	.Metropolis = VA_COMPACT_OFFSET(0x00407394, 0x003fcb94),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00407394, 0x003f997c),
	.CommandCenter = VA_COMPACT_OFFSET(0x00407394, 0x00407d84),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00407394, 0x00409ce4),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00407394, 0x004098ec),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00407394, 0x00408964),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00406d2c),
	.Hoven = VA_COMPACT_OFFSET(0x00406d2c, 0x00406614),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00406d2c, 0x003fe50c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00406d2c, 0x003fd94c),
	.Metropolis = VA_COMPACT_OFFSET(0x00406d2c, 0x003fc52c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00406d2c, 0x003f92b4),
	.CommandCenter = VA_COMPACT_OFFSET(0x00406d2c, 0x00407704),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00406d2c, 0x00409664),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00406d2c, 0x0040926c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00406d2c, 0x004082e4),
#endif
};

//================================================
//=============  patchWeaponShotLag
//================================================
variableAddressCompact_t vaAllWeaponsUDPtoTCP = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00546760),
	.Hoven = VA_COMPACT_OFFSET(0x00546760, 0x00548928),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00546760, 0x0053e200),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00546760, 0x0053b8e8),
	.Metropolis = VA_COMPACT_OFFSET(0x00546760, 0x0053ace8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00546760, 0x005384d0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00546760, 0x00537d28),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00546760, 0x0053a5a8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00546760, 0x005398a8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00546760, 0x00539228),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00543e54),
	.Hoven = VA_COMPACT_OFFSET(0x00543e54, 0x00545f5c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00543e54, 0x0053b874),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00543e54, 0x00538fdc),
	.Metropolis = VA_COMPACT_OFFSET(0x00543e54, 0x005383dc),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00543e54, 0x00535b44),
	.CommandCenter = VA_COMPACT_OFFSET(0x00543e54, 0x00535574),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00543e54, 0x00537db4),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00543e54, 0x005370f4),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00543e54, 0x00536a34),
#endif
};
variableAddressCompact_t vaFluxUDPtoTCP = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0040931c),
	.Hoven = VA_COMPACT_OFFSET(0x0040931c, 0x00408c84),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0040931c, 0x00400b7c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0040931c, 0x003fff5c),
	.Metropolis = VA_COMPACT_OFFSET(0x0040931c, 0x003feb1c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0040931c, 0x003fb904),
	.CommandCenter = VA_COMPACT_OFFSET(0x0040931c, 0x00409d0c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0040931c, 0x0040bc6c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0040931c, 0x0040b874),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0040931c, 0x0040a8ec),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00408c7c),
	.Hoven = VA_COMPACT_OFFSET(0x00408c7c, 0x00408564),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00408c7c, 0x0040045c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00408c7c, 0x003ff89c),
	.Metropolis = VA_COMPACT_OFFSET(0x00408c7c, 0x003fe47c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00408c7c, 0x003fb204),
	.CommandCenter = VA_COMPACT_OFFSET(0x00408c7c, 0x00409654),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00408c7c, 0x0040b5b4),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00408c7c, 0x0040b1bc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00408c7c, 0x0040a234),
#endif
};

//================================================
//=============  patchLevelOfDetail
//================================================
variableAddressCompact_t vaLevelOfDetail_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004c9018),
	.Hoven = VA_COMPACT_OFFSET(0x004c9018, 0x004cb130),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004c9018, 0x004c0a08),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004c9018, 0x004be1a0),
	.Metropolis = VA_COMPACT_OFFSET(0x004c9018, 0x004bd4f0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004c9018, 0x004bad88),
	.CommandCenter = VA_COMPACT_OFFSET(0x004c9018, 0x004bad80),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004c9018, 0x004bd600),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004c9018, 0x004bc900),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004c9018, 0x004bc280),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004c68d8),
	.Hoven = VA_COMPACT_OFFSET(0x004c68d8, 0x004c8930),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004c68d8, 0x004be248),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004c68d8, 0x004bba60),
	.Metropolis = VA_COMPACT_OFFSET(0x004c68d8, 0x004badb0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004c68d8, 0x004b85c8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004c68d8, 0x004b8780),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004c68d8, 0x004bafc0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004c68d8, 0x004ba300),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004c68d8, 0x004b9c40),
#endif
};
variableAddressCompact_t vaLevelOfDetail_Shrubs = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00248e18),
	.Hoven = VA_COMPACT_OFFSET(0x00248e18, 0x00249018),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00248e18, 0x00248f08),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00248e18, 0x00248d88),
	.Metropolis = VA_COMPACT_OFFSET(0x00248e18, 0x00248e08),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00248e18, 0x00248d88),
	.CommandCenter = VA_COMPACT_OFFSET(0x00248e18, 0x00248988),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00248e18, 0x00248a88),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00248e18, 0x00248a88),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00248e18, 0x00248a88),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00248f98),
	.Hoven = VA_COMPACT_OFFSET(0x00248f98, 0x00249198),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00248f98, 0x00249088),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00248f98, 0x00248f08),
	.Metropolis = VA_COMPACT_OFFSET(0x00248f98, 0x00248f88),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00248f98, 0x00248f08),
	.CommandCenter = VA_COMPACT_OFFSET(0x00248f98, 0x00248b08),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00248f98, 0x00248c08),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00248f98, 0x00248c08),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00248f98, 0x00248c08),
#endif
};
variableAddressCompact_t vaLevelOfDetail_Terrain = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00248ee0),
	.Hoven = VA_COMPACT_OFFSET(0x00248ee0, 0x002490e0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00248ee0, 0x00248fd0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00248ee0, 0x00248e50),
	.Metropolis = VA_COMPACT_OFFSET(0x00248ee0, 0x00248ed0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00248ee0, 0x00248e50),
	.CommandCenter = VA_COMPACT_OFFSET(0x00248ee0, 0x00248a50),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00248ee0, 0x00248b50),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00248ee0, 0x00248b50),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00248ee0, 0x00248b50),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00249060),
	.Hoven = VA_COMPACT_OFFSET(0x00249060, 0x00249260),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00249060, 0x00249150),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00249060, 0x00248fd0),
	.Metropolis = VA_COMPACT_OFFSET(0x00249060, 0x00249050),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00249060, 0x00248fd0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00249060, 0x00248bd0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00249060, 0x00248cd0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00249060, 0x00248cd0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00249060, 0x00248cd0),
#endif
};

variableAddressCompact_t vaLevelOfDetail_Ties = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00248f44),
	.Hoven = VA_COMPACT_OFFSET(0x00248f44, 0x00249144),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00248f44, 0x00249034),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00248f44, 0x00248eb4),
	.Metropolis = VA_COMPACT_OFFSET(0x00248f44, 0x00248f34),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00248f44, 0x00248eb4),
	.CommandCenter = VA_COMPACT_OFFSET(0x00248f44, 0x00248ab4),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00248f44, 0x00248bb4),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00248f44, 0x00248bb4),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00248f44, 0x00248bb4),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x002490c4),
	.Hoven = VA_COMPACT_OFFSET(0x002490c4, 0x002492c4),
	.OutpostX12 = VA_COMPACT_OFFSET(0x002490c4, 0x002491b4),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x002490c4, 0x00249034),
	.Metropolis = VA_COMPACT_OFFSET(0x002490c4, 0x002490b4),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x002490c4, 0x00249034),
	.CommandCenter = VA_COMPACT_OFFSET(0x002490c4, 0x00248c34),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x002490c4, 0x00248d34),
	.AquatosSewers = VA_COMPACT_OFFSET(0x002490c4, 0x00248d34),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x002490c4, 0x00248d34),
#endif
};

//================================================
//=============  runFpsCounter
//================================================
variableAddressCompact_t vaFpsCounter_DrawFunc = {
#if UYA_PAL
	.Lobby = 0x00589410,
	.Bakisi = VA_COMPACT_BASE(0x00456328),
	.Hoven = VA_COMPACT_OFFSET(0x00456328, 0x00457ea8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00456328, 0x0044eca8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00456328, 0x0044c868),
	.Metropolis = VA_COMPACT_OFFSET(0x00456328, 0x0044bba8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00456328, 0x004493a8),
	.CommandCenter = VA_COMPACT_OFFSET(0x00456328, 0x0044a028),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00456328, 0x0044c8a8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00456328, 0x0044bba8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00456328, 0x0044b528),
#else
	.Lobby = 0x005882a0,
	.Bakisi = VA_COMPACT_BASE(0x004552b8),
	.Hoven = VA_COMPACT_OFFSET(0x004552b8, 0x00456d78),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004552b8, 0x0044dbb8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004552b8, 0x0044b7f8),
	.Metropolis = VA_COMPACT_OFFSET(0x004552b8, 0x0044ab38),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004552b8, 0x004482b8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004552b8, 0x004490f8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004552b8, 0x0044b938),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004552b8, 0x0044ac78),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004552b8, 0x0044a5b8),
#endif
};
variableAddressCompact_t vaFpsCounter_UpdateFunc = {
#if UYA_PAL
	.Lobby = 0x005fbf18,
	.Bakisi = VA_COMPACT_BASE(0x004ce660),
	.Hoven = VA_COMPACT_OFFSET(0x004ce660, 0x004d0778),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ce660, 0x004c6050),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ce660, 0x004c37e8),
	.Metropolis = VA_COMPACT_OFFSET(0x004ce660, 0x004c2b38),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ce660, 0x004c03d0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ce660, 0x004c03c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ce660, 0x004c2c48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ce660, 0x004c1f48),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ce660, 0x004c18c8),
#else
	.Lobby = 0x005f9780,
	.Bakisi = VA_COMPACT_BASE(0x004cbf20),
	.Hoven = VA_COMPACT_OFFSET(0x004cbf20, 0x004cdf78),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004cbf20, 0x004c3890),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004cbf20, 0x004c10a8),
	.Metropolis = VA_COMPACT_OFFSET(0x004cbf20, 0x004c03f8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004cbf20, 0x004bdc10),
	.CommandCenter = VA_COMPACT_OFFSET(0x004cbf20, 0x004bddc8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004cbf20, 0x004c0608),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004cbf20, 0x004bf948),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004cbf20, 0x004bf288),
#endif
};
variableAddressCompact_t vaFpsCounter_Hooks = {
#if UYA_PAL
	.Lobby = 0x00575fc8,
	.Bakisi = VA_COMPACT_BASE(0x004428c8),
	.Hoven = VA_COMPACT_OFFSET(0x004428c8, 0x00444448),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004428c8, 0x0043b248),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004428c8, 0x00438e08),
	.Metropolis = VA_COMPACT_OFFSET(0x004428c8, 0x00438148),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004428c8, 0x00435948),
	.CommandCenter = VA_COMPACT_OFFSET(0x004428c8, 0x004365c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004428c8, 0x00438e48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004428c8, 0x00438148),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004428c8, 0x00437ac8),
#else
	.Lobby = 0x00575288,
	.Bakisi = VA_COMPACT_BASE(0x00441c88),
	.Hoven = VA_COMPACT_OFFSET(0x00441c88, 0x00443748),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00441c88, 0x0043a588),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00441c88, 0x004381c8),
	.Metropolis = VA_COMPACT_OFFSET(0x00441c88, 0x00437508),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00441c88, 0x00434c88),
	.CommandCenter = VA_COMPACT_OFFSET(0x00441c88, 0x00435ac8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00441c88, 0x00438308),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00441c88, 0x00437648),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00441c88, 0x00436f88),
#endif
};

//================================================
//=============  setupPatchConfigInGame
//================================================
variableAddressCompact_t vaPauseMenuAddr = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x003c073c),
	.Hoven = VA_COMPACT_OFFSET(0x003c073c, 0x003bfd7c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003c073c, 0x003b7c7c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003c073c, 0x003b7a3c),
	.Metropolis = VA_COMPACT_OFFSET(0x003c073c, 0x003b77bc),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003c073c, 0x003b79fc),
	.CommandCenter = VA_COMPACT_OFFSET(0x003c073c, 0x003c7f7c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003c073c, 0x003c837c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003c073c, 0x003c8afc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003c073c, 0x003c807c),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x003c087c),
	.Hoven = VA_COMPACT_OFFSET(0x003c087c, 0x003bfebc),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003c087c, 0x003b7dbc),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003c087c, 0x003b7b7c),
	.Metropolis = VA_COMPACT_OFFSET(0x003c087c, 0x003b78fc),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003c087c, 0x003b7b3c),
	.CommandCenter = VA_COMPACT_OFFSET(0x003c087c, 0x003c80bc),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003c087c, 0x003c84bc),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003c087c, 0x003c8c3c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003c087c, 0x003c81bc),
#endif
};

//================================================
//=============  patchAlwaysShowHealthBar
//================================================
variableAddressCompact_t vaHealthBarTimerSaveZero = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00548020),
	.Hoven = VA_COMPACT_OFFSET(0x00548020, 0x0054a1e8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00548020, 0x0053fac0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00548020, 0x0053c5a8),
	.Metropolis = VA_COMPACT_OFFSET(0x00548020, 0x0053c5a8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00548020, 0x00539d90),
	.CommandCenter = VA_COMPACT_OFFSET(0x00548020, 0x005395e8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00548020, 0x0053be68),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00548020, 0x0053b168),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00548020, 0x0053aae8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00545718),
	.Hoven = VA_COMPACT_OFFSET(0x00545718, 0x00547820),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00545718, 0x0053d138),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00545718, 0x0053a8a0),
	.Metropolis = VA_COMPACT_OFFSET(0x00545718, 0x00539ca0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00545718, 0x00537408),
	.CommandCenter = VA_COMPACT_OFFSET(0x00545718, 0x00536e38),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00545718, 0x00539678),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00545718, 0x005389b8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00545718, 0x005382f8),
#endif
};

//================================================
//=============  patchMapAndScoreboardToggle
//================================================
variableAddressCompact_t vaMapScore_SelectBtn_Addr = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005480fc),
	.Hoven = VA_COMPACT_OFFSET(0x005480fc, 0x0054a2c4),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005480fc, 0x0053fb9c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005480fc, 0x0053d284),
	.Metropolis = VA_COMPACT_OFFSET(0x005480fc, 0x0053c684),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005480fc, 0x00539e6c),
	.CommandCenter = VA_COMPACT_OFFSET(0x005480fc, 0x005396c4),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005480fc, 0x0053bf44),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005480fc, 0x0053b244),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005480fc, 0x0053abc4),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x005457f4),
	.Hoven = VA_COMPACT_OFFSET(0x005457f4, 0x005478fc),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005457f4, 0x0053d214),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005457f4, 0x0053a97c),
	.Metropolis = VA_COMPACT_OFFSET(0x005457f4, 0x00539d7c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005457f4, 0x005374e4),
	.CommandCenter = VA_COMPACT_OFFSET(0x005457f4, 0x00536f14),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005457f4, 0x00539754),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005457f4, 0x00538a94),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005457f4, 0x005383d4),
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
variableAddressCompact_t vaMapScore_SeigeCTFMap_AlwaysRun = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004ae978),
	.Hoven = VA_COMPACT_OFFSET(0x004ae978, 0x004b0a90),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ae978, 0x004a6368),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ae978, 0x004a3b00),
	.Metropolis = VA_COMPACT_OFFSET(0x004ae978, 0x004a2e50),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ae978, 0x004a06e8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ae978, 0x004a06e0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ae978, 0x004a2f60),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ae978, 0x004a2260),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ae978, 0x004a1be0),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004ac428),
	.Hoven = VA_COMPACT_OFFSET(0x004ac428, 0x004ae480),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ac428, 0x004a3d98),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ac428, 0x004a15b0),
	.Metropolis = VA_COMPACT_OFFSET(0x004ac428, 0x004a0900),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ac428, 0x0049e118),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ac428, 0x0049e2d0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ac428, 0x004a0b10),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ac428, 0x0049fe50),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ac428, 0x0049f790),
#endif
};
variableAddressCompact_t vaMapScore_SeigeCTFScoreboard_AlwaysRun = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004b16a8),
	.Hoven = VA_COMPACT_OFFSET(0x004b16a8, 0x004b37c0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004b16a8, 0x004a9098),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004b16a8, 0x004a6830),
	.Metropolis = VA_COMPACT_OFFSET(0x004b16a8, 0x004a5b80),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004b16a8, 0x004a3418),
	.CommandCenter = VA_COMPACT_OFFSET(0x004b16a8, 0x004a3410),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004b16a8, 0x004a5c90),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004b16a8, 0x004a4f90),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004b16a8, 0x004a4910),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004af158),
	.Hoven = VA_COMPACT_OFFSET(0x004af158, 0x004b11b0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004af158, 0x004a6ac8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004af158, 0x004a42e0),
	.Metropolis = VA_COMPACT_OFFSET(0x004af158, 0x004a3630),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004af158, 0x004a0e48),
	.CommandCenter = VA_COMPACT_OFFSET(0x004af158, 0x004a1000),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004af158, 0x004a3840),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004af158, 0x004a2b80),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004af158, 0x004a24c0),
#endif
};
variableAddressCompact_t vaMapScore_MapToggle = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004ae960),
	.Hoven = VA_COMPACT_OFFSET(0x004ae960, 0x004b0a78),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ae960, 0x004a6350),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ae960, 0x004a3ae8),
	.Metropolis = VA_COMPACT_OFFSET(0x004ae960, 0x004a2e38),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ae960, 0x004a06d0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ae960, 0x004a06c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ae960, 0x004a2f48),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ae960, 0x004a2248),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ae960, 0x004a1bc8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004ac410),
	.Hoven = VA_COMPACT_OFFSET(0x004ac410, 0x004ae468),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ac410, 0x004a3d80),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ac410, 0x004a1598),
	.Metropolis = VA_COMPACT_OFFSET(0x004ac410, 0x004a08e8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ac410, 0x0049e100),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ac410, 0x0049e2b8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ac410, 0x004a0af8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ac410, 0x0049fe38),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ac410, 0x0049f778),
#endif
};
variableAddressCompact_t vaMapScore_ScoreboardToggle = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004b1690),
	.Hoven = VA_COMPACT_OFFSET(0x004b1690, 0x004b37a8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004b1690, 0x004a9080),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004b1690, 0x004a6818),
	.Metropolis = VA_COMPACT_OFFSET(0x004b1690, 0x004a5b68),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004b1690, 0x004a3400),
	.CommandCenter = VA_COMPACT_OFFSET(0x004b1690, 0x004a33f8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004b1690, 0x004a5c78),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004b1690, 0x004a4f78),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004b1690, 0x004a48f8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x004af140),
	.Hoven = VA_COMPACT_OFFSET(0x004af140, 0x004b1198),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004af140, 0x004a6ab0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004af140, 0x004a42c8),
	.Metropolis = VA_COMPACT_OFFSET(0x004af140, 0x004a3618),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004af140, 0x004a0e30),
	.CommandCenter = VA_COMPACT_OFFSET(0x004af140, 0x004a0fe8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004af140, 0x004a3828),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004af140, 0x004a2b68),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004af140, 0x004a24a8),
#endif
};

//================================================
//=============  patchQuickSelectTimer
//================================================
variableAddressCompact_t vaQuickSelectCheck_Hook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00549170),
	.Hoven = VA_COMPACT_OFFSET(0x00549170, 0x0054b338),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00549170, 0x00540c10),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00549170, 0x0053e2f8),
	.Metropolis = VA_COMPACT_OFFSET(0x00549170, 0x0053d6f8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00549170, 0x0053aee0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00549170, 0x0053a738),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00549170, 0x0053cfb8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00549170, 0x0053c2b8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00549170, 0x0053bc38),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00546818),
	.Hoven = VA_COMPACT_OFFSET(0x00546818, 0x00548920),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00546818, 0x0053e238),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00546818, 0x0053b9a0),
	.Metropolis = VA_COMPACT_OFFSET(0x00546818, 0x0053ada0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00546818, 0x00538508),
	.CommandCenter = VA_COMPACT_OFFSET(0x00546818, 0x00537f38),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00546818, 0x0053a778),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00546818, 0x00539ab8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00546818, 0x005393f8),
#endif
};
variableAddressCompact_t vaQuickSelectCheck_Func = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0054a8a8),
	.Hoven = VA_COMPACT_OFFSET(0x0054a8a8, 0x0054ca70),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0054a8a8, 0x00542348),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0054a8a8, 0x0053fa30),
	.Metropolis = VA_COMPACT_OFFSET(0x0054a8a8, 0x0053ee30),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0054a8a8, 0x0053c618),
	.CommandCenter = VA_COMPACT_OFFSET(0x0054a8a8, 0x0053be70),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0054a8a8, 0x0053e6f0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0054a8a8, 0x0053d9f0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0054a8a8, 0x0053d370),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00547f00),
	.Hoven = VA_COMPACT_OFFSET(0x00547f00, 0x0054a008),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00547f00, 0x0053f920),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00547f00, 0x0053d088),
	.Metropolis = VA_COMPACT_OFFSET(0x00547f00, 0x0053c488),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00547f00, 0x00539bf0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00547f00, 0x00539620),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00547f00, 0x0053be60),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00547f00, 0x0053b1a0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00547f00, 0x0053aae0),
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
variableAddressCompact_t vaSideFlipJoystickVal = {
#if UYA_PAL
	.Lobby = 0x0062d978,
	.Bakisi = VA_COMPACT_BASE(0x004ff870),
	.Hoven = VA_COMPACT_OFFSET(0x004ff870, 0x00501988),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ff870, 0x004f7260),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ff870, 0x004f49f8),
	.Metropolis = VA_COMPACT_OFFSET(0x004ff870, 0x004f3d48),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ff870, 0x004f15e0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ff870, 0x004f15a8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ff870, 0x004f3e28),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ff870, 0x004f3128),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ff870, 0x004f2aa8),
#else
	.Lobby = 0x0062b1a0,
	.Bakisi = VA_COMPACT_BASE(0x004fd0f0),
	.Hoven = VA_COMPACT_OFFSET(0x004fd0f0, 0x004ff148),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004fd0f0, 0x004f4a60),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004fd0f0, 0x004f2278),
	.Metropolis = VA_COMPACT_OFFSET(0x004fd0f0, 0x004f15c8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004fd0f0, 0x004eede0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004fd0f0, 0x004eef68),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004fd0f0, 0x004f17a8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004fd0f0, 0x004f0ae8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004fd0f0, 0x004f0428),
#endif
};  

variableAddressCompact_t vaZRotDiffSpherical = {
#if UYA_PAL
	.Lobby = 0x00633c78,
	.Bakisi = VA_COMPACT_BASE(0x00505b70),
	.Hoven = VA_COMPACT_OFFSET(0x00505b70, 0x00507c88),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00505b70, 0x004fd560),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00505b70, 0x004facf8),
	.Metropolis = VA_COMPACT_OFFSET(0x00505b70, 0x004fa048),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00505b70, 0x004f78e0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00505b70, 0x004f78a8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00505b70, 0x004fa128),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00505b70, 0x004f9428),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00505b70, 0x004f8da8),
#else
	.Lobby = 0x00631430,
	.Bakisi = VA_COMPACT_BASE(0x00503380),
	.Hoven = VA_COMPACT_OFFSET(0x00503380, 0x005053d8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00503380, 0x004facf0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00503380, 0x004f8508),
	.Metropolis = VA_COMPACT_OFFSET(0x00503380, 0x004f7858),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00503380, 0x004f5070),
	.CommandCenter = VA_COMPACT_OFFSET(0x00503380, 0x004f51f8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00503380, 0x004f7a38),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00503380, 0x004f6d78),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00503380, 0x004f66b8),
#endif
};

// typedef void (*ZRotDiffSpherical_Func)(Player* player, VECTOR *angleVec, int turnDir);
// ZRotDiffSpherical_Func ZRotDiffSpherical = (ZRotDiffSpherical_Func)GetAddress(&vaZRotDiffSpherical);



//================================================
//=============  playerSync
//================================================

variableAddressCompact_t vaOnMobyUpdate_Func = {
#if UYA_PAL
	.Lobby = 0x0052bca8,
	.Bakisi = VA_COMPACT_BASE(0x003db0e8),
	.Hoven = VA_COMPACT_OFFSET(0x003db0e8, 0x003dad08),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003db0e8, 0x003d2c08),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003db0e8, 0x003d3020),
	.Metropolis = VA_COMPACT_OFFSET(0x003db0e8, 0x003d0ba8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003db0e8, 0x003d2908),
	.CommandCenter = VA_COMPACT_OFFSET(0x003db0e8, 0x003dec18),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003db0e8, 0x003e1098),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003db0e8, 0x003e0780),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003db0e8, 0x003ded18),
#else
	.Lobby = 0x0052b740,
	.Bakisi = VA_COMPACT_BASE(0x003daec0),
	.Hoven = VA_COMPACT_OFFSET(0x003daec0, 0x003daa60),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003daec0, 0x003d2960),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003daec0, 0x003d2dd8),
	.Metropolis = VA_COMPACT_OFFSET(0x003daec0, 0x003d0980),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003daec0, 0x003d2660),
	.CommandCenter = VA_COMPACT_OFFSET(0x003daec0, 0x003de9b0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003daec0, 0x003e0e30),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003daec0, 0x003e0518),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003daec0, 0x003deab0),
#endif
};

variableAddressCompact_t vaPlayerSyncFixOverflow1 = {
#if UYA_PAL
	.Lobby = 0x005ac8b8,
	.Bakisi = VA_COMPACT_BASE(0x00479170),
	.Hoven = VA_COMPACT_OFFSET(0x00479170, 0x0047ad30),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00479170, 0x00471b30),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00479170, 0x0046f6b0),
	.Metropolis = VA_COMPACT_OFFSET(0x00479170, 0x0046e9f0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00479170, 0x0046c230),
	.CommandCenter = VA_COMPACT_OFFSET(0x00479170, 0x0046caf0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00479170, 0x0046f370),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00479170, 0x0046e670),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00479170, 0x0046dff0),
#else
	.Lobby = 0x005aa9b8,
	.Bakisi = VA_COMPACT_BASE(0x004773b0),
	.Hoven = VA_COMPACT_OFFSET(0x004773b0, 0x00478eb0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004773b0, 0x0046fcf0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004773b0, 0x0046d8f0),
	.Metropolis = VA_COMPACT_OFFSET(0x004773b0, 0x0046cc30),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004773b0, 0x0046a3f0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004773b0, 0x0046ae70),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004773b0, 0x0046d6b0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004773b0, 0x0046c9f0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004773b0, 0x0046c330),
#endif
};

variableAddressCompact_t vaPlayerSyncFixOverflow2 = {
#if UYA_PAL
	.Lobby = 0x00580418,
	.Bakisi = VA_COMPACT_BASE(0x0044d330),
	.Hoven = VA_COMPACT_OFFSET(0x0044d330, 0x0044eeb0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044d330, 0x00445cb0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044d330, 0x00443870),
	.Metropolis = VA_COMPACT_OFFSET(0x0044d330, 0x00442bb0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044d330, 0x004403b0),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044d330, 0x00441030),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044d330, 0x004438b0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044d330, 0x00442bb0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044d330, 0x00442530),
#else
	.Lobby = 0x0057f470,
	.Bakisi = VA_COMPACT_BASE(0x0044c488),
	.Hoven = VA_COMPACT_OFFSET(0x0044c488, 0x0044df48),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044c488, 0x00444d88),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044c488, 0x004429c8),
	.Metropolis = VA_COMPACT_OFFSET(0x0044c488, 0x00441d08),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044c488, 0x0043f488),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044c488, 0x004402c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044c488, 0x00442b08),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044c488, 0x00441e48),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044c488, 0x00441788),
#endif
};

variableAddressCompact_t vaPlayerSyncFixOverflow3 = {
#if UYA_PAL
	.Lobby = 0x00580424,
	.Bakisi = VA_COMPACT_BASE(0x0044d33c),
	.Hoven = VA_COMPACT_OFFSET(0x0044d33c, 0x0044eebc),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044d33c, 0x00445cbc),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044d33c, 0x0044387c),
	.Metropolis = VA_COMPACT_OFFSET(0x0044d33c, 0x00442bbc),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044d33c, 0x004403bc),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044d33c, 0x0044103c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044d33c, 0x004438bc),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044d33c, 0x00442bbc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044d33c, 0x0044253c),
#else
	.Lobby = 0x0057f47c,
	.Bakisi = VA_COMPACT_BASE(0x0044c494),
	.Hoven = VA_COMPACT_OFFSET(0x0044c494, 0x0044df54),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044c494, 0x00444d94),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044c494, 0x004429d4),
	.Metropolis = VA_COMPACT_OFFSET(0x0044c494, 0x00441d14),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044c494, 0x0043f494),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044c494, 0x004402d4),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044c494, 0x00442b14),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044c494, 0x00441e54),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044c494, 0x00441794),
#endif
};

variableAddressCompact_t vaPlayerSyncFixOverflow4 = {
#if UYA_PAL
	.Lobby = 0x00580440,
	.Bakisi = VA_COMPACT_BASE(0x0044d358),
	.Hoven = VA_COMPACT_OFFSET(0x0044d358, 0x0044eed8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044d358, 0x00445cd8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044d358, 0x00443898),
	.Metropolis = VA_COMPACT_OFFSET(0x0044d358, 0x00442bd8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044d358, 0x004403d8),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044d358, 0x00441058),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044d358, 0x004438d8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044d358, 0x00442bd8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044d358, 0x00442558),
#else
	.Lobby = 0x0057f498,
	.Bakisi = VA_COMPACT_BASE(0x0044c4b0),
	.Hoven = VA_COMPACT_OFFSET(0x0044c4b0, 0x0044df70),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0044c4b0, 0x00444db0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0044c4b0, 0x004429f0),
	.Metropolis = VA_COMPACT_OFFSET(0x0044c4b0, 0x00441d30),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0044c4b0, 0x0043f4b0),
	.CommandCenter = VA_COMPACT_OFFSET(0x0044c4b0, 0x004402f0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0044c4b0, 0x00442b30),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0044c4b0, 0x00441e70),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0044c4b0, 0x004417b0),
#endif
};

variableAddressCompact_t vaPlayerSyncFixLagout1 = {
#if UYA_PAL
	.Lobby = 0x00666240,
	.Bakisi = VA_COMPACT_BASE(0x00538340),
	.Hoven = VA_COMPACT_OFFSET(0x00538340, 0x0053a458),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00538340, 0x0052fd30),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00538340, 0x0052d4c8),
	.Metropolis = VA_COMPACT_OFFSET(0x00538340, 0x0052c818),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00538340, 0x0052a0b0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00538340, 0x00529e70),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00538340, 0x0052c6f0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00538340, 0x0052b9f0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00538340, 0x0052b370),
#else
	.Lobby = 0x00663968,
	.Bakisi = VA_COMPACT_BASE(0x00535ac0),
	.Hoven = VA_COMPACT_OFFSET(0x00535ac0, 0x00537b18),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00535ac0, 0x0052d430),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00535ac0, 0x0052ac48),
	.Metropolis = VA_COMPACT_OFFSET(0x00535ac0, 0x00529f98),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00535ac0, 0x005277b0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00535ac0, 0x00527730),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00535ac0, 0x00529f70),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00535ac0, 0x005292b0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00535ac0, 0x00528bf0),
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



variableAddressCompact_t vaNWUpdate_Func = {
#if UYA_PAL
	.Lobby = 0x001959f8,
	.Bakisi = VA_COMPACT_BASE(0x001959f8),
	.Hoven = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
	.Metropolis = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
	.CommandCenter = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x001959f8, 0x001959f8),
#else
	.Lobby = 0x00195ae8,
	.Bakisi = VA_COMPACT_BASE(0x00195ae8),
	.Hoven = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
	.Metropolis = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
	.CommandCenter = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00195ae8, 0x00195ae8),
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
variableAddressCompact_t vaGadgetEventFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00546510),
	.Hoven = VA_COMPACT_OFFSET(0x00546510, 0x005486d8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00546510, 0x0053dfb0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00546510, 0x0053b698),
	.Metropolis = VA_COMPACT_OFFSET(0x00546510, 0x0053aa98),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00546510, 0x00538280),
	.CommandCenter = VA_COMPACT_OFFSET(0x00546510, 0x00537ad8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00546510, 0x0053a358),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00546510, 0x00539658),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00546510, 0x00538fd8),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00543c00),
	.Hoven = VA_COMPACT_OFFSET(0x00543c00, 0x00545d08),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00543c00, 0x0053b620),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00543c00, 0x00538d88),
	.Metropolis = VA_COMPACT_OFFSET(0x00543c00, 0x00538188),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00543c00, 0x005358f0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00543c00, 0x00535320),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00543c00, 0x00537b60),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00543c00, 0x00536ea0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00543c00, 0x005367e0),
#endif
};
variableAddressCompact_t vaGadgetEventHook = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x0054b23c),
	.Hoven = VA_COMPACT_OFFSET(0x0054b23c, 0x0054d404),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0054b23c, 0x00542cdc),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0054b23c, 0x005403c4),
	.Metropolis = VA_COMPACT_OFFSET(0x0054b23c, 0x0053f7c4),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0054b23c, 0x0053cfac),
	.CommandCenter = VA_COMPACT_OFFSET(0x0054b23c, 0x0053c804),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0054b23c, 0x0053f084),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0054b23c, 0x0053e384),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0054b23c, 0x0053dd04),
#else
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00548894),
	.Hoven = VA_COMPACT_OFFSET(0x00548894, 0x0054a99c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00548894, 0x005402b4),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00548894, 0x0053da1c),
	.Metropolis = VA_COMPACT_OFFSET(0x00548894, 0x0053ce1c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00548894, 0x0053a584),
	.CommandCenter = VA_COMPACT_OFFSET(0x00548894, 0x00539fb4),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00548894, 0x0053c7f4),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00548894, 0x0053bb34),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00548894, 0x0053b474),
#endif
};

variableAddressCompact_t vaWaitingForResponse_Addr = {
#if UYA_PAL
	.Lobby = 0x0065f554,
	.Bakisi = VA_COMPACT_BASE(0x00531654),
	.Hoven = VA_COMPACT_OFFSET(0x00531654, 0x0053376c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00531654, 0x00529044),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00531654, 0x005267dc),
	.Metropolis = VA_COMPACT_OFFSET(0x00531654, 0x00525b2c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00531654, 0x005233c4),
	.CommandCenter = VA_COMPACT_OFFSET(0x00531654, 0x00523184),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00531654, 0x00525a04),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00531654, 0x00524d04),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00531654, 0x00524684),
#else
	.Lobby = 0x0065cc7c,
	.Bakisi = VA_COMPACT_BASE(0x0052edd4),
	.Hoven = VA_COMPACT_OFFSET(0x0052edd4, 0x00530e2c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0052edd4, 0x00526744),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0052edd4, 0x00523f5c),
	.Metropolis = VA_COMPACT_OFFSET(0x0052edd4, 0x005232ac),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0052edd4, 0x00520ac4),
	.CommandCenter = VA_COMPACT_OFFSET(0x0052edd4, 0x00520a44),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0052edd4, 0x00523284),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0052edd4, 0x005225c4),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0052edd4, 0x00521f04),
#endif
};

//================================================
//=============  patchSwingshotGunBug
//================================================
variableAddressCompact_t vaPatchSwingshotGunBug_Hook = {
#ifdef UYA_PAL
	.Lobby = 0x0062a434,
	.Bakisi = VA_COMPACT_BASE(0x004fc32c),
	.Hoven = VA_COMPACT_OFFSET(0x004fc32c, 0x004fe444),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004fc32c, 0x004f3d1c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004fc32c, 0x004f14b4),
	.Metropolis = VA_COMPACT_OFFSET(0x004fc32c, 0x004f0804),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004fc32c, 0x004ee09c),
	.CommandCenter = VA_COMPACT_OFFSET(0x004fc32c, 0x004ee064),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004fc32c, 0x004f08e4),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004fc32c, 0x004efbe4),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004fc32c, 0x004ef564),
#else
	.Lobby = 0x00627c5c,
	.Bakisi = VA_COMPACT_BASE(0x004f9bac),
	.Hoven = VA_COMPACT_OFFSET(0x004f9bac, 0x004fbc04),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f9bac, 0x004f151c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f9bac, 0x004eed34),
	.Metropolis = VA_COMPACT_OFFSET(0x004f9bac, 0x004ee084),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f9bac, 0x004eb89c),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f9bac, 0x004eba24),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f9bac, 0x004ee264),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f9bac, 0x004ed5a4),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f9bac, 0x004ecee4),
#endif
};

variableAddressCompact_t vaSetTextureArrow_Hook = {
#if UYA_PAL
	.Lobby = 0x005de440,
	.Bakisi = VA_COMPACT_BASE(0x004b0b28),
	.Hoven = VA_COMPACT_OFFSET(0x004b0b28, 0x004b2c40),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004b0b28, 0x004a8518),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004b0b28, 0x004a5cb0),
	.Metropolis = VA_COMPACT_OFFSET(0x004b0b28, 0x004a5000),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004b0b28, 0x004a2898),
	.CommandCenter = VA_COMPACT_OFFSET(0x004b0b28, 0x004a2890),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004b0b28, 0x004a5110),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004b0b28, 0x004a4410),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004b0b28, 0x004a3d90),
#else
	.Lobby = 0x005dbe98,
	.Bakisi = VA_COMPACT_BASE(0x004ae5d8),
	.Hoven = VA_COMPACT_OFFSET(0x004ae5d8, 0x004b0630),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ae5d8, 0x004a5f48),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ae5d8, 0x004a3760),
	.Metropolis = VA_COMPACT_OFFSET(0x004ae5d8, 0x004a2ab0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ae5d8, 0x004a02c8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ae5d8, 0x004a0480),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ae5d8, 0x004a2cc0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ae5d8, 0x004a2000),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ae5d8, 0x004a1940),
#endif
};

variableAddressCompact_t vaPadDeadzone_CompareInput = {
#if UYA_PAL
	.Lobby = 0x005c358c,
	.Bakisi = VA_COMPACT_BASE(0x00495d44),
	.Hoven = VA_COMPACT_OFFSET(0x00495d44, 0x00497e5c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00495d44, 0x0048d734),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00495d44, 0x0048ae04),
	.Metropolis = VA_COMPACT_OFFSET(0x00495d44, 0x0048a21c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00495d44, 0x00487ab4),
	.CommandCenter = VA_COMPACT_OFFSET(0x00495d44, 0x00487aac),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00495d44, 0x0048a32c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00495d44, 0x0048962c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00495d44, 0x00488fac),
#else
	.Lobby = 0x005c133c,
	.Bakisi = VA_COMPACT_BASE(0x00493b4c),
	.Hoven = VA_COMPACT_OFFSET(0x00493b4c, 0x00495ba4),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00493b4c, 0x0048b4bc),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00493b4c, 0x00488c0c),
	.Metropolis = VA_COMPACT_OFFSET(0x00493b4c, 0x00488024),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00493b4c, 0x0048583c),
	.CommandCenter = VA_COMPACT_OFFSET(0x00493b4c, 0x004859f4),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00493b4c, 0x00488234),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00493b4c, 0x00487574),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00493b4c, 0x00486eb4),
#endif
};

variableAddressCompact_t vaPadDeadzone_OffsetInput = {
#if UYA_PAL
	.Lobby = 0x005c3598,
	.Bakisi = VA_COMPACT_BASE(0x00495d50),
	.Hoven = VA_COMPACT_OFFSET(0x00495d50, 0x00497e68),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00495d50, 0x0048d740),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00495d50, 0x0048ae10),
	.Metropolis = VA_COMPACT_OFFSET(0x00495d50, 0x0048a228),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00495d50, 0x00487ac0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00495d50, 0x00487ab8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00495d50, 0x0048a338),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00495d50, 0x00489638),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00495d50, 0x00488fb8),
#else
	.Lobby = 0x005c1348,
	.Bakisi = VA_COMPACT_BASE(0x00493b58),
	.Hoven = VA_COMPACT_OFFSET(0x00493b58, 0x00495bb0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00493b58, 0x0048b4c8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00493b58, 0x00488c18),
	.Metropolis = VA_COMPACT_OFFSET(0x00493b58, 0x00488030),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00493b58, 0x00485848),
	.CommandCenter = VA_COMPACT_OFFSET(0x00493b58, 0x00485a00),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00493b58, 0x00488240),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00493b58, 0x00487580),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00493b58, 0x00486ec0),
#endif
};

variableAddressCompact_t vaPadDeadzone_ScaleInput = {
#if UYA_PAL
	.Lobby = 0x005c3560,
	.Bakisi = VA_COMPACT_BASE(0x00495d18),
	.Hoven = VA_COMPACT_OFFSET(0x00495d18, 0x00497e30),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00495d18, 0x0048d708),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00495d18, 0x0048add8),
	.Metropolis = VA_COMPACT_OFFSET(0x00495d18, 0x0048a1f0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00495d18, 0x00487a88),
	.CommandCenter = VA_COMPACT_OFFSET(0x00495d18, 0x00487a80),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00495d18, 0x0048a300),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00495d18, 0x00489600),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00495d18, 0x00488f80),
#else
	.Lobby = 0x005c1310,
	.Bakisi = VA_COMPACT_BASE(0x00493b20),
	.Hoven = VA_COMPACT_OFFSET(0x00493b20, 0x00495b78),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00493b20, 0x0048b490),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00493b20, 0x00488be0),
	.Metropolis = VA_COMPACT_OFFSET(0x00493b20, 0x00487ff8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00493b20, 0x00485810),
	.CommandCenter = VA_COMPACT_OFFSET(0x00493b20, 0x004859c8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00493b20, 0x00488208),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00493b20, 0x00487548),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00493b20, 0x00486e88),
#endif
};

VariableAddress_t vaDisableWrenchAimAssist_Addr = {
#if UYA_PAL
	.Lobby = 0x0062fe08,
	.Bakisi = 0x00501d00,
	.Hoven = 0x00503e18,
	.OutpostX12 = 0x004f96f0,
	.KorgonOutpost = 0x004f6e88,
	.Metropolis = 0x004f61d8,
	.BlackwaterCity = 0x004f3a70,
	.CommandCenter = 0x004f3a38,
	.BlackwaterDocks = 0x004f62b8,
	.AquatosSewers = 0x004f55b8,
	.MarcadiaPalace = 0x004f4f38,
#else
	.Lobby = 0x0062d5c0,
	.Bakisi = 0x004ff510,
	.Hoven = 0x00501568,
	.OutpostX12 = 0x004f6e80,
	.KorgonOutpost = 0x004f4698,
	.Metropolis = 0x004f39e8,
	.BlackwaterCity = 0x004f1200,
	.CommandCenter = 0x004f1388,
	.BlackwaterDocks = 0x004f3bc8,
	.AquatosSewers = 0x004f2f08,
	.MarcadiaPalace = 0x004f2848,
#endif
};

//=============  patchFluxIllumination
// 1: the jal that creates the muzzle point light.  2: the li that sets its 30 frame timer.
VariableAddress_t vaFluxIllumination1_Addr = {
#if UYA_PAL
	.Lobby = 0x0054f548,
	.Bakisi = 0x004072c8,
	.Hoven = 0x00406c30,
	.OutpostX12 = 0x003feb28,
	.KorgonOutpost = 0x003fdf08,
	.Metropolis = 0x003fcac8,
	.BlackwaterCity = 0x003f98b0,
	.CommandCenter = 0x00407cb8,
	.BlackwaterDocks = 0x00409c18,
	.AquatosSewers = 0x00409820,
	.MarcadiaPalace = 0x00408898,
#else
	.Lobby = 0x0054ec30,
	.Bakisi = 0x00406c60,
	.Hoven = 0x00406548,
	.OutpostX12 = 0x003fe440,
	.KorgonOutpost = 0x003fd880,
	.Metropolis = 0x003fc460,
	.BlackwaterCity = 0x003f91e8,
	.CommandCenter = 0x00407638,
	.BlackwaterDocks = 0x00409598,
	.AquatosSewers = 0x004091a0,
	.MarcadiaPalace = 0x00408218,
#endif
};

VariableAddress_t vaFluxIllumination2_Addr = {
#if UYA_PAL
	.Lobby = 0x0054f550,
	.Bakisi = 0x004072d0,
	.Hoven = 0x00406c38,
	.OutpostX12 = 0x003feb30,
	.KorgonOutpost = 0x003fdf10,
	.Metropolis = 0x003fcad0,
	.BlackwaterCity = 0x003f98b8,
	.CommandCenter = 0x00407cc0,
	.BlackwaterDocks = 0x00409c20,
	.AquatosSewers = 0x00409828,
	.MarcadiaPalace = 0x004088a0,
#else
	.Lobby = 0x0054ec38,
	.Bakisi = 0x00406c68,
	.Hoven = 0x00406550,
	.OutpostX12 = 0x003fe448,
	.KorgonOutpost = 0x003fd888,
	.Metropolis = 0x003fc468,
	.BlackwaterCity = 0x003f91f0,
	.CommandCenter = 0x00407640,
	.BlackwaterDocks = 0x004095a0,
	.AquatosSewers = 0x004091a8,
	.MarcadiaPalace = 0x00408220,
#endif
};
