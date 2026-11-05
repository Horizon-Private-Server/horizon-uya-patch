#include <libuya/interop.h>

//================================================
//=============  disableWeaponPacks
//================================================
variableAddressCompact_t vaWeaponPackSpawnFunc = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004fa350,
	.Hoven = VA_COMPACT_OFFSET(0x004fa350, 0x004fc468),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004fa350, 0x004f1d40),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004fa350, 0x004ef4d8),
	.Metropolis = VA_COMPACT_OFFSET(0x004fa350, 0x004ee828),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004fa350, 0x004ec0c0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004fa350, 0x004ec088),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004fa350, 0x004ee908),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004fa350, 0x004edc08),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004fa350, 0x004ed588),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004f7bd0,
	.Hoven = VA_COMPACT_OFFSET(0x004f7bd0, 0x004f9c28),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f7bd0, 0x004ef540),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f7bd0, 0x004ecd58),
	.Metropolis = VA_COMPACT_OFFSET(0x004f7bd0, 0x004ec0a8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f7bd0, 0x004e98c0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f7bd0, 0x004e9a48),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f7bd0, 0x004ec288),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f7bd0, 0x004eb5c8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f7bd0, 0x004eaf08),
#endif
};

//================================================
//=============  v2_Setting
//================================================
VariableAddress_t vaCheckWeaponKill = {
#ifdef UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00544488,
	.Hoven = 0x00546650,
	.OutpostX12 = 0x0053bf28,
	.KorgonOutpost = 0x00539610,
	.Metropolis = 0x00538a910,
	.BlackwaterCity = 0x005361f8,
	.CommandCenter = 0x00535a50,
	.BlackwaterDocks = 0x005382d0,
	.AquatosSewers = 0x005375d0,
	.MarcadiaPalace = 0x00536f50,
#else
	.Lobby = 0,
	.Bakisi = 0x00541b78,
	.Hoven = 0x00543c80,
	.OutpostX12 = 0x00539598,
	.KorgonOutpost = 0x00536d00,
	.Metropolis = 0x00536100,
	.BlackwaterCity = 0x00533868,
	.CommandCenter = 0x00533298,
	.BlackwaterDocks = 0x00535ad8,
	.AquatosSewers = 0x00534e18,
	.MarcadiaPalace = 0x00534758,
#endif
};
variableAddressCompact_t vaUpdateWeaponKill = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004fb448,
	.Hoven = VA_COMPACT_OFFSET(0x004fb448, 0x004fd560),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004fb448, 0x004f2e38),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004fb448, 0x004f05d0),
	.Metropolis = VA_COMPACT_OFFSET(0x004fb448, 0x004ef920),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004fb448, 0x004ed1b8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004fb448, 0x004ed180),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004fb448, 0x004efa00),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004fb448, 0x004eed00),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004fb448, 0x004ee680),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004f8cc8,
	.Hoven = VA_COMPACT_OFFSET(0x004f8cc8, 0x004fad20),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f8cc8, 0x004f0638),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f8cc8, 0x004ede50),
	.Metropolis = VA_COMPACT_OFFSET(0x004f8cc8, 0x004ed1a0),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f8cc8, 0x004ea9b8),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f8cc8, 0x004eab40),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f8cc8, 0x004ed380),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f8cc8, 0x004ec6c0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f8cc8, 0x004ec000),
#endif
};

