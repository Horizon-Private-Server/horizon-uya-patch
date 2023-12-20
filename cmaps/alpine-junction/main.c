/***************************************************
 * FILENAME :		main.c
 * 
 * DESCRIPTION :
 * 		
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>
#include <libuya/math3d.h>

// set to 1 from patch to enable shrubs
#define SPECIAL_SHRUBS_ENABLED          (*(int*)0x000FFFFC)

// NTSC HOOK 0x0045a74c

//--------------------------------------------------------------------------
void update (int enabled)
{
  int i;
  static int count = 0;

#if UYA_PAL
  int *shrubCount = (int*)0;
#elif UYA_NTSC
  int *shrubCount = (int*)0x00249180;
#endif

  if (!count) count = *shrubCount;
  *shrubCount = enabled ? count : 0;
}

//--------------------------------------------------------------------------
int main (void)
{
  int enabled = SPECIAL_SHRUBS_ENABLED;
  update(enabled);

	return 0;
}
