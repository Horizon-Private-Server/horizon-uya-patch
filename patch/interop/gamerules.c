#include <libuya/interop.h>

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
//=============  vampireLogic
//================================================
VariableAddress_t vaUpdateScoreboard = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00544488,
    .Hoven = 0x00546650,
    .OutpostX12 = 0x0053bf28,
    .KorgonOutpost = 0x00539610,
    .Metropolis = 0x00538a10,
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