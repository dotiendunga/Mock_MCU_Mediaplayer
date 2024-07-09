/***********************************************************************************
*
* @Name 				: Systick.h
* @Author				: Tran Ba Than
* @Created date			: Jun 14 2024
*
************************************************************************************/

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "CMSIS_Common.h"

#define SYSTICK_PERIOD_1MS              47999u

uint32_t SysTick_Config(uint32_t ticks);

void SysTick_DeInit();

void SysTick_Handler(void);

uint32_t getTick();

void Delay_ms(uint32_t Delay);

#endif /* SYSTICK_H_ */
