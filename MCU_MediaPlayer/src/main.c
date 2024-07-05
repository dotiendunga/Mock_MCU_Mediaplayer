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


#define START_BYTE		0xAA;
#define BYTE_BUTTON_1	12U;
#define BYTE_BUTTON_2	13U;
#define BYTE_ADC		14U;

uint8_t Button_Pressed_counter;


void USART_send_messsage_button(uint8_t Peripheral_type, uint8_t Button_Pressed_counter) 
{
    //Define Frame Uart
	uint8_t Frame_UART[4];
    uint8_t check_sum;


    // Tạo frame
    Frame_UART[0] = START_BYTE;     						// 1 byte start
    Frame_UART[1] = Peripheral_type;    					// 1 byte phân loại button
    Frame_UART[2] = Button_Pressed_counter;        		// 1 byte đếm counter số lần nhấn button

    //Caculate checksum (XOR all byte)
    check_sum = Frame_UART[0] ^ Frame_UART[1] ^ Frame_UART[2];
    Frame_UART[3] = check_sum;       	// 1 byte checksum

    // Truyền frame qua UART
    for (int i = 0; i < 4; i++) {
        LPUART_WriteByte(LPUART1, Frame_UART[i]);
    }
}


void USART_send_messsage_ADC(uint8_t Peripheral_type, uint8_t Data_ADC) 
{
    //Define Frame Uart
	uint8_t Frame_UART[4];
    uint8_t check_sum;


    // Tạo frame
    Frame_UART[0] = START_BYTE;     						// 1 byte start
    Frame_UART[1] = Peripheral_type;    					// 1 byte phân loại button
    Frame_UART[2] = Data_ADC;        					// 1 byte đếm counter số lần nhấn button

    //Caculate checksum (XOR all byte)
    check_sum = Frame_UART[0] ^ Frame_UART[1] ^ Frame_UART[2];
    Frame_UART[3] = check_sum;       	// 1 byte checksum

    // Truyền frame qua UART
    for (int i = 0; i < 4; i++) {
        LPUART_WriteByte(LPUART1, Frame_UART[i]);
    }
}

USART_Config_t UART1;

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
		Driver_UART.Transmit(&UART1, &a, 1);
		Delay_ms(1000);
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
	UART1.instance 		= LPUART1;
	UART1.baudrate 		= USART_BAURATE_9600;
	UART1.datalength 	= USART_8_BIT_DATA;
	UART1.direct 		= USART_LSB_FIRST;
	UART1.parity 		= USART_PARITY_NONE;
	UART1.stopbit 		= USART_1_STOP_BIT;
	Driver_UART.Init(&UART1);
}

void ADC_Setup()
{
	ADC_Handle.EnableCLK(ADC0, ADC_DIVIDE_1);
	ADC_Handle.SetResolution(ADC0, ADC_RESOLUTION_12BIT);
	ADC_Handle.SetTrigger(ADC0, ADC_TRIGGER_SOFTWARE);
	ADC_Handle.SetMode(ADC0, ADC_MODE_ONESHOT);
	ADC_Handle.SetReference(ADC0, ADC_REF_EXTERNAL);
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
