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

// Parity
#define ARM_USART_PARITY_NONE               0U
#define ARM_USART_PARITY_ODD                1U
#define ARM_USART_PARITY_EVEN               2U

// First transfer bit
#define ARM_USART_LSB_FIRST                 0U
#define ARM_USART_MSB_FIRST                 1U

// Stop bit
#define ARM_USART_1_STOP_BIT                1U
#define ARM_USART_2_STOP_BIT                2U

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
	int32_t (*Enable)(void);
	int32_t (*Uninitialize)(void);
	int32_t (*Init)(USART_Config_t* usart);
	int32_t (*Transmit)(const USART_Config_t* usart, const void *data, uint32_t ln);
	int32_t (*Receive)(const USART_Config_t* usart, void *data, uint32_t ln);
	// int32_t (*ReceiveBlocking)(uint8_t *pRxBuffer, uint8_t len);
	// int32_t (*SetBaudrate)(USART_Config_t* usart);
	// int32_t (*SetData)(USART_Config_t* usart);
	// int32_t (*SetParity)(USART_Config_t* usart);
	// int32_t (*SetDirection)(USART_Config_t* usart);
	// int32_t (*SetStopBit)(USART_Config_t* usart);
	// int32_t (*SendString)(uint8_t * data);
} const ARM_DRIVER_USART;

#endif /* DRIVER_USART_H_ */
