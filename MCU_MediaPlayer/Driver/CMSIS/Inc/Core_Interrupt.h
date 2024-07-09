/***********************************************************************************
*
* @Name 				: Core_Interrupt.h
* @Author				: Tran Ba Than
* @Created date			: Jun 23 2024
*
************************************************************************************/

#ifndef INC_CORE_INTERRUPT_H_
#define INC_CORE_INTERRUPT_H_

#include "S32K144.h"

// Enable interrupt
void NVIC_EnableIRQ(IRQn_Type IRQn);

// Disable interrupt
void NVIC_DisableIRQ(IRQn_Type IRQn);

// Set Priority
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);

#endif /* INC_CORE_INTERRUPT_H_ */
