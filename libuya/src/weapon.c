#include "weapon.h"
#include "interop.h"

#define GADGET_TABLE                    ((GadgetDef *)GetAddress(&vaWeaponsPvar))
#define END_GADGETS                     ((GadgetDef *)((u32)GetAddress(&vaWeaponsPvar) + 0xc00))
#define TWEAKERS_GRAVITY_BOMB           (GetAddress(&vaWeapon_GravityBomb_Tweakers))

variableAddressCompact_t vaWeaponsPvar = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x003c2ed0,
	.Hoven = VA_COMPACT_OFFSET(0x003c2ed0, 0x003c2510),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003c2ed0, 0x003ba410),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003c2ed0, 0x003ba1d0),
	.Metropolis = VA_COMPACT_OFFSET(0x003c2ed0, 0x003b9f50),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003c2ed0, 0x003ba190),
	.CommandCenter = VA_COMPACT_OFFSET(0x003c2ed0, 0x003ca710),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003c2ed0, 0x003cab10),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003c2ed0, 0x003cb290),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003c2ed0, 0x003ca810),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x003c3010,
	.Hoven = VA_COMPACT_OFFSET(0x003c3010, 0x003c2650),
	.OutpostX12 = VA_COMPACT_OFFSET(0x003c3010, 0x003ba550),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x003c3010, 0x003ba310),
	.Metropolis = VA_COMPACT_OFFSET(0x003c3010, 0x003ba090),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x003c3010, 0x003ba2d0),
	.CommandCenter = VA_COMPACT_OFFSET(0x003c3010, 0x003ca850),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x003c3010, 0x003cac50),
	.AquatosSewers = VA_COMPACT_OFFSET(0x003c3010, 0x003cb3d0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x003c3010, 0x003ca950),
#endif
};

// GadgetBox: Is Weapon Enabled?
variableAddressCompact_t vaGB_IsWeaponEnabled = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x00545bb0,
	.Hoven = VA_COMPACT_OFFSET(0x00545bb0, 0x00547d78),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00545bb0, 0x0053d650),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00545bb0, 0x0053ad38),
	.Metropolis = VA_COMPACT_OFFSET(0x00545bb0, 0x0053a138),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00545bb0, 0x00537920),
	.CommandCenter = VA_COMPACT_OFFSET(0x00545bb0, 0x00537178),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00545bb0, 0x005399f8),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00545bb0, 0x00538cf8),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00545bb0, 0x00538678),
#else
	.Lobby = VA_COMPACT_LOBBY(0),
	.Bakisi = 0x005432a0,
	.Hoven = VA_COMPACT_OFFSET(0x005432a0, 0x005453a8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x005432a0, 0x0053acc0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x005432a0, 0x00538428),
	.Metropolis = VA_COMPACT_OFFSET(0x005432a0, 0x00537828),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x005432a0, 0x00534f90),
	.CommandCenter = VA_COMPACT_OFFSET(0x005432a0, 0x005349c0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x005432a0, 0x00537200),
	.AquatosSewers = VA_COMPACT_OFFSET(0x005432a0, 0x00536540),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x005432a0, 0x00535e80),
#endif
};

variableAddressCompact_t vaWeapon_GravityBomb_Tweakers = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x0024b010),
	.Bakisi = 0x00247100,
	.Hoven = VA_COMPACT_OFFSET(0x00247100, 0x00247140),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00247100, 0x00247140),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00247100, 0x002470e0),
	.Metropolis = VA_COMPACT_OFFSET(0x00247100, 0x00247110),
	.CommandCenter = VA_COMPACT_OFFSET(0x00247100, 0x00247020),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00247100, 0x00247050),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00247100, 0x002470f0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00247100, 0x00247090),
#else
	.Lobby = VA_COMPACT_LOBBY(0x0024b120),
	.Bakisi = 0x00247280,
	.Hoven = VA_COMPACT_OFFSET(0x00247280, 0x002472c0),
	.OutpostX12 = VA_COMPACT_OFFSET(0x00247280, 0x002472c0),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x00247280, 0x00247260),
	.Metropolis = VA_COMPACT_OFFSET(0x00247280, 0x00247290),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x00247280, 0x00247170),
	.CommandCenter = VA_COMPACT_OFFSET(0x00247280, 0x002471a0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x00247280, 0x002471d0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x00247280, 0x00247270),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x00247280, 0x00247210),
#endif
};



/*
 * Returns pointer to the start of the moby list.
 */
GadgetDef * weaponGadgetList(void)
{
    return GADGET_TABLE;
}

TweakersGravityBomb_t * weaponGravityBombTweakers(void)
{
    return TWEAKERS_GRAVITY_BOMB;
}
