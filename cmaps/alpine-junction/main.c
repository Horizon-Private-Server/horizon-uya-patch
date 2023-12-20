/***************************************************
 * FILENAME :		main.c
 * 
 * DESCRIPTION :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>
#include <libuya/math3d.h>
#include "config.h"

//--------------------------------------------------------------------------
void update (int enabled)
{
  int i;
  static int count = 0;

#if UYA_PAL
  int *shrubCount = (int*)0x00249000;
#elif UYA_NTSC
  int *shrubCount = (int*)0x00249180;
#endif

  if (!count) count = *shrubCount;
  *shrubCount = enabled ? count : 0;
}

//--------------------------------------------------------------------------
int main (void)
{
  int enabled = 0;
  if (PATCH_POINTERS && PATCH_POINTERS->ServerTimeMonth == 12) enabled = 1;
  
  update(enabled);

	return 0;
}
