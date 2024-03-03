#include "moby.h"
#include "interop.h"
#include "guber.h"

//--------------------------------------------------------
#define BEGIN_MOBY_PTR							((Moby**)GetAddress(&vaBEGIN_MOBY_PTR))
#define END_MOBY_PTR							((Moby**)((u32)GetAddress(&vaBEGIN_MOBY_PTR) + 0x8))
#define MOBY_VARS_LIST_TOP_PTR					((Moby**)((u32)GetAddress(&vaBEGIN_MOBY_PTR) + 0x1b0))

// For Orxon x12.
// Not needed anymore, but keeping just so I still have record of it.
#define MOBY_CLASS_LOADED_ADDR                  ((u16*)0x0026e7b0)

VariableAddress_t vaBEGIN_MOBY_PTR = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x0024845c,
	.Hoven = 0x0024865c,
	.OutpostX12 = 0x0024855c,
    .KorgonOutpost = 0x002483dc,
	.Metropolis = 0x0024845c,
	.BlackwaterCity = 0x002483dc,
	.CommandCenter = 0x00247fdc,
    .BlackwaterDocks = 0x002480dc,
    .AquatosSewers = 0x002480dc,
    .MarcadiaPalace = 0x002480dc,
#else
	.Lobby = 0,
	.Bakisi = 0x002485dc,
	.Hoven = 0x002487dc,
	.OutpostX12 = 0x002486dc,
    .KorgonOutpost = 0x0024855c,
	.Metropolis = 0x002485dc,
	.BlackwaterCity = 0x0024855c,
	.CommandCenter = 0x0024815c,
    .BlackwaterDocks = 0x0024825c,
    .AquatosSewers = 0x0024825c,
    .MarcadiaPalace = 0x0024825c,
#endif
};

VariableAddress_t vaGetMobyFunctions = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00493810,
	.Hoven = 0x00495928,
	.OutpostX12 = 0x0048b200,
    .KorgonOutpost = 0x004888d0,
	.Metropolis = 0x00487ce8,
	.BlackwaterCity = 0x00485580,
	.CommandCenter = 0x00485578,
    .BlackwaterDocks = 0x00487df8,
    .AquatosSewers = 0x004870f8,
    .MarcadiaPalace = 0x00486a7c,
#else
	.Lobby = 0,
	.Bakisi = 0x004918a8,
	.Hoven = 0x00493900,
	.OutpostX12 = 0x00489218,
    .KorgonOutpost = 0x00486968,
	.Metropolis = 0x00485d80,
	.BlackwaterCity = 0x00483598,
	.CommandCenter = 0x00483750,
    .BlackwaterDocks = 0x00485f90,
    .AquatosSewers = 0x004852d0,
    .MarcadiaPalace = 0x00484c10,
#endif
};

VariableAddress_t vaMobyCreateFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00476900,
	.Hoven = 0x004784c0,
	.OutpostX12 = 0x0046f2c0,
    .KorgonOutpost = 0x0046ce40,
	.Metropolis = 0x0046c180,
	.BlackwaterCity = 0x004699c0,
	.CommandCenter = 0x0046a280,
    .BlackwaterDocks = 0x0046cb00,
    .AquatosSewers = 0x0046be00,
    .MarcadiaPalace = 0x0046b780,
#else
	.Lobby = 0,
	.Bakisi = 0x00474b80,
	.Hoven = 0x00476680,
	.OutpostX12 = 0x0046d4c0,
    .KorgonOutpost = 0x0046b0c0,
	.Metropolis = 0x0046a400,
	.BlackwaterCity = 0x00467bc0,
	.CommandCenter = 0x00468640,
    .BlackwaterDocks = 0x0046ae80,
    .AquatosSewers = 0x0046a1c0,
    .MarcadiaPalace = 0x00469b00,
#endif
};

VariableAddress_t vaMobyDeleteFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00476cd0,
	.Hoven = 0x00478890,
	.OutpostX12 = 0x0046f690,
    .KorgonOutpost = 0x0046d210,
	.Metropolis = 0x0046c550,
	.BlackwaterCity = 0x00469d90,
	.CommandCenter = 0x0046a650,
    .BlackwaterDocks = 0x0046ced0,
    .AquatosSewers = 0x0046c1d0,
    .MarcadiaPalace = 0x0046bb50,