//================================================
//=============  AutoRespawn
//================================================
variableAddressCompact_t vaDM_PressXToRespawn = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004b22b0,
	.Hoven = VA_COMPACT_OFFSET(0x004b22b0, 0x004b43c8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004b22b0, 0x004a9ca0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004b22b0, 0x004a7438),
	.Metropolis = VA_COMPACT_OFFSET(0x004b22b0, 0x004a6788),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004b22b0, 0x004a4020),
	.CommandCenter = VA_COMPACT_OFFSET(0x004b22b0, 0x004a4018),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004b22b0, 0x004a6898),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004b22b0, 0x004a5b98),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004b22b0, 0x004a5518),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004afd60,
	.Hoven = VA_COMPACT_OFFSET(0x004afd60, 0x004b1db8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004afd60, 0x004a76d0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004afd60, 0x004a4ee8),
	.Metropolis = VA_COMPACT_OFFSET(0x004afd60, 0x004a4238),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004afd60, 0x004a1a50),
	.CommandCenter = VA_COMPACT_OFFSET(0x004afd60, 0x004a1c08),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004afd60, 0x004a4448),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004afd60, 0x004a3788),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004afd60, 0x004a30c8),
#endif
};

//================================================
//=============  disableCameraShake
//================================================
variableAddressCompact_t vaCameraShakeFunc = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00446eb0,
	.Hoven = VA_COMPACT_OFFSET(0x00446eb0, 0x00448a30),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00446eb0, 0x0043f830),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00446eb0, 0x0043d3f0),
	.Metropolis = VA_COMPACT_OFFSET(0x00446eb0, 0x0043c730),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00446eb0, 0x00439f30),
	.CommandCenter = VA_COMPACT_OFFSET(0x00446eb0, 0x0043abb0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00446eb0, 0x0043d430),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00446eb0, 0x0043c730),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00446eb0, 0x0043c0b0),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00446010,
	.Hoven = VA_COMPACT_OFFSET(0x00446010, 0x00447ad0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00446010, 0x0043e910),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00446010, 0x0043c550),
	.Metropolis = VA_COMPACT_OFFSET(0x00446010, 0x0043b890),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00446010, 0x00439010),
	.CommandCenter = VA_COMPACT_OFFSET(0x00446010, 0x0043c690),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00446010, 0x0043c690),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00446010, 0x0043b9d0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00446010, 0x0043b310),
#endif
};

//================================================
//=============  disableRespawning
//================================================
variableAddressCompact_t vaDM_RespawnUpdater = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004b21f4,
	.Hoven = VA_COMPACT_OFFSET(0x004b21f4, 0x004b430c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004b21f4, 0x004a9be4),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004b21f4, 0x004a737c),
	.Metropolis = VA_COMPACT_OFFSET(0x004b21f4, 0x004a66cc),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004b21f4, 0x004a3f64),
	.CommandCenter = VA_COMPACT_OFFSET(0x004b21f4, 0x004a3f5c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004b21f4, 0x004a67dc),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004b21f4, 0x004a5adc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004b21f4, 0x004a545c),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004afca4,
	.Hoven = VA_COMPACT_OFFSET(0x004afca4, 0x004b1cfc),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004afca4, 0x004a7614),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004afca4, 0x004a4e2c),
	.Metropolis = VA_COMPACT_OFFSET(0x004afca4, 0x004a417c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004afca4, 0x004a1994),
	.CommandCenter = VA_COMPACT_OFFSET(0x004afca4, 0x004a1b4c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004afca4, 0x004a438c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004afca4, 0x004a36cc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004afca4, 0x004a300c),
#endif
};

//================================================
//=============  setRespawnTimer_Player
//================================================
VariableAddress_t vaRespawnTimerFunc_Player = {
#if UYA_PAL
	.Lobby = 0x006282fc,
	.Bakisi = 0x004fa1f4,
	.Hoven = 0x004fc30c,
	.OutpostX12 = 0x004f1be4,
	.KorgonOutpost = 0x004ef37c,
	.Metropolis = 0x004ee6cc,
	.BlackwaterCity = 0x004ebf64,
	.CommandCenter = 0x004ebf2c,
	.BlackwaterDocks = 0x004ee7ac,
	.AquatosSewers = 0x01795ffc,
	.MarcadiaPalace = 0x004ed42c,
#else
	.Lobby = 0x00625b24,
	.Bakisi = 0x004f7a74,
	.Hoven = 0x004f9acc,
	.OutpostX12 = 0x004ef3e4,
	.KorgonOutpost = 0x004ecbfc,
	.Metropolis = 0x004ebf4c,
	.BlackwaterCity = 0x004e9764,
	.CommandCenter = 0x004e98ec,
	.BlackwaterDocks = 0x004ec12c,
	.AquatosSewers = 0x004eb46c,
	.MarcadiaPalace = 0x004eadac,
#endif
};

