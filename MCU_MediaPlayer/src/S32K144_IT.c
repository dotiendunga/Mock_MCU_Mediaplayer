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
extern ARM_DRIVER_USART Driver_UART;
extern USART_Config_t UART1;
extern bool isPlaying;

extern uint8_t UART_Rx_Data[4];

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

void LPUART1_RxTx_IRQHandler()
{
	static int count = 0;
	uint8_t buffer;
    Driver_UART.Receive_IT(&UART1, &buffer, 1);
    UART_Rx_Data[count++] = buffer;
    if(UART_Rx_Data[0] != 0xAA)
    {
		/* Start byte is wrong */
    	UART_Rx_Data[0] = 0U;
		count = 0;
    }
    else
    {
		/* Start byte is correct */
    	if(count == 4)
    	{
    		if(UART_Rx_Data[3] == crc8(UART_Rx_Data, 3))
    		{
    			isPlaying = UART_Rx_Data[2];
    			Driver_GPIO.SetOutput(BLUE_LED, 1);
    			count = 0;
    		}
    		else
    		{
    			/* Do something */
    		}
    	}
    	else
    	{
    		/* Do something */
    	}
    }
}

#endif /* S32K144_IT_C_ */
