#include <libuya/interop.h>

//================================================
//=============  disableWeaponPacks
//================================================
VariableAddress_t vaWeaponPackSpawnFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004FA350,
    .Hoven = 0x004FC468,
    .OutpostX12 = 0x004F1D40,
    .KorgonOutpost = 0x004EF4D8,
    .Metropolis = 0x004EE828,
    .BlackwaterCity = 0x004EC0C0,
    .CommandCenter = 0x004EC088,
    .BlackwaterDocks = 0x004EE908,
    .AquatosSewers = 0x004EDC08,
    .MarcadiaPalace = 0x004ED588,
#else
    .Lobby = 0,
    .Bakisi = 0x004F7BD0,
    .Hoven = 0x004F9C28,
    .OutpostX12 = 0x004EF540,
    .KorgonOutpost = 0x004ECD58,
    .Metropolis = 0x004EC0A8,
    .BlackwaterCity = 0x004E98C0,
    .CommandCenter = 0x004E9A48,
    .BlackwaterDocks = 0x004EC288,
    .AquatosSewers = 0x004EB5C8,
    .MarcadiaPalace = 0x004EAF08,
#endif
};

//================================================
//=============  spawnWeaponPackOnDeath - SpawnPack
//================================================
VariableAddress_t vaRespawnTimerFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004e3690,
    .Hoven = 0x004e57a8,
    .OutpostX12 = 0x004db080,
    .KorgonOutpost = 0x004d8818,
    .Metropolis = 0x004d7b68,
    .BlackwaterCity = 0x004d5400,
    .CommandCenter = 0x004d53c8,
    .BlackwaterDocks = 0x004d7c48,
    .AquatosSewers = 0x004d6f48,
    .MarcadiaPalace = 0x004d68c8,
#else
    .Lobby = 0,
    .Bakisi = 0x004e0fd0,
    .Hoven = 0x004e3028,
    .OutpostX12 = 0x004D8940,
    .KorgonOutpost = 0x004d6158,
    .Metropolis = 0x004d54a8,
    .BlackwaterCity = 0x004d2cc0,
    .CommandCenter = 0x004d2e48,
    .BlackwaterDocks = 0x004d5688,
    .AquatosSewers = 0x004d49c8,
    .MarcadiaPalace = 0x004d4308,
#endif
};
VariableAddress_t vaSpawnWeaponPackFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004fb188,
    .Hoven = 0x004fd2a0,
    .OutpostX12 = 0x004f2b78,
    .KorgonOutpost = 0x004f0310,
    .Metropolis = 0x004ef660,
    .BlackwaterCity = 0x004ecef8,
    .CommandCenter = 0x004ecec0,
    .BlackwaterDocks = 0x004ef740,
    .AquatosSewers = 0x004eea40,
    .MarcadiaPalace = 0x004ee3c0,
#else
    .Lobby = 0,
    .Bakisi = 0x004f8a08,
    .Hoven = 0x004faa60,
    .OutpostX12 = 0x004f0378,
    .KorgonOutpost = 0x004edb90,
    .Metropolis = 0x004ecee0,
    .BlackwaterCity = 0x004ea6f8,
    .CommandCenter = 0x004ea880,
    .BlackwaterDocks = 0x004ed0c0,
    .AquatosSewers = 0x004ec400,
    .MarcadiaPalace = 0x004ebd40,
#endif
};

//================================================
//=============  spawnWeaponPackOnDeath
//================================================
VariableAddress_t vaRespawnTimerHook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004b00f4,
    .Hoven = 0x004b220c,
    .OutpostX12 = 0x004a7ae4,
    .KorgonOutpost = 0x004a527c,
    .Metropolis = 0x004a45cc,
    .BlackwaterCity = 0x004a1e64,
    .CommandCenter = 0x004a1e5c,
    .BlackwaterDocks = 0x004a46dc,
    .AquatosSewers = 0x004a39dc,
    .MarcadiaPalace = 0x004a335c,
#else
    .Lobby = 0,
    .Bakisi = 0x004adba4,
    .Hoven = 0x004afbfc,
    .OutpostX12 = 0x004a5514,
    .KorgonOutpost = 0x004a2d2c,
    .Metropolis = 0x004a207c,
    .BlackwaterCity = 0x0049f894,
    .CommandCenter = 0x0049fa4c,
    .BlackwaterDocks = 0x004a228c,
    .AquatosSewers = 0x004a15cc,
    .MarcadiaPalace = 0x004a0f0c,
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
VariableAddress_t vaUpdateWeaponKill = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004fb448,
	.Hoven = 0x004fd560,
	.OutpostX12 = 0x004f2e38,
	.KorgonOutpost = 0x004f05d0,
	.Metropolis = 0x004ef920,
	.BlackwaterCity = 0x004ed1b8,
	.CommandCenter = 0x004ed180,
	.BlackwaterDocks = 0x004efa00,
	.AquatosSewers = 0x004eed00,
	.MarcadiaPalace = 0x004ee680,
