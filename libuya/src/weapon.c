#include "weapon.h"
#include "interop.h"

#define GADGET_TABLE                    ((GadgetDef *)GetAddress(&vaWeaponsPvar))
#define END_GADGETS                     ((GadgetDef *)((u32)GetAddress(&vaWeaponsPvar) + 0xc00))
#define TWEAKERS_GRAVITY_BOMB           ((TweakersGravityBomb_t*)GetAddress(&vaWeapon_GravityBomb_Tweakers))

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

// GadgetBox: Is Weapon Enabled?
VariableAddress_t vaGB_IsWeaponEnabled = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00545bb0,
    .Hoven = 0x00547d78,
    .OutpostX12 = 0x0053d650,
    .KorgonOutpost = 0x0053ad38,
    .Metropolis = 0x0053a138,
    .BlackwaterCity = 0x00537920,
    .CommandCenter = 0x00537178,
    .BlackwaterDocks = 0x005399f8,
    .AquatosSewers = 0x00538cf8,
    .MarcadiaPalace = 0x00538678,
#else
    .Lobby = 0,
    .Bakisi = 0x005432a0,
    .Hoven = 0x005453a8,
    .OutpostX12 = 0x0053acc0,
    .KorgonOutpost = 0x00538428,
    .Metropolis = 0x00537828,
    .BlackwaterCity = 0x00534f90,
    .CommandCenter = 0x005349c0,
    .BlackwaterDocks = 0x00537200,
    .AquatosSewers = 0x00536540,
    .MarcadiaPalace = 0x00535e80,
#endif
};

VariableAddress_t vaWeapon_GravityBomb_Tweakers = {
#if UYA_PAL
	.Lobby = 0x0024afd0
	.BakisiIsles = 0x002470c0
	.HovenGorge = 0x00247100
	.OutpostX12 = 0x00247100
	.KorgonOutpost = 0x002470a0
	.Metropolis = 0x002470d0
	.BlackwaterCity = 0x00246fb0
	.CommandCenter = 0x00246fe0
	.BlackwaterDocks = 0x00247010
	.AquatosSewers = 0x002470b0
	.MarcadiaPalace = 0x00247050
#else
	.Lobby = 0x0024b0e0
	.BakisiIsles = 0x00247240
	.HovenGorge = 0x00247280
	.OutpostX12 = 0x00247280
	.KorgonOutpost = 0x00247220
	.Metropolis = 0x00247250
	.BlackwaterCity = 0x00247130
	.CommandCenter = 0x00247160
	.BlackwaterDocks = 0x00247190
	.AquatosSewers = 0x00247230
	.MarcadiaPalace = 0x002471d0
#endif
};

/*
 * Returns pointer to the start of the moby list.
 */
GadgetDef * weaponGadgetList(void)
{
    return GADGET_TABLE;
}

TweakersGravityBomb_t* weaponGravityBombTweakers(void)
{
    return TWEAKERS_GRAVITY_BOMB;
}