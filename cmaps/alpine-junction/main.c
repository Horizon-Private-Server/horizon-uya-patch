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
#define SPECIAL_SHRUBS_ENABLED          (*(int*)0x0)

//--------------------------------------------------------------------------
struct ShrubInstance
{
  int Class;
  float Distance;
  char PAD[8];
  MATRIX World;
  int Red;
  int Blue;
  int Green;
  char PAD2[0x14];
};

//--------------------------------------------------------------------------
const int ShrubClasses[] = {
  0x1225,
  0x1226,
  0x1227,
  0x1228,
  0x1229
};
const int ShrubClassesCount = sizeof(ShrubClasses)/sizeof(int);

//--------------------------------------------------------------------------
int is_class (int class)
{
  int i;

  for (i = 0; i < ShrubClassesCount; ++i) {
    if (ShrubClasses[i] == class) return 1;
  }

  return 0;
}

//--------------------------------------------------------------------------
void update (int enabled)
{
  int i;

#if UYA_PAL
  int shrubCount = *(int*)0x0076FFE0;
  struct ShrubInstance* shrubInstances = (struct ShrubInstance*)0x0076FFF0;
#elif UYA_NTSC
  int shrubCount = *(int*)0;
  struct ShrubInstance* shrubInstances = (struct ShrubInstance*)0;
#endif

  for (i = 0; i < shrubCount; ++i) {
    if (is_class(shrubInstances[i].Class)) {
      shrubInstances[i].Distance = enabled ? 128.0 : 0;
    }
  }
}

//--------------------------------------------------------------------------
int main (void)
{
	static int last = 0;

  int enabled = SPECIAL_SHRUBS_ENABLED;
  if (enabled != last) {
    update(enabled);
    last = enabled;
  }

	return 0;
}
