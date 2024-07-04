/***********************************************************************************
*
* Name 					: Driver_UART.c
* Author				: Group 3
* Created date			: July 4 2024
*
************************************************************************************/

#include "Driver_UART.h"

static int32_t ARM_USART_Enable(void)
{
    //Enable clock for Port C
	// PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
	//Pin 6 RX
	PORTC->PCR[USART_RX] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[USART_RX] |= PORT_PCR_MUX(PIN_MUX_CONTROL_GPIO);			//Mux uart
	//Pin 7 TX
	PORTC->PCR[USART_TX] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[USART_TX] |= PORT_PCR_MUX(PIN_MUX_CONTROL_GPIO);			//Mux uart
	/*Clock for UART module*/
	/*Clock source*/
	SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    /* Disable Fast IRC */
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1u);	 /* Divider = 1 */
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1u);       /* Enable Fast IRC */
	/*Select clock source and enable clock for LPUART1 module*/
	PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(3u); //Select FIRCDIV2_CLK
	PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_CGC_MASK;//Enable clock for LPUART1 module
	/*clock = 48MHz*/
	return ARM_DRIVER_OK;
}

static int32_t ARM_USART_SetBaudrate(USART_Config_t* usart)
{
	int32_t result = ARM_DRIVER_OK;
	usart->instance->CTRL &= ~(LPUART_CTRL_TE_MASK|LPUART_CTRL_RE_MASK);
	switch (usart->baudrate)
	{
	case USART_Baurate_9600:
		usart->instance->BAUD &= ~LPUART_BAUD_SBR_MASK;
		usart->instance->BAUD |= LPUART_BAUD_SBR(312U);
		break;
	case USART_Baurate_112500:
		usart->instance->BAUD &= ~LPUART_BAUD_SBR_MASK;
		usart->instance->BAUD |= LPUART_BAUD_SBR(27U);
		break;
    default:
        result = ARM_USART_ERROR_BAUDRATE;
	}
	return result;
}

static int32_t ARM_USART_SetDataLength(USART_Config_t* usart)
{
	int32_t result = ARM_DRIVER_OK;
	usart->instance->CTRL &= ~(LPUART_CTRL_TE_MASK|LPUART_CTRL_RE_MASK);
	/* Configure data */
	switch (usart->datalength)
	{
	case 7:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		usart->instance->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Select 7-bit data characters
		usart->instance->CTRL |= LPUART_CTRL_M7(1u);
        break;
	case 8:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		usart->instance->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Clear bit M7 to 0 to select 8to 10-bit data
		usart->instance->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 8-bit data characters
		usart->instance->CTRL &= ~LPUART_CTRL_M_MASK;
		break;
	case 9:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		usart->instance->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Clear bit M7 to 0 to select 8to 10-bit data
		usart->instance->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 9-bit data characters
		usart->instance->CTRL |= LPUART_CTRL_M(1u);
		break;
	case 10:
		//Clear bit M7 to 0 to select 8to 10-bit data
		usart->instance->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 10-bit data characters
		usart->instance->BAUD |= LPUART_BAUD_M10(1u);
		break;
    default:
        result = ARM_USART_ERROR_DATA_BITS;
	}
	return result;
}

static int32_t ARM_USART_SetParity(const USART_Config_t* usart)
{
	int32_t result = ARM_DRIVER_OK;
	usart->instance->CTRL &= ~LPUART_CTRL_PE_MASK;
    switch (usart->parity)
    {
    case ARM_USART_PARITY_ODD:
        usart->instance->CTRL |= LPUART_CTRL_PE_MASK;
        usart->instance->CTRL |= LPUART_CTRL_PT_MASK;
        break;
    case ARM_USART_PARITY_EVEN:
        usart->instance->CTRL |= LPUART_CTRL_PE_MASK;
        usart->instance->CTRL &= ~LPUART_CTRL_PT_MASK;
        break;
    case ARM_USART_PARITY_NONE:
		usart->instance->CTRL &= ~LPUART_CTRL_PE_MASK;
        usart->instance->CTRL &= ~LPUART_CTRL_PT_MASK;
        break;
    default:
        result = ARM_USART_ERROR_PARITY;
    }
	return result;
}

static int32_t ARM_USART_SetShiftDirection(const USART_Config_t* usart)
{
	int32_t result = ARM_DRIVER_OK;
    switch (usart->direct)
    {
    case ARM_USART_MSB_FIRST:
        usart->instance->STAT |= LPUART_STAT_MSBF_MASK;
        break;
	case ARM_USART_LSB_FIRST:
        usart->instance->STAT &= ~LPUART_STAT_MSBF_MASK;
        break;
    default:
        result = ARM_USART_ERROR_MODE;
    }
    return result;
}

static int32_t ARM_USART_SetStopBit(const USART_Config_t* usart)
{
    usart->instance->CTRL &= ~LPUART_CTRL_TXINV_MASK;
	usart->instance->STAT &= ~LPUART_STAT_RXINV_MASK;
	/* Configure stop bit*/
	switch (usart->stopbit)
	{
	case ARM_USART_1_STOP_BIT:
		usart->instance->BAUD &= ~LPUART_BAUD_SBNS_MASK;
		break;
	case ARM_USART_2_STOP_BIT:
		usart->instance->BAUD |= LPUART_BAUD_SBNS_MASK;
		break;
	default:
		return ARM_USART_ERROR_MODE;
		break;
	}
	return ARM_DRIVER_OK;
}


static int32_t ARM_USART_Uninitialize(void)
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
    usart->instance->CTRL |= LPUART_CTRL_TE_MASK;

	if(usart->datalength == 8)
	{
		const uint8_t *data_ptr = (const uint8_t *)data;
		for (uint32_t i = 0; i < ln; i++)
		{
			// Wait until transmit data register empty
			while (!(usart->instance->STAT & LPUART_STAT_TDRE_MASK));

			// Write Data to Tx Buffer
			usart->instance->DATA = data_ptr[i];
		}

		// Wait until transmission complete
		while (!(usart->instance->STAT & LPUART_STAT_TC_MASK));
	}
	else
	{
		/* 7bit 9bit 10bit */
		/* Do something */
	}

    // Disable Transmitter
    usart->instance->CTRL &= ~LPUART_CTRL_TE_MASK;

    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Receive(const USART_Config_t* usart, void *data, uint32_t ln)
{
    if (data == NULL || ln == 0)
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    // Enable Receiver
    LPUART1->CTRL |= LPUART_CTRL_RE_MASK;

    if(usart->datalength == 8)
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

static int32_t ARM_USART_Init(USART_Config_t* usart)
{
	if(ARM_USART_SetBaudrate(usart)) return ARM_USART_ERROR_BAUDRATE;
	if(ARM_USART_SetDataLength(usart)) return ARM_USART_ERROR_DATA_BITS;
	if(ARM_USART_SetParity(usart)) return ARM_USART_ERROR_PARITY;
	if(ARM_USART_SetShiftDirection(usart)) return ARM_USART_ERROR_MODE;
	if(ARM_USART_SetStopBit(usart)) return ARM_USART_ERROR_MODE;
	return ARM_DRIVER_OK;
}

ARM_DRIVER_USART Driver_UART = 
{
	.Enable = ARM_USART_Enable,
	.Uninitialize = ARM_USART_Uninitialize,
	.Init = ARM_USART_Init,
	.Transmit = ARM_USART_Transmit,
	.Receive = ARM_USART_Receive,
};