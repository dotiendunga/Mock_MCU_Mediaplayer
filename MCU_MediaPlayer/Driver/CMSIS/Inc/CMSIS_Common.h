/*
 * CMSIS_DEF.h
 *
 *  Created on: Jun 24, 2024
 *      Author: Tran Thoi
 */

#ifndef INC_CMSIS_DEF_H_
#define INC_CMSIS_DEF_H_

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

//Status Error Codes
#define ARM_DRIVER_OK                    0
#define ARM_DRIVER_ERROR                -1
#define ARM_DRIVER_ERROR_BUSY           -2
#define ARM_DRIVER_ERROR_TIMEOUT        -3
#define ARM_DRIVER_ERROR_UNSUPPORTED    -4
#define ARM_DRIVER_ERROR_PARAMETER      -5
#define ARM_DRIVER_ERROR_SPECIFIC       -6

#endif /* INC_CMSIS_DEF_H_ */
