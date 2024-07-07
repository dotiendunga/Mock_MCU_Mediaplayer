/***********************************************************************************
*
* Name 					: Driver_UART.c
* Author				: Group 3
* Created date			: July 4 2024
*
************************************************************************************/

#include "Driver_UART.h"

static int32_t ARM_USART_Pin(void)
{
	//Pin 6 RX
	PORTC->PCR[USART_RX] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[USART_RX] |= PORT_PCR_MUX(PIN_MUX_CONTROL_GPIO);			//Mux uart
	//Pin 7 TX
	PORTC->PCR[USART_TX] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[USART_TX] |= PORT_PCR_MUX(PIN_MUX_CONTROL_GPIO);			//Mux uart

	return ARM_DRIVER_OK;
}

static int32_t ARM_USART_SetBaudrate(USART_Config_t* usart)
{
	int32_t result = ARM_DRIVER_OK;
	usart->Instance->CTRL &= ~(LPUART_CTRL_TE_MASK|LPUART_CTRL_RE_MASK);
	switch (usart->Baudrate)
	{
	case 9600:
		usart->Instance->BAUD &= ~LPUART_BAUD_SBR_MASK;
		usart->Instance->BAUD |= LPUART_BAUD_SBR(312U);
		break;
	case 112500:
		usart->Instance->BAUD &= ~LPUART_BAUD_SBR_MASK;
		usart->Instance->BAUD |= LPUART_BAUD_SBR(27U);
		break;
    default:
        result = ARM_USART_ERROR_BAUDRATE;
	}
	return result;
}

static int32_t ARM_USART_SetDataLength(USART_Config_t* usart)
{
	int32_t result = ARM_DRIVER_OK;
	usart->Instance->CTRL &= ~(LPUART_CTRL_TE_MASK|LPUART_CTRL_RE_MASK);
	/* Configure data */
	switch (usart->Datalength)
	{
	case USART_7_BIT_DATA:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		usart->Instance->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Select 7-bit data characters
		usart->Instance->CTRL |= LPUART_CTRL_M7(1u);
        break;
	case USART_8_BIT_DATA:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		usart->Instance->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Clear bit M7 to 0 to select 8to 10-bit data
		usart->Instance->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 8-bit data characters
		usart->Instance->CTRL &= ~LPUART_CTRL_M_MASK;
		break;
	case USART_9_BIT_DATA:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		usart->Instance->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Clear bit M7 to 0 to select 8to 10-bit data
		usart->Instance->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 9-bit data characters
		usart->Instance->CTRL |= LPUART_CTRL_M(1u);
		break;
	case USART_10_BIT_DATA:
		//Clear bit M7 to 0 to select 8to 10-bit data
		usart->Instance->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 10-bit data characters
		usart->Instance->BAUD |= LPUART_BAUD_M10(1u);
		break;
    default:
        result = ARM_USART_ERROR_DATA_BITS;
	}
	return result;
}

static int32_t ARM_USART_SetParity(const USART_Config_t* usart)
{
	int32_t result = ARM_DRIVER_OK;
	usart->Instance->CTRL &= ~LPUART_CTRL_PE_MASK;
    switch (usart->Parity)
    {
    case USART_PARITY_ODD:
        usart->Instance->CTRL |= LPUART_CTRL_PE_MASK;
        usart->Instance->CTRL |= LPUART_CTRL_PT_MASK;
        break;
    case USART_PARITY_EVEN:
        usart->Instance->CTRL |= LPUART_CTRL_PE_MASK;
        usart->Instance->CTRL &= ~LPUART_CTRL_PT_MASK;
        break;
    case USART_PARITY_NONE:
		usart->Instance->CTRL &= ~LPUART_CTRL_PE_MASK;
        usart->Instance->CTRL &= ~LPUART_CTRL_PT_MASK;
        break;
    default:
        result = ARM_USART_ERROR_PARITY;
    }
	return result;
}

static int32_t ARM_USART_SetShiftDirection(const USART_Config_t* usart)
{
	int32_t result = ARM_DRIVER_OK;
    switch (usart->Direct)
    {
    case USART_MSB_FIRST:
        usart->Instance->STAT |= LPUART_STAT_MSBF_MASK;
        break;
	case USART_LSB_FIRST:
        usart->Instance->STAT &= ~LPUART_STAT_MSBF_MASK;
        break;
    default:
        result = ARM_USART_ERROR_MODE;
    }
    return result;
}

