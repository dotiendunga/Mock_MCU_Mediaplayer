/***********************************************************************************
*
* @Name 				: S32K144_IT.c
* @Author				: Tran Ba Than
* @Created date			: Jun 27 2024
*
************************************************************************************/

#include "S32K144_IT.h"

#ifndef S32K144_IT_C_
#define S32K144_IT_C_

extern uint8_t Button_Counter;

void PORTC_IRQHandler()
{
    GPIO_Check_IRQs(BTN1);
    GPIO_Check_IRQs(BTN2);
}

__weak void ADC0_IRQHandler()
{

}

void LPIT0_Ch0_IRQHandler()
{
    LPIT0->MSR |= (1 << 0);
    USART_send_messsage_button(BYTE_BUTTON_1, Button_Counter);
    Button_Counter = 0;
}


#endif /* S32K144_IT_C_ */
