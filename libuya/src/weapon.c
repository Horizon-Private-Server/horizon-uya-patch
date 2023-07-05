#include "weapon.h"
#include "interop.h"

#define GADGET_TABLE                          ((GadgetDef *)GetAddress(&vaWeaponsPvar))
#define END_GADGETS                            ((GadgetDef *)((u32)GetAddress(&vaWeaponsPvar) + 0xc00))


VariableAddress_t vaWeaponsPvar = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x003c2ed0,
	.Hoven = 0x003c2510,
	.OutpostX12 = 0x003ba410,
    .KorgonOutpost = 0x003ba1d0,
	.Metropolis = 0x003b9f50,
	.BlackwaterCity = 0x003ba190,
	.CommandCenter = 0x003ca710,
    .BlackwaterDocks = 0x003cab10,
    .AquatosSewers = 0x003cb290,
    .MarcadiaPalace = 0x003ca810,
#else
	.Lobby = 0,
	.Bakisi = 0x003c3010,
	.Hoven = 0x003c2650,
	.OutpostX12 = 0x003ba550,
    .KorgonOutpost = 0x003ba310,
	.Metropolis = 0x003ba090,
	.BlackwaterCity = 0x003ba2d0,
	.CommandCenter = 0x003ca850,
    .BlackwaterDocks = 0x003cac50,
    .AquatosSewers = 0x003cb3d0,
    .MarcadiaPalace = 0x003ca950,
#endif
};

/*
 * Returns pointer to the start of the moby list.
 */
GadgetDef * weaponGadgetList(void)
{
    return GADGET_TABLE;
}
