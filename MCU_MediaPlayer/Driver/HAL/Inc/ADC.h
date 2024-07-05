/***********************************************************************************
*
* Name 					    : ADC.h
* Author				    : Tran Ba Than
* Created date				: Jun 25 2024
*
************************************************************************************/

#ifndef INC_DRIVER_ADC_H_
#define INC_DRIVER_ADC_H_

#include "HAL_Common.h"
#include "Core_Systick.h"

#define ADC_CHANNEL_MAX                 16U
#define ADC_CHANNLE_IS_AVAILABLE(n)    	((n) < ADC_CHANNEL_MAX)

#define ADC_CHANNEL_0					0U
#define ADC_CHANNEL_1					1U
#define ADC_CHANNEL_2					2U
#define ADC_CHANNEL_3					3U
#define ADC_CHANNEL_4					4U
#define ADC_CHANNEL_5					5U
#define ADC_CHANNEL_6					6U
#define ADC_CHANNEL_7					7U
#define ADC_CHANNEL_8					8U
#define ADC_CHANNEL_9					9U
#define ADC_CHANNEL_10					10U
#define ADC_CHANNEL_11					11U
#define ADC_CHANNEL_12					12U
#define ADC_CHANNEL_13					13U
#define ADC_CHANNEL_14					14U
#define ADC_CHANNEL_15					15U

typedef ADC_Type* ADCx_t;
typedef uint32_t ADC_Channel_t;
typedef uint32_t ADC_Input_t;

typedef enum
{
	ADC_RESOLUTION_8BIT = 0,
	ADC_RESOLUTION_12BIT,
	ADC_RESOLUTION_10BIT
} ADC_Resolution_t;


typedef enum
{
	ADC_DIVIDE_1 = 0,
	ADC_DIVIDE_2,
	ADC_DIVIDE_4,
	ADC_DIVIDE_8
} ADC_CLK_Ratio_t;

typedef enum
{
	ADC_Trigger_Sorfware = 0,
	ADC_Trigger_Hardware
} ADC_Trigger_t;

typedef enum
{
	ADC_Ref_External = 0,
	ADC_Ref_Internal
} ADC_RefSelection_t;

typedef enum
{
	ADC_Mode_Oneshot = 0,
	ADC_Mode_Continuous
} ADC_Mode_t;

typedef enum
{
	ADC_IT_DISABLE = 0,
	ADC_IT_ENABLE
} ADC_Interrupt_t;

typedef struct
{
	HAL_StatusTypeDef (*EnableCLK)(ADCx_t ADCx, ADC_CLK_Ratio_t div);
	HAL_StatusTypeDef (*SetResolution) (ADCx_t ADCx, ADC_Resolution_t res);
	HAL_StatusTypeDef (*SetTrigger) (ADCx_t ADCx, ADC_Trigger_t trig);
	HAL_StatusTypeDef (*SetReference) (ADCx_t ADCx, ADC_RefSelection_t ref);
	HAL_StatusTypeDef (*SetMode) (ADCx_t ADCx, ADC_Mode_t mode);
	HAL_StatusTypeDef (*InterruptEnable) (ADCx_t ADCx, ADC_Channel_t channel , ADC_Interrupt_t it);
	HAL_StatusTypeDef (*Start) (ADCx_t ADCx, ADC_Channel_t channel, ADC_Input_t input);
	HAL_StatusTypeDef (*IsConversionDone) (ADCx_t ADCx, ADC_Channel_t channel, uint32_t timeout);
	uint32_t (*Read) (ADCx_t ADCx, ADC_Channel_t channel);
} ADC_Handler_t;


#endif /* INC_DRIVER_ADC_H_ */
