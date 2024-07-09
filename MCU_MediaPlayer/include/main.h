/***********************************************************************************
*
* @Name 				: main.h
* @Author				: Tran Ba Than
* @Created date			: Jun 13 2024
*
************************************************************************************/


#ifndef MAIN_H_
#define MAIN_H_

#include "S32K144.h"
#include "S32K144_IT.h"
#include "Driver_GPIO.h"
#include "device_registers.h"
#include "Clock.h"
#include "ADC.h"
#include "Timer.h"
#include "Driver_UART.h"

#define GPIO_PTC_12			76U
#define BTN1				GPIO_PTC_12

#define GPIO_PTC_13			77U
#define BTN2				GPIO_PTC_13

#define GPIO_PTD_0			96U
#define BLUE_LED			GPIO_PTD_0

#define GPIO_PTD_15			111U
#define RED_LED			    GPIO_PTD_15

#define GPIO_PTD_16			112U
#define GREEN_LED			GPIO_PTD_16

#define LED1_FREQ			1000	//ms
#define LED2_FREQ			2000	//ms

#define START_BYTE		    0xAA
#define BYTE_BUTTON_1	    0x01U
#define BYTE_BUTTON_2	    0x02U
#define BYTE_ADC		    0x03U

/* App */
void USART_send_messsage_button(uint8_t Button_type);
void USART_send_messsage_ADC();


#endif /* MAIN_H_ */
