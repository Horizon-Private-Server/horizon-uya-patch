#include <tamtypes.h>
#include "gamesettings.h"
#include "math.h"
#include "help.h"

/*
 * Points to the active game settings.
 */
#define GLOBAL_GAMESETTINGS			            ((GameSettings*)(*(u32*)0x002417C0))


/*
 * Points to the active game options.
 */
#define GLOBAL_GAMEOPTIONS			            ((GameOptions*)((u32)GLOBAL_GAMESETTINGS + 0x188))

/*
 *
 */
#define GAME_KILLS_TO_WIN                       (*(u8*)0x002417D4)
#define VEHICLES_ALLOWED                        (*(u8*)0x002417D7)
#define GAME_WEAPONS                            (*(u8*)0x002417DA)
#define CTF_CAP_LIMIT                           (*(u8*)0x002417DC)


//--------------------------------------------------------
GameSettings * gameGetSettings(void)
{
    return GLOBAL_GAMESETTINGS;
}

//--------------------------------------------------------
GameOptions * gameGetOptions(void)
{
    return GLOBAL_GAMEOPTIONS;
}

//--------------------------------------------------------
char * gameGetGameModeName(int modeId)
{
  switch (modeId)
  {
      case GAMERULE_SEIGE: return helpGetString(HELP_STRING_ID_SEIGE);
      case GAMERULE_CTF: return helpGetString(HELP_STRING_ID_CTF);
      case GAMERULE_DM: return helpGetString(HELP_STRING_ID_DEATHMATCH);
      default: return NULL;
  }
}
