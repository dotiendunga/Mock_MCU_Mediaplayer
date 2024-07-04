/***********************************************************************************
*
* Name 					: Clock.c
* Author				: Tran Ba Than
* Created date			: July 4 2024
*
************************************************************************************/

#include "Clock.h"

void Clock_Enable_FastIRC(FAST_IRC_Gate_t gate,CLOCK_Divide_t div)
{
    SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    /* Disable Fast IRC */
    switch (gate)
    {
    case FAST_IRC_GATE_1:
        SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV1(div);
        break;
    case FAST_IRC_GATE_2:
        SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(div);
        break;
    default:
        break;
    }
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;       /* Enable Fast IRC */
}

void Clock_Enable_Module(uint32_t moduleid, uint32_t gate)
{
    // Select clock source
	PCC->PCCn[moduleid] |= PCC_PCCn_PCS(gate); 
    // Enable clock
	PCC->PCCn[moduleid] |= PCC_PCCn_CGC_MASK;
}
