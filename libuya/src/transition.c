#include "transition.h"


#ifdef UYA_PAL
#define TRANSITION_MULTIPLAYER ((TransitionData_Multiplayer_t *)0x00352ce0)
#else
#define TRANSITION_MULTIPLAYER ((TransitionData_Multiplayer_t *)0x00352de0)
#endif

TransitionData_Multiplayer_t *transGetData(void)
{
    return TRANSITION_MULTIPLAYER;
}
