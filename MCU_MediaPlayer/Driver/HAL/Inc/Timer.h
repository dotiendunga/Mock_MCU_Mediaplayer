/***********************************************************************************
*
* Name 					    : Timer.h
* Author				    : Tran Ba Than
* Created date			    : Jun 25 2024
*
************************************************************************************/

#ifndef HAL_INC_TIMER_H_
#define HAL_INC_TIMER_H_

#include "HAL_Common.h"
#include "stdbool.h"

typedef enum
{
    LPIT_CHANNEL_0 = 0,
    LPIT_CHANNEL_1,
    LPIT_CHANNEL_2,
    LPIT_CHANNEL_3
} LPIT_Channel_t;

static inline void LPIT_Init(LPIT_Channel_t channel, uint32_t val)
{
    /*Enable M_CEN first*/
    LPIT0->MCR |= (1 << LPIT_MCR_M_CEN_SHIFT) | (1u << LPIT_MCR_DBG_EN_SHIFT);              // Enable with debug mode
    LPIT0->TMR[(uint32_t)channel].TVAL = val;
    LPIT0->TMR[(uint32_t)channel].TCTRL |= LPIT_TMR_TCTRL_TSOI_MASK;                        // Oneshot
}

static inline void LPIT_Reset(LPIT_Channel_t channel)
{
    LPIT0->CLRTEN |= (1 << (uint32_t)channel);
}

static inline void LPIT_Start(LPIT_Channel_t channel)
{
    LPIT0->SETTEN |= (1 << (uint32_t)channel);
}

static inline uint32_t LPIT_ReadCurrentValue(LPIT_Channel_t channel)
{
    return LPIT0->TMR[(uint32_t)channel].CVAL;
}

static inline void LPIT_SetInterrupt(LPIT_Channel_t channel)
{
    LPIT0->MIER |= (1 << (uint32_t)channel);
}

static inline bool LPIT_CheckIRQs(LPIT_Channel_t channel)
{
    return (LPIT0->MSR & (1 << (uint32_t)channel)) >> (uint32_t)channel;
}

static inline void LPIT_ClearIRQsFlag(LPIT_Channel_t channel)
{
	LPIT0->MSR |= (1 << channel);
}


#endif /* HAL_INC_TIMER_H_ */
