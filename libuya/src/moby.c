#include "moby.h"
#include "interop.h"

//--------------------------------------------------------
#define BEGIN_MOBY_PTR                          ((Moby**)GetAddress(&vaBEGIN_MOBY_PTR))
#define END_MOBY_PTR                            ((Moby**)((u32)GetAddress(&vaBEGIN_MOBY_PTR) + 0x8))

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
 * 
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