//================================================
//=============  noPostHitInvinc
//================================================
variableAddressCompact_t vaPostHitInvinc = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x005274b4,
	.Hoven = VA_COMPACT_OFFSET(0x005274b4, 0x005295cc),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005274b4, 0x0051eea4),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005274b4, 0x0051c63c),
	.Metropolis = VA_COMPACT_OFFSET(0x005274b4, 0x0051b98c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005274b4, 0x00519224),
	.CommandCenter = VA_COMPACT_OFFSET(0x005274b4, 0x00518fe4),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005274b4, 0x0051b864),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005274b4, 0x0051ab64),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005274b4, 0x0051a4e4),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00524c34,
	.Hoven = VA_COMPACT_OFFSET(0x00524c34, 0x00526c8c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00524c34, 0x0051c5a4),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00524c34, 0x00519dbc),
	.Metropolis = VA_COMPACT_OFFSET(0x00524c34, 0x0051910c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00524c34, 0x00516924),
	.CommandCenter = VA_COMPACT_OFFSET(0x00524c34, 0x005168a4),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00524c34, 0x005190e4),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00524c34, 0x00518424),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00524c34, 0x00517d64),
#endif
};

//================================================
//=============  healthbars
//================================================
variableAddressCompact_t vaHealthbars_Hook = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00507c70,
	.Hoven = VA_COMPACT_OFFSET(0x00507c70, 0x00509d88),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00507c70, 0x004ff660),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00507c70, 0x004fcdf8),
	.Metropolis = VA_COMPACT_OFFSET(0x00507c70, 0x004fc148),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00507c70, 0x004f99e0),
	.CommandCenter = VA_COMPACT_OFFSET(0x00507c70, 0x004f99a8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00507c70, 0x004fc228),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00507c70, 0x004fb528),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00507c70, 0x004faea8),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00505480,
	.Hoven = VA_COMPACT_OFFSET(0x00505480, 0x005074d8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00505480, 0x004fcdf0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00505480, 0x004fa608),
	.Metropolis = VA_COMPACT_OFFSET(0x00505480, 0x004f9958),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00505480, 0x004f7170),
	.CommandCenter = VA_COMPACT_OFFSET(0x00505480, 0x004f72f8),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00505480, 0x004f9b38),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00505480, 0x004f8e78),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00505480, 0x004f87b8),
#endif
};

//================================================
//=============  radarBlips
//================================================
variableAddressCompact_t vaRadarBlips_FloatVal = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004aec24,
	.Hoven = VA_COMPACT_OFFSET(0x004aec24, 0x004b0d3c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004aec24, 0x004a6614),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004aec24, 0x004a3dac),
	.Metropolis = VA_COMPACT_OFFSET(0x004aec24, 0x004a30fc),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004aec24, 0x004a0994),
	.CommandCenter = VA_COMPACT_OFFSET(0x004aec24, 0x004a098c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004aec24, 0x004a320c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004aec24, 0x004a250c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004aec24, 0x004a1e8c),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004ac6d4,
	.Hoven = VA_COMPACT_OFFSET(0x004ac6d4, 0x004ae72c),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004ac6d4, 0x004a4044),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004ac6d4, 0x004a185c),
	.Metropolis = VA_COMPACT_OFFSET(0x004ac6d4, 0x004a0bac),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004ac6d4, 0x0049e3c4),
	.CommandCenter = VA_COMPACT_OFFSET(0x004ac6d4, 0x0049e57c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004ac6d4, 0x004a0dbc),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004ac6d4, 0x004a00fc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004ac6d4, 0x0049fa3c),
