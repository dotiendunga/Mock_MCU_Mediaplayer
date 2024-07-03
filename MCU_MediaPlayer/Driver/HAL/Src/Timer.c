/***********************************************************************************
*
* Name 					    : Timer.c
* Author				    : Tran Ba Than
* Created date			    : Jun 25 2024
*
************************************************************************************/

#include "Timer.h"

void Init_LPIT()
{
    /*Enable M_CEN first*/
    LPIT0->MCR |= (1 << LPIT_MCR_M_CEN_SHIFT) | (1u << LPIT_MCR_DBG_EN_SHIFT);

}
