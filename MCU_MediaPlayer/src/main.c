/***********************************************************************************
*
* Name 					: main.c
* Author				: Tran Ba Than
* Created date			: Jun 13 2024
*
************************************************************************************/
#include "main.h"
#include "Core_Systick.h"
#include "Core_Interrupt.h"


extern ARM_DRIVER_GPIO Driver_GPIO;
extern ADC_Handler_t ADC_Handle;
extern ARM_DRIVER_USART Driver_UART;

USART_Config_t UART1;

volatile uint32_t ADC_value = 0;
volatile uint8_t Button_Counter = 0;

/* Init function */
static void Clock_Setup();
static void GPIO_Setup();
static void ADC_Setup();
static void UART_Setup();

/* App */
void USART_send_messsage_button(uint8_t Peripheral_type, uint8_t Button_Pressed_counter);
void USART_send_messsage_ADC(/*uint8_t Peripheral_type*/);

int main(void)
{
	SysTick_Config(SYSTICK_PERIOD_1MS);
	Clock_Setup();
	GPIO_Setup();
	ADC_Setup();
	UART_Setup();
	LPIT_SetInterrupt(LPIT_CHANNEL_0);
	LPIT_Init(LPIT_CHANNEL_0, 47999000 / 2);
	NVIC_EnableIRQ(LPIT0_Ch0_IRQn);
	ADC_Handle.Start(ADC0, ADC_CHANNEL_0, 12);
	while(1)
    {
		if(ADC_Handle.IsConversionDone(ADC0, ADC_CHANNEL_0, 100) == HAL_OK)
		{
			USART_send_messsage_ADC();
			ADC_Handle.Start(ADC0, ADC_CHANNEL_0, 12);
		}
		Delay_ms(200);
    }
    return 0;
}
/*========================================== Init function==============================================*/
static void Clock_Setup()
{
	/* GPIO, UART */
	_ENABLE_PORTC_CLK();
	/* Fast IRC div2 */
	Clock_Enable_FastIRC(FAST_IRC_GATE_2, CLK_DIVIDE_BY_1);
	/* Enable clock UART 1*/
	Clock_Enable_Module(PCC_LPUART1_INDEX, FIRCDIV2_CLK);
	/* Enable clock ADC0 */
	Clock_Enable_Module(PCC_ADC0_INDEX, FIRCDIV2_CLK);
	/* Enbale clock LPIT */
	Clock_Enable_Module(PCC_LPIT_INDEX, FIRCDIV2_CLK);
}

void UART_Setup()
{
	UART1.Instance = LPUART1;
	UART1.Baudrate = 9600;
	UART1.Datalength = USART_8_BIT_DATA;
	UART1.Direct = USART_LSB_FIRST;
	UART1.Parity = USART_PARITY_NONE;
	UART1.Stopbit = USART_1_STOP_BIT;
	/* Init uart1 */
	Driver_UART.Init(&UART1);
}

void ADC_Setup()
{
	ADC_Handle.EnableCLK(ADC0, ADC_DIVIDE_1);
	ADC_Handle.SetResolution(ADC0, ADC_RESOLUTION_8BIT);
	ADC_Handle.SetTrigger(ADC0, ADC_TRIGGER_SOFTWARE);
	ADC_Handle.SetMode(ADC0, ADC_MODE_ONESHOT);
	ADC_Handle.SetReference(ADC0, ADC_REF_EXTERNAL);
	ADC_Handle.InterruptEnable(ADC0, ADC_CHANNEL_0, ADC_IT_DISABLE);
	NVIC_EnableIRQ(ADC0_IRQn);
}

void GPIO_Setup()
{
	/* RED led */
	Driver_GPIO.Setup          (RED_LED, NULL);
	Driver_GPIO.SetDirection   (RED_LED, ARM_GPIO_OUTPUT);
	Driver_GPIO.SetOutput      (RED_LED, 0U);

	/* BTN 1 */
	Driver_GPIO.Setup          (BTN1, ARM_GPIO_SignalEvent);
	Driver_GPIO.SetDirection   (BTN1, ARM_GPIO_INPUT);
	Driver_GPIO.SetOutputMode  (BTN1, ARM_GPIO_PUSH_PULL);
	Driver_GPIO.SetEventTrigger(BTN1, ARM_GPIO_TRIGGER_RISING_EDGE);
	Driver_GPIO.SetPullResistor(BTN1, ARM_GPIO_PULL_DOWN);

	/* BTN 2 */
	Driver_GPIO.Setup          (BTN2, ARM_GPIO_SignalEvent);
	Driver_GPIO.SetDirection   (BTN2, ARM_GPIO_INPUT);
	Driver_GPIO.SetOutputMode  (BTN2, ARM_GPIO_PUSH_PULL);
	Driver_GPIO.SetEventTrigger(BTN2, ARM_GPIO_TRIGGER_RISING_EDGE);
	Driver_GPIO.SetPullResistor(BTN2, ARM_GPIO_PULL_DOWN);

	NVIC_EnableIRQ(PORTC_IRQn);
}
/* Interrup handle function */
void ADC0_IRQHandler()
{
	ADC_value = ADC_Handle.Read(ADC0, ADC_CHANNEL_0);
	ADC_Handle.Start(ADC0, ADC_CHANNEL_0, 12);
}

void ARM_GPIO_SignalEvent(ARM_GPIO_Pin_t pin, uint32_t event)
{
	switch(pin)
	{
		case BTN1:
			Button_Counter++;
			Driver_GPIO.SetOutput(RED_LED, Button_Counter % 2);
			break;
		case BTN2:
			Button_Counter++;
			LPIT_Reset(LPIT_CHANNEL_0);
			LPIT_Start(LPIT_CHANNEL_0);
			break;
		default:
			break;
	}
}

/*=========================================== APP function=============================================*/
void USART_send_messsage_button(uint8_t Peripheral_type, uint8_t Button_Pressed_counter) 
{
    //Define Frame Uart
	uint8_t Frame_UART[4];
    uint8_t check_sum;

    // Create frame
    Frame_UART[0] = START_BYTE;     							// 1 byte start
    Frame_UART[1] = Peripheral_type;    						// 1 byte button type
    Frame_UART[2] = Button_Pressed_counter;        				// 1 byte count

    //Caculate checksum (XOR all byte)
    check_sum = Frame_UART[0] ^ Frame_UART[1] ^ Frame_UART[2];
    Frame_UART[3] = check_sum;       							// 1 byte checksum

	//Send message
	Driver_UART.Transmit(&UART1, &Frame_UART, 4);
}


void USART_send_messsage_ADC(/*uint8_t Peripheral_type*/) 
{
    //Define Frame Uart
	uint8_t Frame_UART[4];
    uint8_t check_sum;
	uint32_t ADC_temp = ADC_Handle.Read(ADC0, ADC_CHANNEL_0) / 4;	//64 levels
	if(ADC_temp != ADC_value)
	{
		ADC_value = ADC_temp;

		// Create frame
		Frame_UART[0] = START_BYTE;     							// 1 byte start
		Frame_UART[1] = BYTE_ADC;    								// 1 byte button
		Frame_UART[2] = (uint8_t)(ADC_value);	        			// 1 byte

		//Caculate checksum (XOR all byte)
		check_sum = Frame_UART[0] ^ Frame_UART[1] ^ Frame_UART[2];
		Frame_UART[3] = check_sum;       							// 1 byte checksum

		// Send message
		Driver_UART.Transmit(&UART1, &Frame_UART, 4);
	}
	else
	{
		/* Do something */
	}
}