#else
	.Lobby = 0,
	.Bakisi = 0x00474f30,
	.Hoven = 0x00476a30,
	.OutpostX12 = 0x0046d870,
    .KorgonOutpost = 0x0046b470,
	.Metropolis = 0x0046a7b0,
	.BlackwaterCity = 0x00467f70,
	.CommandCenter = 0x004689f0,
    .BlackwaterDocks = 0x0046b230,
    .AquatosSewers = 0x0046a570,
    .MarcadiaPalace = 0x00469eb0,
#endif
};

VariableAddress_t vaMobyGetDamageFunc = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x00483608,
	.Hoven = 0x004851c8,
	.OutpostX12 = 0x0047bfc8,
    .KorgonOutpost = 0x00479b48,
	.Metropolis = 0x00478e88,
	.BlackwaterCity = 0x00476440,
	.CommandCenter = 0x00476f88,
    .BlackwaterDocks = 0x00479808,
    .AquatosSewers = 0x00478b08,
    .MarcadiaPalace = 0x00478488,
#else
	.Lobby = 0,
	.Bakisi = 0x00481810,
	.Hoven = 0x00483310,
	.OutpostX12 = 0x0047a150,
    .KorgonOutpost = 0x00477d50,
	.Metropolis = 0x00477090,
	.BlackwaterCity = 0x004745c8,
	.CommandCenter = 0x004752d0,
    .BlackwaterDocks = 0x00477b10,
    .AquatosSewers = 0x00476e50,
    .MarcadiaPalace = 0x00476790,
#endif
};

// Returns damagea amount of given moby.
VariableAddress_t vaGetDamager = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x004eed98,
	.Hoven = 0x004f0eb0,
	.OutpostX12 = 0x004e6788,
    .KorgonOutpost = 0x004e3f20,
	.Metropolis = 0x004e3270,
	.BlackwaterCity = 0x004e0b08,
	.CommandCenter = 0x004e0ad0,
    .BlackwaterDocks = 0x004e3350,
    .AquatosSewers = 0x004e2650,
    .MarcadiaPalace = 0x004e1fd0,
#else
	.Lobby = 0,
	.Bakisi = 0x004ec678,
	.Hoven = 0x004ee6d0,
	.OutpostX12 = 0x004e3fe8,
    .KorgonOutpost = 0x004e1800,
	.Metropolis = 0x004e0b50,
	.BlackwaterCity = 0x004de368,
	.CommandCenter = 0x004de4f0,
    .BlackwaterDocks = 0x004e0d30,
    .AquatosSewers = 0x004e0070,
    .MarcadiaPalace = 0x004df9b0,
#endif
};

/*
 * Returns pointer to the start of the moby list.
 */
Moby * mobyListGetStart(void)
{
    return *BEGIN_MOBY_PTR;
}

/*
 * Returns pointer to the end of the moby list.
 */
Moby * mobyListGetEnd(void)
{
    return *END_MOBY_PTR;
}

/*
 * Returns non-zero if the given moby is destroyed.
 */
int mobyIsDestroyed(Moby* moby)
{
    return moby && moby->State <= -2;
}

/*
 */
// int mobyGetNumSpawnableMobys(void)
// {
//     return *(int*)0x00222790;
// }

/*
 * Returns a pointer to the next living moby of the given oclass.
 * Returns NULL if none found.
 */
Moby * mobyFindNextByOClass(Moby* start, int oClass)
{
    Moby* mEnd = mobyListGetEnd();
    
	// find
	if (oClass > 0 && start) {
		while (start < mEnd && (start->OClass != oClass || mobyIsDestroyed(start)))
			++start;
	}

    // found
    if (start < mEnd)
        return start;
    
    // couldn't find
    return NULL;
}

/*
 * Returns non-zero if the given o class is loaded in the map.
 */
