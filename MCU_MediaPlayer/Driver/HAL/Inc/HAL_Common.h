/***********************************************************************************
*
* Name 					      : HAL_Commom.h
* Author				      : Tran Ba Than
* Created date			  : Jun 25 2024
*
************************************************************************************/

#ifndef HAL_INC_HAL_COMMON_H_
#define HAL_INC_HAL_COMMON_H_

#include "stdint.h"
#include "S32K144.h"

// Define Null pointer
#ifndef NULL
#define NULL ((void*)0)
#endif

// Define week keyword
#ifndef __weak
#define __weak __attribute__((weak))
#endif

// Define HAL Error
typedef enum 
{
  HAL_OK = 0,
  HAL_PARAMETRER_ERROR,
  HAL_ERROR,
  HAL_BUSY,
  HAL_TIMEOUT
} HAL_StatusTypeDef;


#endif /* HAL_INC_HAL_COMMON_H_ */
