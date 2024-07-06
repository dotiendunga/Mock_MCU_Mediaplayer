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
#include "ADC.h"
#include "Driver_UART.h"

extern ARM_DRIVER_GPIO Driver_GPIO;
extern ADC_Handler_t ADC_Handle;
extern ARM_DRIVER_USART Driver_UART;

USART_Config_t UART1;

volatile uint32_t ADC_value = 0;

/* Init function */
static void Clock_Setup();
static void GPIO_Setup();
static void ADC_Setup();
static void UART_Setup();

/* App */
void sentVolume()
{
	uint32_t volume = ADC_value / 128 * 100;
	Driver_UART.Transmit(&UART1, &volume, 1);
}

int main(void)
{
	SysTick_Config(SYSTICK_PERIOD_1MS);
	Clock_Setup();
	GPIO_Setup();
	ADC_Setup();
	UART_Setup();
	ADC_Handle.Start(ADC0, ADC_CHANNEL_0, 12);
	while(1)
    {
		if(ADC_Handle.IsConversionDone(ADC0, ADC_CHANNEL_0, 100) == HAL_OK)
		{
			ADC_value = ADC_Handle.Read(ADC0, ADC_CHANNEL_0);
			sentVolume();
			ADC_Handle.Start(ADC0, ADC_CHANNEL_0, 12);
		}
		Delay_ms(500);
    }
    return 0;
}

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
	ADC_Handle.SetResolution(ADC0, ADC_RESOLUTION_12BIT);
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
}

void ADC0_IRQHandler()
{
	ADC_value = ADC_Handle.Read(ADC0, ADC_CHANNEL_0);
	ADC_Handle.Start(ADC0, ADC_CHANNEL_0, 12);
}