int mobyClassIsLoaded(int oClass)
{
    u16 * list = mobyGetLoadedMobyClassList();
    u16 class = 0;
    while ((class = *list++) != 0xFFFF)
    {
        if (class == oClass)
            return 1;
    }

    return 0;
}

/*
 */
Moby* mobyGetByGuberUid(u32 uid)
{
  Moby* mEnd = mobyListGetEnd();
  Moby* m = mobyListGetStart();

  while (m < mEnd)
  {
    Guber* guber = guberGetObjectByMoby(m);
    if (guber && guber->Id.UID == uid)
      return m;

    ++m;
  }

  return NULL;
}

VariableAddress_t vaShieldTriggerVars = {
#if UYA_PAL
	.Lobby = 0,
	.Bakisi = 0x002478b0,
	.Hoven = 0x00247a78,
	.OutpostX12 = 0x00247a78,
	.KorgonOutpost = 0x002477f0,
	.Metropolis = 0x002478b8,
	.BlackwaterCity = 0x002476f0,
	.CommandCenter = 0x00247588,
	.BlackwaterDocks = 0x00247648,
	.AquatosSewers = 0x00247658,
	.MarcadiaPalace = 0x00247688,
#else
	.Lobby = 0,
	.Bakisi = 0x00247a30,
	.Hoven = 0x00247bf8,
	.OutpostX12 = 0x00247af8,
	.KorgonOutpost = 0x00247970,
	.Metropolis = 0x00247a38,
	.BlackwaterCity = 0x00247870,
	.CommandCenter = 0x00247708,
	.BlackwaterDocks = 0x002477c8,
	.AquatosSewers = 0x002477d8,
	.MarcadiaPalace = 0x00247808,
#endif
};
ShieldVars* mobyGetShieldVars(void)
{
	return GetAddress(&vaShieldTriggerVars);
}

VariableAddress_t vaFlagUpdate_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00430608,
    .Hoven = 0x00432060,
    .OutpostX12 = 0x00428f90,
    .KorgonOutpost = 0x00426b38,
    .Metropolis = 0x00425e90,
    .BlackwaterCity = 0x00421cf8,
    .CommandCenter = 0x00426488,
    .BlackwaterDocks = 0x00428cd8,
    .AquatosSewers = 0x00427ff0,
    .MarcadiaPalace = 0x00427958,
#else
    .Lobby = 0,
    .Bakisi = 0x0042fb80,
    .Hoven = 0x00431510,
    .OutpostX12 = 0x00428458,
    .KorgonOutpost = 0x004260a8,
    .Metropolis = 0x00425408,
    .BlackwaterCity = 0x00421228,
    .CommandCenter = 0x00425ad8,
    .BlackwaterDocks = 0x00428310,
    .AquatosSewers = 0x00427640,
    .MarcadiaPalace = 0x00426f90,
#endif
};
VariableAddress_t vaFlagIsAtBase_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004305b0,
    .Hoven = 0x00432008,
    .OutpostX12 = 0x00428f38,
    .KorgonOutpost = 0x00426ae0,
    .Metropolis = 0x00425e38,
    .BlackwaterCity = 0x00421ca0,
    .CommandCenter = 0x00426430,
    .BlackwaterDocks = 0x00428c80,
    .AquatosSewers = 0x00427f98,
    .MarcadiaPalace = 0x00427900,
#else
    .Lobby = 0,
    .Bakisi = 0x0042fb28,
    .Hoven = 0x004314b8,
    .OutpostX12 = 0x00428400,
    .KorgonOutpost = 0x00426050,
    .Metropolis = 0x004253b0,
    .BlackwaterCity = 0x004211d0,
    .CommandCenter = 0x00425a80,
    .BlackwaterDocks = 0x004282b8,
    .AquatosSewers = 0x004275e8,
    .MarcadiaPalace = 0x00426f38,
#endif
};
VariableAddress_t vaFlagReturnToBase_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00430100,
    .Hoven = 0x00431b58,
    .OutpostX12 = 0x00428a88,
    .KorgonOutpost = 0x00426630,
    .Metropolis = 0x00425988,
    .BlackwaterCity = 0x004217f0,
    .CommandCenter = 0x00425f80,
    .BlackwaterDocks = 0x004287d0,
    .AquatosSewers = 0x00427ae8,
    .MarcadiaPalace = 0x00427450,
