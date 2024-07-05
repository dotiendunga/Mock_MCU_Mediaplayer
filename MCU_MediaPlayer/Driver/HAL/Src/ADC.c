/***********************************************************************************
*
* Name 					    : ADC.c
* Author				    : Tran Ba Than
* Created date			    : Jun 25 2024
*
************************************************************************************/

#include "ADC.h"

HAL_StatusTypeDef ADC_EnableCLK(ADCx_t ADCx, ADC_CLK_Ratio_t div)
{
    /* Input clock: Alternate clock 1, Divide ratio = 1 */
	ADCx->CFG1 &= ~ (ADC_CFG1_ADICLK_MASK | ((uint32_t)div << ADC_CFG1_ADIV_SHIFT));
	return HAL_OK;
}

static HAL_StatusTypeDef ADC_SetResolution(ADCx_t ADCx, ADC_Resolution_t res)
{
    HAL_StatusTypeDef result = HAL_OK;
    switch (res)
    {
        case ADC_RESOLUTION_8BIT:
        case ADC_RESOLUTION_12BIT:
        case ADC_RESOLUTION_10BIT:
            ADCx->CFG1 |= ADC_CFG1_MODE(res);
            break;
        default:
            result = HAL_PARAMETRER_ERROR;
            break;
    }
    return result;
}
static HAL_StatusTypeDef ADC_SetTrigger(ADCx_t ADCx, ADC_Trigger_t trig)
{
    HAL_StatusTypeDef result = HAL_OK;
    switch (trig)
    {
        case ADC_TRIGGER_SOFTWARE:
            ADCx->SC2 &= ~ADC_SC2_ADTRG_MASK;
            break;
        case ADC_TRIGGER_HARDWARE:
		    ADCx->SC2 |= ADC_SC2_ADTRG(trig);
            break;
        default:
            result = HAL_PARAMETRER_ERROR;
            break;
    }
    return result;
}

static HAL_StatusTypeDef ADC_SetRef(ADCx_t ADCx, ADC_RefSelection_t ref)
{
    HAL_StatusTypeDef result = HAL_OK;
    switch (ref)
    {
        case ADC_REF_EXTERNAL:
            ADCx->SC2 &= ~ADC_SC2_REFSEL_MASK;
            break;
        case ADC_REF_INTERNAL:
		    ADCx->SC2 |= ADC_SC2_REFSEL(ref);
            break;
        default:
            result = HAL_PARAMETRER_ERROR;
            break;
    }
    return result;
}

static HAL_StatusTypeDef ADC_SetMode(ADCx_t ADCx, ADC_Mode_t mode)
{
    HAL_StatusTypeDef result = HAL_OK;
    switch (mode)
    {
        case ADC_MODE_ONESHOT:
            ADCx->SC3 &= ~ADC_SC3_ADCO_MASK;
            break;
        case ADC_MODE_CONTINUOUS:
		    ADCx->SC3 |= ADC_SC3_ADCO(mode);
            break;
        default:
            result = HAL_PARAMETRER_ERROR;
            break;
    }
    return result;
}

static HAL_StatusTypeDef ADC_InterruptEnable(ADCx_t ADCx, ADC_Channel_t channel , ADC_Interrupt_t it)
{
    HAL_StatusTypeDef result = HAL_OK;
    if(ADC_CHANNLE_IS_AVAILABLE(channel))
    {
        switch (it)
        {
            case ADC_IT_DISABLE:
                ADCx->SC1[channel] &= ~ADC_SC1_AIEN_MASK;
                break;
            case ADC_IT_ENABLE:
                ADCx->SC1[channel] |= ADC_SC1_AIEN_MASK;
                break;
            default:
                result = HAL_PARAMETRER_ERROR;
                break;
        }
    }
    else
    {
        result = HAL_PARAMETRER_ERROR;
    }
    return result;
}

static HAL_StatusTypeDef ADC_Start(ADCx_t ADCx, ADC_Channel_t channel, ADC_Input_t input)
{
    HAL_StatusTypeDef result = HAL_OK;
    if(ADC_CHANNLE_IS_AVAILABLE(channel))
    { 
        ADCx->SC1[channel] |= ADC_SC1_ADCH_MASK;	//Disable channel (default)
        ADCx->SC1[channel] = (ADCx->SC1[channel] & ~ADC_SC1_ADCH_MASK) | ADC_SC1_ADCH(input);
    }
    else
    {
		result = HAL_PARAMETRER_ERROR;
    }
    return result;
}

static uint32_t ADC_Read(ADCx_t ADCx, ADC_Channel_t channel)
{
	uint32_t result = 0;
    if(ADC_CHANNLE_IS_AVAILABLE(channel))
    {
		result = ADCx->R[channel];
    }
	return result;
}

static HAL_StatusTypeDef ADC_IsConversionDone(ADCx_t ADCx, ADC_Channel_t channel, uint32_t timeout)
{
	uint32_t startTime = getTick();
    if(ADC_CHANNLE_IS_AVAILABLE(channel))
    { 
        while ((getTick() - startTime) < timeout)
		{
			if(ADCx->SC1[channel] & ADC_SC1_COCO_MASK)
			{
				return HAL_OK;
			}
		}
		return HAL_TIMEOUT;
    }
    else
    {
		return HAL_PARAMETRER_ERROR;
    }
}

ADC_Handler_t ADC_Handle =
{
	.EnableCLK                      = ADC_EnableCLK,
	.SetResolution                  = ADC_SetResolution,
	.SetResolution                  = ADC_SetResolution,
	.SetTrigger                     = ADC_SetTrigger,
	.SetMode                        = ADC_SetMode,
	.SetReference                   = ADC_SetRef,
	.InterruptEnable                = ADC_InterruptEnable,
	.Start                          = ADC_Start,
	.IsConversionDone               = ADC_IsConversionDone,
	.Read                           = ADC_Read
};
