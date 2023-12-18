#include <tamtypes.h>
#include "interop.h"
#include "random.h"

VariableAddress_t vaMB_rand = {
#if UYA_PAL
    .Lobby = 0x005b4398,
    .Bakisi = 0x00480c50,
    .Hoven = 0x00482810,
    .OutpostX12 = 0x00479610,
    .KorgonOutpost = 0x00477190,
    .Metropolis = 0x004764d0,
    .BlackwaterCity = 0x00473d10,
    .CommandCenter = 0x004745d0,
    .BlackwaterDocks = 0x00476e50,
    .AquatosSewers = 0x00476150,
    .MarcadiaPalace = 0x00475ad0,
#else
    .Lobby = 0x005b2498,
    .Bakisi = 0x0047ee90,
    .Hoven = 0x00480990,
    .OutpostX12 = 0x004777d0,
    .KorgonOutpost = 0x004753d0,
    .Metropolis = 0x00474710,
    .BlackwaterCity = 0x00471ed0,
    .CommandCenter = 0x00472950,
    .BlackwaterDocks = 0x00475190,
    .AquatosSewers = 0x004744d0,
    .MarcadiaPalace = 0x00473e10,
#endif
};

VariableAddress_t vaMB_randRangeInt = {
#if UYA_PAL
    .Lobby = 0x005b43c8,
    .Bakisi = 0x00480c80,
    .Hoven = 0x00482840,
    .OutpostX12 = 0x00479640,
    .KorgonOutpost = 0x004771c0,
    .Metropolis = 0x00476500,
    .BlackwaterCity = 0x00473d40,
    .CommandCenter = 0x00474600,
    .BlackwaterDocks = 0x00476e80,
    .AquatosSewers = 0x00476180,
    .MarcadiaPalace = 0x00475b00,
#else
    .Lobby = 0x005B24c8,
    .Bakisi = 0x0047eec0,
    .Hoven = 0x004809c0,
    .OutpostX12 = 0x00477800,
    .KorgonOutpost = 0x00475400,
    .Metropolis = 0x00474740,
    .BlackwaterCity = 0x00471f00,
    .CommandCenter = 0x00472980,
    .BlackwaterDocks = 0x004751c0,
    .AquatosSewers = 0x00474500,
    .MarcadiaPalace = 0x00473e40,
#endif
};

VariableAddress_t vaMB_randRange = {
#if UYA_PAL
    .Lobby = 0x005B4410,
    .Bakisi = 0x0480CC8,
    .Hoven = 0x00482888,
    .OutpostX12 = 0x00479688,
    .KorgonOutpost = 0x00477208,
    .Metropolis = 0x00476548,
    .BlackwaterCity = 0x00473d88,
    .CommandCenter = 0x00474648,
    .BlackwaterDocks = 0x00476EC8,
    .AquatosSewers = 0x004761C8,
    .MarcadiaPalace = 0x00475b48,
#else
    .Lobby = 0x005B2510,
    .Bakisi = 0x0047EF08,
    .Hoven = 0x00480A08,
    .OutpostX12 = 0x00477848,
    .KorgonOutpost = 0x00475448,
    .Metropolis = 0x00474788,
    .BlackwaterCity = 0x00471f48,
    .CommandCenter = 0x004729C8,
    .BlackwaterDocks = 0x00475208,
    .AquatosSewers = 0x00474548,
    .MarcadiaPalace = 0x00473e88,
#endif
};

VariableAddress_t vaMB_randRange2 = {
#if UYA_PAL
    .Lobby = 0x005B4468,
    .Bakisi = 0x00480D20,
    .Hoven = 0x004828E0,
    .OutpostX12 = 0x004796E0,
    .KorgonOutpost = 0x00477260,
    .Metropolis = 0x004765a0,
    .BlackwaterCity = 0x00473de0,
    .CommandCenter = 0x004746a0,
    .BlackwaterDocks = 0x00476F20,
    .AquatosSewers = 0x00476220,
    .MarcadiaPalace = 0x00475ba0,
#else
    .Lobby = 0x005B2568,
    .Bakisi = 0x0047ef60,
    .Hoven = 0x00480A60,
    .OutpostX12 = 0x0047780A0,
    .KorgonOutpost = 0x004754A0,
    .Metropolis = 0x004747E0,
    .BlackwaterCity = 0x00471fa0,
    .CommandCenter = 0x00472A20,
    .BlackwaterDocks = 0x00475260,
    .AquatosSewers = 0x004745a0,
    .MarcadiaPalace = 0x00473EE0,
#endif
};

VariableAddress_t vaMB_randRot = {
#if UYA_PAL
    .Lobby = 0x005B44c8,
    .Bakisi = 0x00480D80,
    .Hoven = 0x00482940,
    .OutpostX12 = 0x00479740,
    .KorgonOutpost = 0x004772c0,
    .Metropolis = 0x00476600,
    .BlackwaterCity = 0x00473e40,
    .CommandCenter = 0x00474700,
    .BlackwaterDocks = 0x00476F80,
    .AquatosSewers = 0x00476280,
    .MarcadiaPalace = 0x00475c00,
#else
    .Lobby = 0x005B25c8,
    .Bakisi = 0x0047efc0,
    .Hoven = 0x00480Ac0,
    .OutpostX12 = 0x00477900,
    .KorgonOutpost = 0x00475500,
    .Metropolis = 0x00474840,
    .BlackwaterCity = 0x00472000,
    .CommandCenter = 0x00472A80,
    .BlackwaterDocks = 0x004752c0,
    .AquatosSewers = 0x00474600,
    .MarcadiaPalace = 0x00473F40,
#endif
};
