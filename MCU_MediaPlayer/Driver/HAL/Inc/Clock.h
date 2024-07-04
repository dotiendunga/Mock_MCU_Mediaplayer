/***********************************************************************************
*
* Name 					      : Clock.h
* Author				      : Tran Ba Than
* Created date			      : July 4 2024
*
************************************************************************************/
#ifndef HAL_INC_CLOCK_H_
#define HAL_INC_CLOCK_H_

#include "HAL_Common.h"

#define _ENABLE_PORTA_CLK()			PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTB_CLK()			PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTC_CLK()			PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTD_CLK()			PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTE_CLK()			PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;

void Clock_Enable_FastIRC(/*uint32_t div*/)
{
    SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    /* Disable Fast IRC */
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1u);	 /* Divider = 1 */
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1u);       /* Enable Fast IRC */
}
void Clock_Enable_USART()
{
    /*Select clock source and enable clock for LPUART1 module*/
	PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(3u); //Select FIRCDIV2_CLK
	PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_CGC_MASK;//Enable clock for LPUART1 module
}

#endif /* HAL_INC_CLOCK_H_ */
