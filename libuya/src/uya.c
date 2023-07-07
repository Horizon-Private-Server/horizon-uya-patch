#include "uya.h"
#include "player.h"

// 
extern void padUpdate(void);
extern void playerPadUpdate(void);

/*
 * NAME :		uyaPreUpdate
 * 
 * DESCRIPTION :
 * 			Must be called at the beginning of each tick.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void uyaPreUpdate(void)
{
    
}

/*
 * NAME :		uyaPostUpdate
 * 
 * DESCRIPTION :
 * 			Must be called at the end of each tick.
 * 
 * NOTES :
 * 
 * ARGS : 
 * 
 * RETURN :
 * 
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void uyaPostUpdate(void)
{
  padUpdate();
  playerPadUpdate();
}
