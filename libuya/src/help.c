#include "help.h"
#include "game.h"

// 
char * internal_helpGetString_inGame(int);
char * internal_helpGetString_inLobby(int);

//--------------------------------------------------------
char * helpGetString(int stringId)
{
  if (isInGame())
  {
      return internal_helpGetString_inGame(stringId);
  }
  else if (isInMenus())
  {
      return internal_helpGetString_inLobby(stringId);
  }

  return NULL;
}