#endif
};

//================================================
//=============  playerInvicibleTimer
//================================================
variableAddressCompact_t vaPlayerInvincibleTimer_Hook = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x0052916c,
	.Hoven = VA_COMPACT_OFFSET(0x0052916c, 0x0052b284),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0052916c, 0x00520b5c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0052916c, 0x0051e2f4),
	.Metropolis = VA_COMPACT_OFFSET(0x0052916c, 0x0051d644),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0052916c, 0x0051aedc),
	.CommandCenter = VA_COMPACT_OFFSET(0x0052916c, 0x0051ac9c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0052916c, 0x0051d51c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0052916c, 0x0051c81c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0052916c, 0x0051c19c),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x005268ec,
	.Hoven = VA_COMPACT_OFFSET(0x005268ec, 0x00528944),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005268ec, 0x0051e25c),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005268ec, 0x0051ba74),
	.Metropolis = VA_COMPACT_OFFSET(0x005268ec, 0x0051adc4),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005268ec, 0x005185dc),
	.CommandCenter = VA_COMPACT_OFFSET(0x005268ec, 0x0051855c),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005268ec, 0x0051ad9c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005268ec, 0x0051a0dc),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005268ec, 0x00519a1c),
#endif
};
variableAddressCompact_t vaPlayerInvincibleTimer_Func = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004f8c30,
	.Hoven = VA_COMPACT_OFFSET(0x004f8c30, 0x004fad48),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f8c30, 0x004f0620),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f8c30, 0x004eddb8),
	.Metropolis = VA_COMPACT_OFFSET(0x004f8c30, 0x004ed108),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f8c30, 0x004ea9a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f8c30, 0x004ea968),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f8c30, 0x004ed1e8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f8c30, 0x004ec4e8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f8c30, 0x004ebe68),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x004f64b0,
	.Hoven = VA_COMPACT_OFFSET(0x004f64b0, 0x004f8508),
	.OutpostX12 = VA_COMPACT_OFFSET(0x004f64b0, 0x004ede20),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x004f64b0, 0x004eb638),
	.Metropolis = VA_COMPACT_OFFSET(0x004f64b0, 0x004ea988),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x004f64b0, 0x004e81a0),
	.CommandCenter = VA_COMPACT_OFFSET(0x004f64b0, 0x004e8328),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x004f64b0, 0x004eab68),
	.AquatosSewers = VA_COMPACT_OFFSET(0x004f64b0, 0x004e9ea8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x004f64b0, 0x004e97e8),
#endif
};

//================================================
//=============  patchSiegeTimeUp
//================================================
variableAddressCompact_t vaGB_UpdateGameController_MasterEndGame_Hook = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x0067063c),
	.Bakisi = 0x00543fe4,
	.Hoven = VA_COMPACT_OFFSET(0x00543fe4, 0x005461ac),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00543fe4, 0x0053ba84),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00543fe4, 0x0053916c),
	.Metropolis = VA_COMPACT_OFFSET(0x00543fe4, 0x0053856c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00543fe4, 0x00535d54),
	.CommandCenter = VA_COMPACT_OFFSET(0x00543fe4, 0x005355ac),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00543fe4, 0x00537e2c),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00543fe4, 0x0053712c),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00543fe4, 0x00536aac),
#else
	.Lobby = VA_COMPACT_LOBBY(0x0066dcec),
	.Bakisi = 0x005416d4,
	.Hoven = VA_COMPACT_OFFSET(0x005416d4, 0x005437dc),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005416d4, 0x005390f4),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005416d4, 0x0053685c),
	.Metropolis = VA_COMPACT_OFFSET(0x005416d4, 0x00535c5c),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005416d4, 0x005333c4),
	.CommandCenter = VA_COMPACT_OFFSET(0x005416d4, 0x00532df4),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005416d4, 0x00535634),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005416d4, 0x00534974),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005416d4, 0x005342b4),
#endif
};
