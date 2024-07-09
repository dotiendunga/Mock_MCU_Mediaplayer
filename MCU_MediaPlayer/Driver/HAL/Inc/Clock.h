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

// Gate for LPIT, LPSPI, FLEXIO, LPI2C, LPUART, ADC, FTM, LPTMR, ENET
#define SOSCDIV2_CLK                0x01
#define SIRCDIV2_CLK                0x02
#define FIRCDIV2_CLK                0x03
#define SPLLDIV2_CLK                0x06

// Clock divide
typedef enum
{
    CLK_DIVIDE_DISABLE = 0,
    CLK_DIVIDE_BY_1,
    CLK_DIVIDE_BY_2,
    CLK_DIVIDE_BY_4,
    CLK_DIVIDE_BY_8,
    CLK_DIVIDE_BY_16,
    CLK_DIVIDE_BY_32,
    CLK_DIVIDE_BY_64
} CLOCK_Divide_t;

/* GPIO */
#define _ENABLE_PORTA_CLK()			PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTB_CLK()			PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTC_CLK()			PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTD_CLK()			PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
#define _ENABLE_PORTE_CLK()			PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;

/* FastIRC */
typedef enum
{
    FAST_IRC_GATE_1 = 0,
    FAST_IRC_GATE_2
} FAST_IRC_Gate_t;

void Clock_Enable_FastIRC(FAST_IRC_Gate_t gate, CLOCK_Divide_t div);

/* Enable module clock */
void Clock_Enable_Module(uint32_t moduleid, uint32_t gate);

// Get frequency
uint32_t Clock_Get_FIRCDIV2_CLK();


/* Inline */
static inline uint32_t Clock_GetFircRange(const SCG_Type* const base)
{
    return ((base->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) >> SCG_FIRCCFG_RANGE_SHIFT);
}

static inline uint32_t Clock_GetFircDivider_2(const SCG_Type const * base)
{
    return (base->FIRCDIV & SCG_FIRCDIV_FIRCDIV2_MASK) >> SCG_FIRCDIV_FIRCDIV2_SHIFT;
}
#endif /* HAL_INC_CLOCK_H_ */
