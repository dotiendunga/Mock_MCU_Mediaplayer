/***********************************************************************************
*
* Name 					      : Driver_UART.h
* Author				      : Group 3
* Created date			      : July 4 2024
*
************************************************************************************/

#ifndef DRIVER_USART_H_
#define DRIVER_USART_H_

#include "CMSIS_Common.h"

//Status UART Error Codes
#define ARM_USART_ERROR_MODE                (ARM_DRIVER_ERROR_SPECIFIC - 1)
#define ARM_USART_ERROR_BAUDRATE            (ARM_DRIVER_ERROR_SPECIFIC - 2)
#define ARM_USART_ERROR_DATA_BITS           (ARM_DRIVER_ERROR_SPECIFIC - 3)
#define ARM_USART_ERROR_PARITY              (ARM_DRIVER_ERROR_SPECIFIC - 4)
#define ARM_USART_ERROR_STOP_BITS           (ARM_DRIVER_ERROR_SPECIFIC - 5)
#define ARM_USART_ERROR_FLOW_CONTROL        (ARM_DRIVER_ERROR_SPECIFIC - 6)
#define ARM_USART_ERROR_CPOL                (ARM_DRIVER_ERROR_SPECIFIC - 7)
#define ARM_USART_ERROR_CPHA                (ARM_DRIVER_ERROR_SPECIFIC - 8)

#define USART_RX                			6U
#define USART_TX                			7U
#define PIN_MUX_CONTROL_GPIO    			2U

//Data bit
typedef enum
{
	USART_7_bit_Data	= 7,
	USART_8_bit_Data	= 8,
	USART_9_bit_Data	= 9,
	USART_10_bit_Data	= 10,
} USART_Data_bit_t;

// Parity
typedef enum
{
	USART_Parity_none	= 0,
	USART_Parity_Odd	= 1,
	USART_Parity_Even	= 2,
} USART_Parity_bit_t;

// First transfer bit
typedef enum
{
	USART_LSB_FIRST = 0,
	USART_MSB_FIRST = 1,
} USART_First_transfer_bit_t;

// Stop bit
typedef enum
{
	USART_1_Stop_bit = 1,
	USART_2_Stop_bit = 2,
} USART_Stopbit_t;


// Baudrate
typedef enum
{
	USART_Baurate_9600 = 0,
	USART_Baurate_112500,
} USART_Baurate_t;


typedef void (*ARM_USART_SignalEvent_t) (uint32_t event);

typedef struct
{
	LPUART_Type 	*instance;
	uint32_t    	baudrate;
	uint8_t     	datalength;
	uint8_t     	parity;
	uint8_t     	stopbit;
	uint8_t     	direct;
} USART_Config_t;

typedef struct 
{
	int32_t (*Init)(USART_Config_t* usart);
	int32_t (*Uninitialize)(USART_Config_t* usart);
	int32_t (*Transmit)(const USART_Config_t* usart, const void *data, uint32_t ln);
	int32_t (*Receive)(const USART_Config_t* usart, void *data, uint32_t ln);
} const ARM_DRIVER_USART;

#endif /* DRIVER_USART_H_ */
