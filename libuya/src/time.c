#include <tamtypes.h>
#include "interop.h"
#include "time.h"

VariableAddress_t vaFastDecTimeInt = {
#if UYA_PAL
    .Lobby = 0x00590138,
    .Bakisi = 0x0045d3a8,
    .Hoven = 0x0045ef28,
    .OutpostX12 = 0x00455d28,
    .KorgonOutpost = 0x004538e8,
    .Metropolis = 0x00452c28,
    .BlackwaterCity = 0x00450428,
    .CommandCenter = 0x00450d50,
    .BlackwaterDocks = 0x004535d0,
    .AquatosSewers = 0x004528d0,
    .MarcadiaPalace = 0x00452250,
#else
    .Lobby = 0x0058ef88,
    .Bakisi = 0x0045c2f8,
    .Hoven = 0x0045ddb8,
    .OutpostX12 = 0x00454bf8,
    .KorgonOutpost = 0x00452838,
    .Metropolis = 0x00451b78,
    .BlackwaterCity = 0x0044f2f8,
    .CommandCenter = 0x0044fde0,
    .BlackwaterDocks = 0x00452620,
    .AquatosSewers = 0x00451960,
    .MarcadiaPalace = 0x004512a0,
#endif
};

VariableAddress_t vaFastDecTimeShort = {
#if UYA_PAL
    .Lobby = 0x00590168,
    .Bakisi = 0x0045d3d8,
    .Hoven = 0x0045ef58,
    .OutpostX12 = 0x00455d58,
    .KorgonOutpost = 0x00453918,
    .Metropolis = 0x00452c58,
    .BlackwaterCity = 0x00450458,
    .CommandCenter = 0x00450d80,
    .BlackwaterDocks = 0x00453600,
    .AquatosSewers = 0x00452900,
    .MarcadiaPalace = 0x00452280,
#else
    .Lobby = 0x0058efb8,
    .Bakisi = 0x0045c328,
    .Hoven = 0x0045dde8,
    .OutpostX12 = 0x00454c28,
    .KorgonOutpost = 0x00452868,
    .Metropolis = 0x00451ba8,
    .BlackwaterCity = 0x0044f328,
    .CommandCenter = 0x0044fe10,
    .BlackwaterDocks = 0x00452650,
    .AquatosSewers = 0x00451990,
    .MarcadiaPalace = 0x004512d0,
#endif
};