static int32_t ARM_USART_SetStopBit(const USART_Config_t* usart)
{
    usart->Instance->CTRL &= ~LPUART_CTRL_TXINV_MASK;
	usart->Instance->STAT &= ~LPUART_STAT_RXINV_MASK;
	/* Configure stop bit*/
	switch (usart->Stopbit)
	{
	case USART_1_STOP_BIT:
		usart->Instance->BAUD &= ~LPUART_BAUD_SBNS_MASK;
		break;
	case USART_2_STOP_BIT:
		usart->Instance->BAUD |= LPUART_BAUD_SBNS_MASK;
		break;
	default:
		return ARM_USART_ERROR_MODE;
		break;
	}
	return ARM_DRIVER_OK;
}


static int32_t ARM_USART_Uninitialize(USART_Config_t* usart)
{
    //Disable UART
	return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Transmit(const USART_Config_t* usart, const void *data, uint32_t ln)
{
    if (data == NULL || ln == 0)
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    // Enable Transmitter
    usart->Instance->CTRL |= LPUART_CTRL_TE_MASK;

	if(usart->Datalength == 8)
	{
		const uint8_t *data_ptr = (const uint8_t *)data;
		for (uint32_t i = 0; i < ln; i++)
		{
			// Wait until transmit data register empty
			while (!(usart->Instance->STAT & LPUART_STAT_TDRE_MASK));

			// Write Data to Tx Buffer
			usart->Instance->DATA = data_ptr[i];
		}

		// Wait until transmission complete
		while (!(usart->Instance->STAT & LPUART_STAT_TC_MASK));
	}
	else
	{
		/* 7bit 9bit 10bit */
		/* Do something */
	}

    // Disable Transmitter
    usart->Instance->CTRL &= ~LPUART_CTRL_TE_MASK;

    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Transmit_IT(const USART_Config_t* usart, const void *data, uint32_t ln)
{
    if (data == NULL || ln == 0)
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }



    return ARM_DRIVER_OK;
}
static int32_t ARM_USART_Receive(const USART_Config_t* const usart, void *data, uint32_t ln)
{
    if (data == NULL || ln == 0)
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    // Enable Receiver
    LPUART1->CTRL |= LPUART_CTRL_RE_MASK;

    if(usart->Datalength == 8)
	{
		uint8_t *data_ptr = (uint8_t *)data;
		for (uint32_t i = 0; i < ln; i++)
		{
			// Wait until receive data register full (RDRF = 1)
			while (!(LPUART1->STAT & LPUART_STAT_RDRF_MASK));

			// Read Data from Rx Buffer
			data_ptr[i] = LPUART1->DATA;
		}
	}
	else
	{
		/* 7bit 9bit 10bit */
		/* Do something */
	}

    // Disable Receiver
    LPUART1->CTRL &= ~LPUART_CTRL_RE_MASK;
    return ARM_DRIVER_OK;
}
static int32_t ARM_USART_Receive_IT(const USART_Config_t* usart, void *data, uint32_t ln)
{

}

static int32_t ARM_USART_Init(USART_Config_t* usart)
{
	if(ARM_USART_Pin())	return ARM_USART_ERROR_MODE;
	if(ARM_USART_SetBaudrate(usart)) return ARM_USART_ERROR_BAUDRATE;
	if(ARM_USART_SetDataLength(usart)) return ARM_USART_ERROR_DATA_BITS;
	if(ARM_USART_SetParity(usart)) return ARM_USART_ERROR_PARITY;
	if(ARM_USART_SetShiftDirection(usart)) return ARM_USART_ERROR_MODE;
	if(ARM_USART_SetStopBit(usart)) return ARM_USART_ERROR_MODE;
	return ARM_DRIVER_OK;
}

ARM_DRIVER_USART Driver_UART = 
{
	.Init = ARM_USART_Init,
	.Uninitialize = ARM_USART_Uninitialize,
	.Transmit = ARM_USART_Transmit,
	.Transmit_IT = ARM_USART_Transmit_IT,
	.Receive = ARM_USART_Receive,
	.Receive_IT = ARM_USART_Receive_IT,
};