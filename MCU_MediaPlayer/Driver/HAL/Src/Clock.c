/***********************************************************************************
*
* Name 					: Clock.c
* Author				: Tran Ba Than
* Created date			: July 4 2024
*
************************************************************************************/

#include "Clock.h"

/* Range of frequency */
static const uint32_t fircFreq[] =
{
    48000000U,      // 48 MHz
    52000000U,      // 52 MHz
    56000000U,      // 56 MHz
    60000000U       // 60 MHz
};

static const uint32_t fircDividers[] =
{
	0U,				// Disable
    1U,             // 0b000
    2U,             // 0b001
    4U,             // 0b010
    8U,             // 0b011
    16U,            // 0b100
    32U,            // 0b101
    64U             // 0b110
};

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

uint32_t Clock_Get_FIRCDIV2_CLK()
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    freq = fircFreq[Clock_GetFircRange(SCG)];
    div  = fircDividers[Clock_GetFircDivider_2(SCG)];
    return (uint32_t)(freq/div);
}
