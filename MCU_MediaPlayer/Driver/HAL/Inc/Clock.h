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



#endif /* HAL_INC_CLOCK_H_ */