#else
	.Lobby = 0,
	.Bakisi = 0x004f8cc8,
	.Hoven = 0x004fad20,
	.OutpostX12 = 0x004f0638,
	.KorgonOutpost = 0x004ede50,
	.Metropolis = 0x004ed1a0,
	.BlackwaterCity = 0x004ea9b8,
	.CommandCenter = 0x004eab40,
	.BlackwaterDocks = 0x004ed380,
	.AquatosSewers = 0x004ec6c0,
	.MarcadiaPalace = 0x004ec000,
#endif
};

//================================================
//=============  AutoRespawn
//================================================
VariableAddress_t vaDM_PressXToRespawn = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004b22b0,
    .Hoven = 0x004b43c8,
    .OutpostX12 = 0x004a9ca0,
    .KorgonOutpost = 0x004a7438,
    .Metropolis = 0x004a6788,
    .BlackwaterCity = 0x004a4020,
    .CommandCenter = 0x004a4018,
    .BlackwaterDocks = 0x004a6898,
    .AquatosSewers = 0x004a5b98,
    .MarcadiaPalace = 0x004a5518,
#else
    .Lobby = 0,
    .Bakisi = 0x004afd60,
    .Hoven = 0x004b1db8,
    .OutpostX12 = 0x004a76d0,
    .KorgonOutpost = 0x004a4ee8,
    .Metropolis = 0x004a4238,
    .BlackwaterCity = 0x004a1a50,
    .CommandCenter = 0x004a1c08,
    .BlackwaterDocks = 0x004a4448,
    .AquatosSewers = 0x004a3788,
    .MarcadiaPalace = 0x004a30c8,
#endif
};

//================================================
//=============  deleteNodeTurretsUpdate
//================================================
VariableAddress_t vaNodeTurret_UpdateFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x0041ef60,
    .Hoven = 0x004209b8,
    .OutpostX12 = 0x004178e8,
    .KorgonOutpost = 0x00414f58,
    .Metropolis = 0x004147e8,
    .BlackwaterCity = 0x00410650,
    .CommandCenter = 0x0041acc0,
    .BlackwaterDocks = 0x0041d510,
    .AquatosSewers = 0x0041c828,
    .MarcadiaPalace = 0x0041c190,
#else
    .Lobby = 0,
    .Bakisi = 0x0041e6f8,
    .Hoven = 0x00420088,
    .OutpostX12 = 0x00416fd0,
    .KorgonOutpost = 0x004146e8,
    .Metropolis = 0x00413f80,
    .BlackwaterCity = 0x0040fda0,
    .CommandCenter = 0x0041a488,
    .BlackwaterDocks = 0x0041ccc0,
    .AquatosSewers = 0x0041bff0,
    .MarcadiaPalace = 0x0041b940,
#endif
};

//================================================
//=============  disableCameraShake
//================================================
VariableAddress_t vaCameraShakeFunc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00446eb0,
    .Hoven = 0x00448a30,
    .OutpostX12 = 0x0043f830,
    .KorgonOutpost = 0x0043d3f0,
    .Metropolis = 0x0043c730,
    .BlackwaterCity = 0x00439f30,
    .CommandCenter = 0x0043abb0,
    .BlackwaterDocks = 0x0043d430,
    .AquatosSewers = 0x0043c730,
    .MarcadiaPalace = 0x0043c0b0,
#else
    .Lobby = 0,
    .Bakisi = 0x00446010,
    .Hoven = 0x00447ad0,
    .OutpostX12 = 0x0043e910,
    .KorgonOutpost = 0x0043c550,
    .Metropolis = 0x0043b890,
    .BlackwaterCity = 0x00439010,
    .CommandCenter = 0x0043c690,
    .BlackwaterDocks = 0x0043c690,
    .AquatosSewers = 0x0043b9d0,
    .MarcadiaPalace = 0x0043b310,
#endif
};

//================================================
//=============  disableRespawning
//================================================
VariableAddress_t vaDM_RespawnUpdater = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004b21f4,
    .Hoven = 0x004b430c,
    .OutpostX12 = 0x004a9be4,
    .KorgonOutpost = 0x004a737c,
    .Metropolis = 0x004a66cc,
    .BlackwaterCity = 0x004a3f64,
    .CommandCenter = 0x004a3f5c,
    .BlackwaterDocks = 0x004a67dc,
    .AquatosSewers = 0x004a5adc,
    .MarcadiaPalace = 0x004a545c,
#else
    .Lobby = 0,
    .Bakisi = 0x004afca4,
    .Hoven = 0x004b1cfc,
    .OutpostX12 = 0x004a7614,
    .KorgonOutpost = 0x004a4e2c,
    .Metropolis = 0x004a417c,
    .BlackwaterCity = 0x004a1994,
    .CommandCenter = 0x004a1b4c,
    .BlackwaterDocks = 0x004a438c,
    .AquatosSewers = 0x004a36cc,
    .MarcadiaPalace = 0x004a300c,