#else
    .Lobby = 0,
    .Bakisi = 0x0042f678,
    .Hoven = 0x00431008,
    .OutpostX12 = 0x00427f50,
    .KorgonOutpost = 0x00425ba0,
    .Metropolis = 0x00424f00,
    .BlackwaterCity = 0x00420d20,
    .CommandCenter = 0x004255d0,
    .BlackwaterDocks = 0x00427e08,
    .AquatosSewers = 0x00427138,
    .MarcadiaPalace = 0x00426a88,
#endif
};
VariableAddress_t vaFlagIsReturning_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x004300d8,
    .Hoven = 0x00431b30,
    .OutpostX12 = 0x00428a60,
    .KorgonOutpost = 0x00426608,
    .Metropolis = 0x00425960,
    .BlackwaterCity = 0x004217c8,
    .CommandCenter = 0x00425f58,
    .BlackwaterDocks = 0x004287a8,
    .AquatosSewers = 0x00427ac0,
    .MarcadiaPalace = 0x00427428,
#else
    .Lobby = 0,
    .Bakisi = 0x0042f650,
    .Hoven = 0x00430fe0,
    .OutpostX12 = 0x00427f28,
    .KorgonOutpost = 0x00425b78,
    .Metropolis = 0x00424ed8,
    .BlackwaterCity = 0x00420cf8,
    .CommandCenter = 0x004255a8,
    .BlackwaterDocks = 0x00427de0,
    .AquatosSewers = 0x00427110,
    .MarcadiaPalace = 0x00426a60,
#endif
};
VariableAddress_t vaFlagPickup_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00430090,
    .Hoven = 0x00431AE8,
    .OutpostX12 = 0x00428A18,
    .KorgonOutpost = 0x004265C0,
    .Metropolis = 0x00425918,
    .BlackwaterCity = 0x00421780,
    .CommandCenter = 0x00425F10,
    .BlackwaterDocks = 0x00428760,
    .AquatosSewers = 0x00427A78,
    .MarcadiaPalace = 0x004273E0,
#else
    .Lobby = 0,
    .Bakisi = 0x0042f608,
    .Hoven = 0x00430F98,
    .OutpostX12 = 0x00427EE0,
    .KorgonOutpost = 0x00425B30,
    .Metropolis = 0x00424E90,
    .BlackwaterCity = 0x00420CB0,
    .CommandCenter = 0x00425560,
    .BlackwaterDocks = 0x00427D98,
    .AquatosSewers = 0x004270C8,
    .MarcadiaPalace = 0x00426A18,
#endif
};
VariableAddress_t vaFlagIsBeingPickedUp_Func = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00430068,
    .Hoven = 0x00431ac0,
    .OutpostX12 = 0x004289f0,
    .KorgonOutpost = 0x00426598,
    .Metropolis = 0x004258f0,
    .BlackwaterCity = 0x00421758,
    .CommandCenter = 0x00425ee8,
    .BlackwaterDocks = 0x00428738,
    .AquatosSewers = 0x00427a50,
    .MarcadiaPalace = 0x004273b8,
#else
    .Lobby = 0,
    .Bakisi = 0x0042f5e0,
    .Hoven = 0x00430f70,
    .OutpostX12 = 0x00427eb8,
    .KorgonOutpost = 0x00425b08,
    .Metropolis = 0x00424e68,
    .BlackwaterCity = 0x00420c88,
    .CommandCenter = 0x00425538,
    .BlackwaterDocks = 0x00427d70,
    .AquatosSewers = 0x004270a0,
    .MarcadiaPalace = 0x004269f0,
#endif
};

int flagIsOnSafeGround(Moby* flagMoby)
{
	int hotspots = CollHotspot();
	switch (hotspots) {
		case 0:
		case 1:
		case 3:
		case 5:
		case 6:
		case 8:
		case 11:
		case 12:
		case 13:
			return 0;
	}
	return 1;
}
