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
#include "Clock.h"

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
	USART_7_BIT_DATA		= 7,
	USART_8_BIT_DATA		= 8,
	USART_9_BIT_DATA		= 9,
	USART_10_BIT_DATA		= 10,
} USART_Datalength_t;

// Parity
typedef enum
{
	USART_PARITY_NONE		= 0,
	USART_PARITY_ODD		= 1,
	USART_PARITY_EVEN		= 2,
} USART_Parity_t;

// First transfer bit
typedef enum
{
	USART_LSB_FIRST 		= 0,
	USART_MSB_FIRST 		= 1,
} USART_Direction_t;

// Stop bit
typedef enum
{
	USART_1_STOP_BIT 		= 1,
	USART_2_STOP_BIT 		= 2,
} USART_Stopbit_t;

// Interrupt
typedef enum
{
	USART_INTERRUPT_RX 		= 1,
	USART_INTERRUPT_TX 		= 2,
	USART_INTERRUPT_TX_RX 	= 3
} USART_Interrupt_t;


typedef void (*ARM_USART_SignalEvent_t) (uint32_t event);

typedef struct
{
	LPUART_Type 		*Instance;
	uint32_t    		Baudrate;
	USART_Datalength_t  Datalength;
	USART_Parity_t     	Parity;
	USART_Stopbit_t     Stopbit;
	USART_Direction_t   Direct;
	USART_Interrupt_t	It;
} USART_Config_t;

typedef struct 
{
	int32_t (*Init)(USART_Config_t* usart);
	int32_t (*Uninitialize)(USART_Config_t* usart);
	int32_t (*Transmit)(const USART_Config_t* usart, const void *data, uint32_t ln);
	int32_t (*Transmit_IT)(const USART_Config_t* usart, const void *data, uint32_t ln);
	int32_t (*Receive)(const USART_Config_t* usart, void *data, uint32_t ln);
	int32_t (*Receive_IT)(const USART_Config_t* usart, void *data, uint32_t ln);
} const ARM_DRIVER_USART;



#endif /* DRIVER_USART_H_ */
