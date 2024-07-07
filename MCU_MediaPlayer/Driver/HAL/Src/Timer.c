/***********************************************************************************
*
* Name 					    : Timer.c
* Author				    : Tran Ba Than
* Created date			    : Jun 25 2024
*
************************************************************************************/

#include "Timer.h"

void LPIT_Init(LPIT_Channel_t channel, uint32_t val)
{
    /*Enable M_CEN first*/
    LPIT0->MCR |= (1 << LPIT_MCR_M_CEN_SHIFT) | (1u << LPIT_MCR_DBG_EN_SHIFT);              // Enable with debug mode
    LPIT0->TMR[(uint32_t)channel].TVAL = val;
    LPIT0->TMR[(uint32_t)channel].TCTRL |= LPIT_TMR_TCTRL_TSOI_MASK;                        // Oneshot
}

void LPIT_Reset(LPIT_Channel_t channel)
{
    LPIT0->CLRTEN |= (1 << (uint32_t)channel);   
}

void LPIT_Start(LPIT_Channel_t channel)
{
    LPIT0->SETTEN |= (1 << (uint32_t)channel);
}

uint32_t LPIT_ReadCurrentValue(LPIT_Channel_t channel)
{
    return LPIT0->TMR[(uint32_t)channel].CVAL;
}

void LPIT_SetInterrupt(LPIT_Channel_t channel)
{
    LPIT0->MIER |= (1 << (uint32_t)channel);
}

bool LPIT_CheckIRQs(LPIT_Channel_t channel)
{
    return (LPIT0->MSR & (1 << (uint32_t)channel)) >> (uint32_t)channel;
}
