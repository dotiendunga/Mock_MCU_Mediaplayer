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

pUSART_Config_t pUART1;

volatile uint32_t data = 0;

static void Clock_Setup();
static void GPIO_Setup();
static void ADC_Setup();
static void UART_Setup();

int main(void)
{
	SysTick_Config(SYSTICK_PERIOD_1MS);
	Clock_Setup();
	GPIO_Setup();
	ADC_Setup();
	UART_Setup();
	uint8_t a;
	ADC_Handle.Start(ADC0, ADC_CHANNEL_0, 12);
	while (1)
    {
		a = (uint8_t)data;
		Driver_UART.Transmit(pUART1, &a, 1);
		Delay_ms(1000);
    }
    return 0;
}

static void Clock_Setup()
{
	/* GPIO, UART */
	_ENABLE_PORTC_CLK();
	Clock_Enable_FastIRC();
	Clock_Enable_USART();
}

void UART_Setup()
{
	Driver_UART.Enable();
	pUART1->instance = LPUART1;
	pUART1->baudrate = USART_Baurate_9600;
	pUART1->datalength = 8;
	pUART1->direct = ARM_USART_LSB_FIRST;
	pUART1->parity = ARM_USART_PARITY_NONE;
	pUART1->stopbit = ARM_USART_1_STOP_BIT;
	Driver_UART.Init(pUART1);
}

void ADC_Setup()
{
	ADC_Handle.EnableCLK(ADC0, ADC_DIVIDE_1);
	ADC_Handle.SetResolution(ADC0, ADC_RESOLUTION_12BIT);
	ADC_Handle.SetTrigger(ADC0, ADC_Trigger_Sorfware);
	ADC_Handle.SetMode(ADC0, ADC_Mode_Oneshot);
	ADC_Handle.SetReference(ADC0, ADC_Ref_External);
	ADC_Handle.InterruptEnable(ADC0, ADC_CHANNEL_0, ADC_IT_ENABLE);
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
	data = ADC_Handle.Read(ADC0, ADC_CHANNEL_0);
	ADC_Handle.Start(ADC0, ADC_CHANNEL_0, 12);
}
