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
extern ARM_DRIVER_GPIO Driver_GPIO;

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
	LPIT_ClearIRQsFlag(LPIT_CHANNEL_0);
    USART_send_messsage_button(BYTE_BUTTON_1);
    Button_Counter = 0;
    Driver_GPIO.SetOutput(RED_LED, 1U);
}

void LPIT0_Ch1_IRQHandler()
{
	LPIT_ClearIRQsFlag(LPIT_CHANNEL_1);
    USART_send_messsage_button(BYTE_BUTTON_2);
    Button_Counter = 0;
    Driver_GPIO.SetOutput(RED_LED, 1U);
}

void LPIT0_Ch2_IRQHandler()
{
	LPIT_ClearIRQsFlag(LPIT_CHANNEL_2);

}

#endif /* S32K144_IT_C_ */
