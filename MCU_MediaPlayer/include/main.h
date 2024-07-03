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

#define _ENABLE_PORTA_CLK()			PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTB_CLK()			PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTC_CLK()			PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTD_CLK()			PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTE_CLK()			PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;

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




#endif /* MAIN_H_ */
