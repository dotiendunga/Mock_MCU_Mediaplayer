/***********************************************************************************
*
* @Name 				: Systick.c
* @Author				: Tran Ba Than
* @Created date			: Jun 14 2024
*
************************************************************************************/
#include "Core_Systick.h"

uint32_t sysTick;

uint32_t SysTick_Config(uint32_t ticks)
{
    SysTick_DeInit();
    
    // Set Reload Value
    S32_SysTick->RVR = ticks-1;

    // Processor Clock
    S32_SysTick->CSR |= S32_SysTick_CSR_CLKSOURCE(1u);

    // Enable Systick Exeption
    S32_SysTick->CSR |= S32_SysTick_CSR_TICKINT_MASK;

    // Enable Systick 
    S32_SysTick->CSR |= S32_SysTick_CSR_ENABLE_MASK;
    return ARM_DRIVER_OK;
}

void SysTick_DeInit()
{
    sysTick = 0;
	S32_SysTick->CSR = 0u;
	S32_SysTick->CVR = 0u;
	S32_SysTick->RVR = 0u;
}

void SysTick_Handler(void)
{
	sysTick += 1U;
}

uint32_t getTick()
{
	return sysTick;
}

void Delay_ms(uint32_t Delay)
{
	uint32_t sTick = getTick();

	while(getTick() - sTick < Delay);
}
