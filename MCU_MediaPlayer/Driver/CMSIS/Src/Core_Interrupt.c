/***********************************************************************************
*
* @Name 				: Core_Interrupt.c
* @Author				: Tran Ba Than
* @Created date			: Jun 23 2024
*
************************************************************************************/
#include "Core_Interrupt.h"

void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    if ((int32_t)(IRQn) >= 0)
    {
        S32_NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] |= (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    }
}

void NVIC_DisableIRQ(IRQn_Type IRQn)
{
    if ((int32_t)(IRQn) >= 0)
    {
        S32_NVIC->ICER[(((uint32_t)IRQn) >> 5UL)] |= (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    }
}

void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    if ((int32_t)(IRQn) >= 0)
    {
        S32_NVIC->IP[((uint32_t)IRQn)] = (uint32_t)((priority << 4) & 0xFFUL);  //Note: uint32_t is more benifical than uint8_t here
    }
}