#include "Driver_UART.h"

// static ARM_USART_SignalEvent_t cb_signal_event = NULL;

// static int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t cb_event)
// {
//     int32_t result = ARM_DRIVER_OK;
//     if(cb_event = NULL)
//     {
//         return ARM_DRIVER_OK;
//     }
//     else
//     {
//         cb_signal_event = cb_event;
//     }
// }

static int32_t ARM_USART_Enable(void)
{
    //Enable clock for Port C
	PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
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

static int32_t ARM_USART_SetBaudrate(uint32_t baudrate)
{
	LPUART1->CTRL &= ~(LPUART_CTRL_TE_MASK|LPUART_CTRL_RE_MASK);
	switch (baudrate){
	case 9600:
		//OSR = 15 -> SBR = 312 (9600)
		LPUART1->BAUD &= ~LPUART_BAUD_SBR_MASK;
		LPUART1->BAUD |= LPUART_BAUD_SBR(312u);
		break;
    default:
        return ARM_USART_ERROR_BAUDRATE;
	}
	return ARM_DRIVER_OK;
}

static int32_t ARM_USART_ConfigData(USART_Config_t* config)
{
	LPUART1->CTRL &= ~(LPUART_CTRL_TE_MASK|LPUART_CTRL_RE_MASK);
	/* Configure data */
	switch (config->data)
	{
	case 7:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		LPUART1->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Select 7-bit data characters
		LPUART1->CTRL |= LPUART_CTRL_M7(1u);
        break;
	case 8:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		LPUART1->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Clear bit M7 to 0 to select 8to 10-bit data
		LPUART1->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 8-bit data characters
		LPUART1->CTRL &= ~LPUART_CTRL_M_MASK;
		break;
	case 9:
		//Set bit M10 to 0 to choose 7 to 9 bit data
		LPUART1->BAUD &= ~LPUART_BAUD_M10_MASK;
		//Clear bit M7 to 0 to select 8to 10-bit data
		LPUART1->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 9-bit data characters
		LPUART1->CTRL |= LPUART_CTRL_M(1u);
		break;
	case 10:
		//Clear bit M7 to 0 to select 8to 10-bit data
		LPUART1->CTRL &= ~LPUART_CTRL_M7_MASK;
		//Select 10-bit data characters
		LPUART1->BAUD |= LPUART_BAUD_M10(1u);
		break;
    default:
        return ARM_USART_ERROR_DATA_BITS;
	}
	return ARM_DRIVER_OK;
}

static int32_t ARM_USART_ConfigParity(USART_Config_t* config)
{
	/* Configure parity bit. Default parity none*/
	//Disable parity bit
	LPUART1->CTRL &= ~LPUART_CTRL_PE_MASK;
    switch (config->parity)
    {
    case ARM_USART_PARITY_ODD:
        LPUART1->CTRL |= LPUART_CTRL_PE(1u);
        LPUART1->CTRL |= LPUART_CTRL_PT(1u);
        break;
    case ARM_USART_PARITY_EVEN:
        LPUART1->CTRL |= LPUART_CTRL_PE(1u);
        LPUART1->CTRL &= ~LPUART_CTRL_PT_MASK;
        break;
    case ARM_USART_PARITY_NONE:
        break;
    default:
        return ARM_USART_ERROR_PARITY;
    }
	return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Configdirection(USART_Config_t* config)
{
    /* Configure direction frame. Default LSB */
	LPUART1->STAT &= ~ LPUART_STAT_MSBF_MASK;
    switch (config->direct)
    {
    case ARM_USART_MSB_FIRST:
        LPUART1->STAT |= LPUART_STAT_MSBF(1u);
        break;
    default:
        return ARM_USART_ERROR_MODE;
    }
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Configstopbit(USART_Config_t* config)
{
    LPUART1->CTRL &= ~LPUART_CTRL_TXINV_MASK;
	LPUART1->STAT &= ~LPUART_STAT_RXINV_MASK;
	/* Configure stop bit*/
	LPUART1->BAUD &= ~LPUART_BAUD_SBNS_MASK;
	if (config->stopbit == ARM_USART_2_STOP_BIT)
	{
		LPUART1->BAUD |= LPUART_BAUD_SBNS_MASK;
	}
	return ARM_DRIVER_OK;
}


static int32_t ARM_USART_Uninitialize(void)
{
    //Disable UART
	return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Transmit(const void *data, uint32_t num)
{
    if (data == NULL || num == 0)
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    // Enable Transmitter
    LPUART1->CTRL |= LPUART_CTRL_TE_MASK;

    const uint8_t *data_ptr = (const uint8_t *)data;

    for (uint32_t i = 0; i < num; i++)
    {
        // Wait until transmit data register empty
        while (!(LPUART1->STAT & LPUART_STAT_TDRE_MASK));

        // Write Data to Tx Buffer
        LPUART1->DATA = data_ptr[i];
    }

    // Wait until transmission complete
    while (!(LPUART1->STAT & LPUART_STAT_TC_MASK));

    // Disable Transmitter (optional, depends on your requirements)
    // LPUART1->CTRL &= ~LPUART_CTRL_TE_MASK;

    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Receive(void *data, uint32_t num)
{
    if (data == NULL || num == 0)
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    // Enable Receiver
    LPUART1->CTRL |= LPUART_CTRL_RE_MASK;

    uint8_t *data_ptr = (uint8_t *)data;

    for (uint32_t i = 0; i < num; i++)
    {
        // Wait until receive data register full (RDRF = 1)
        while (!(LPUART1->STAT & LPUART_STAT_RDRF_MASK));

        // Read Data from Rx Buffer
        data_ptr[i] = LPUART1->DATA;
    }

    // Disable Receiver (optional, depends on your requirements)
    // LPUART1->CTRL &= ~LPUART_CTRL_RE_MASK;
    return ARM_DRIVER_OK;
}


static int32_t ARM_USART_SendString(uint8_t * data)
{
	/* Enable Transmit */
	LPUART1->CTRL |= LPUART_CTRL_TE(1u);
	while (*data != '\0')
	{
		while(!(LPUART1->STAT & LPUART_STAT_TDRE_MASK));
		LPUART1->DATA = *(data++);
	}
	/* Disable Transmit */
	while(!(LPUART1->STAT & LPUART_STAT_TC_MASK));
	LPUART1->CTRL &= ~LPUART_CTRL_TE_MASK;
	return ARM_DRIVER_OK;
}


static int32_t ARM_USART_ReceiveBlocking(uint8_t *pRxBuffer, uint8_t len)
{
//	uint8_t data_extra;
	//Enable Receive
	LPUART1->CTRL |= LPUART_CTRL_RE(1u);
	//wait RDRF = 1
	while (len--)
	{
		while(!(LPUART1->STAT & LPUART_STAT_RDRF_MASK));
		//Read Rx Data Buffer
		*pRxBuffer = LPUART1->DATA;
		pRxBuffer++;
	}
	*pRxBuffer = '\0';
//	data_extra = LPUART1->DATA;
	//Disable Receive
	LPUART1->CTRL &= ~LPUART_CTRL_RE_MASK;\
	return ARM_DRIVER_OK;
}

// static int32_t ARM_USART_Transfer(const void *data_out, void *data_in, uint32_t num)
// {

// }

// static int32_t ARM_USART_SignalEvent(uint32_t event)
// {
//     // function body
// }
ARM_DRIVER_USART Driver_UART = 
{
	.Enable = ARM_USART_Enable,
	.Uninitialize = ARM_USART_Uninitialize,
    .SetBaudrate = ARM_USART_SetBaudrate,
    .SetData = ARM_USART_ConfigData,
    .SetParity = ARM_USART_ConfigParity,
    .SetDirection = ARM_USART_Configdirection,
	.Configstopbit = ARM_USART_Configstopbit,
	.Transmit = ARM_USART_Transmit,
	.SendString = ARM_USART_SendString,
	.Receive = ARM_USART_Receive,
	.ReceiveBlocking = ARM_USART_ReceiveBlocking
};
