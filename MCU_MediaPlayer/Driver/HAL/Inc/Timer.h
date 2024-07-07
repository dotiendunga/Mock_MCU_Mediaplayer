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

void LPIT_Init(LPIT_Channel_t channel, uint32_t val);
void LPIT_Reset(LPIT_Channel_t channel);
void LPIT_Start(LPIT_Channel_t channel);
uint32_t LPIT_ReadCurrentValue(LPIT_Channel_t channel);
void LPIT_SetInterrupt(LPIT_Channel_t channel);
bool LPIT_CheckIRQs(LPIT_Channel_t channel);


#endif /* HAL_INC_TIMER_H_ */