#endif
};

//================================================
//=============  setRespawnTimer_Player
//================================================
VariableAddress_t vaRespawnTimerFunc_Player = {
// Uses the start of the Respawn Timer Function
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x003f6098,
    .Hoven = 0x003f4f20,
    .OutpostX12 = 0x003ece18,
    .KorgonOutpost = 0x003eccd8,
    .Metropolis = 0x003eadb8,
    .BlackwaterCity = 0x003e7ba0,
    .CommandCenter = 0x003f6a88,
    .BlackwaterDocks = 0x003f89e8,
    .AquatosSewers = 0x003f85f0,
    .MarcadiaPalace = 0x003f6b88,
#else
    .Lobby = 0,
    .Bakisi = 0x003f5ba8,
    .Hoven = 0x003f49b0,
    .OutpostX12 = 0x003ec8a8,
    .KorgonOutpost = 0x003ec7c8,
    .Metropolis = 0x003ea8c8,
    .BlackwaterCity = 0x003e7650,
    .CommandCenter = 0x003f6580,
    .BlackwaterDocks = 0x003f84e0,
    .AquatosSewers = 0x003f80e8,
    .MarcadiaPalace = 0x003f6680,
#endif
};

//================================================
//=============  noPostHitInvinc
//================================================
VariableAddress_t vaPostHitInvinc = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x005274b4,
    .Hoven = 0x005295cc,
    .OutpostX12 = 0x0051eea4,
    .KorgonOutpost = 0x0051c63c,
    .Metropolis = 0x0051b98c,
    .BlackwaterCity = 0x00519224,
    .CommandCenter = 0x00518fe4,
    .BlackwaterDocks = 0x0051b864,
    .AquatosSewers = 0x0051ab64,
    .MarcadiaPalace = 0x0051a4e4,
#else
    .Lobby = 0,
    .Bakisi = 0x00524c34,
    .Hoven = 0x00526c8c,
    .OutpostX12 = 0x0051c5a4,
    .KorgonOutpost = 0x00519dbc,
    .Metropolis = 0x0051910c,
    .BlackwaterCity = 0x00516924,
    .CommandCenter = 0x005168a4,
    .BlackwaterDocks = 0x005190e4,
    .AquatosSewers = 0x00518424,
    .MarcadiaPalace = 0x00517d64,
#endif
};

//================================================
//=============  healthbars
//================================================
VariableAddress_t vaHealthbars_Hook = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00507c70,
    .Hoven = 0x00509d88,
    .OutpostX12 = 0x004ff660,
    .KorgonOutpost = 0x004fcdf8,
    .Metropolis = 0x004fc148,
    .BlackwaterCity = 0x004f99e0,
    .CommandCenter = 0x004f99a8,
    .BlackwaterDocks = 0x004fc228,
    .AquatosSewers = 0x004fb528,
    .MarcadiaPalace = 0x004faea8,
#else
    .Lobby = 0,
    .Bakisi = 0x00505480,
    .Hoven = 0x005074d8,
    .OutpostX12 = 0x004fcdf0,
    .KorgonOutpost = 0x004fa608,
    .Metropolis = 0x004f9958,
    .BlackwaterCity = 0x004f7170,
    .CommandCenter = 0x004f72f8,
    .BlackwaterDocks = 0x004f9b38,
    .AquatosSewers = 0x004f8e78,
    .MarcadiaPalace = 0x004f87b8,
#endif
};

//================================================
//=============  radarBlips
//================================================
VariableAddress_t vaRadarBlips_FloatVal = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004aec24,
    .Hoven = 0x004b0d3c,
    .OutpostX12 = 0x004a6614,
    .KorgonOutpost = 0x004a3dac,
    .Metropolis = 0x004a30fc,
    .BlackwaterCity = 0x004a0994,
    .CommandCenter = 0x004a098c,
    .BlackwaterDocks = 0x004a320c,
    .AquatosSewers = 0x004a250c,
    .MarcadiaPalace = 0x004a1e8c,
#else
    .Lobby = 0,
    .Bakisi = 0x004ac6d4,
    .Hoven = 0x004ae72c,
    .OutpostX12 = 0x004a4044,
    .KorgonOutpost = 0x004a185c,
    .Metropolis = 0x004a0bac,
    .BlackwaterCity = 0x0049e3c4,
    .CommandCenter = 0x0049e57c,
    .BlackwaterDocks = 0x004a0dbc,
    .AquatosSewers = 0x004a00fc,
    .MarcadiaPalace = 0x0049fa3c,
#endif
};

//================================================
//=============  playerInvicibleTimer
//================================================
VariableAddress_t vaPlayerInvincibleTimer_Hook = {
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
VariableAddress_t vaPlayerInvincibleTimer_Func = {
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

