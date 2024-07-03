/***********************************************************************************
*
* @Name 				: Driver_GPIO.c
* @Author				: Tran Ba Than
* @Created date			: Jun 22 2024
*
************************************************************************************/

#include "Driver_GPIO.h"

ARM_DRIVER_GPIO Driver_GPIO =
{
	.Setup = ARM_GPIO_Setup,
	.SetDirection = ARM_GPIO_SetDirection,
	.SetOutputMode =ARM_GPIO_SetOutputMode,
	.SetPullResistor = ARM_GPIO_SetPullResistor,
	.SetEventTrigger = ARM_GPIO_SetEventTrigger,
	.SetOutput = ARM_GPIO_SetOutput,
    .ToggleOutput = ARM_GPIO_ToggleOutput,
    .GetInput = ARM_GPIO_GetInput
};

static ARM_GPIO_SignalEvent_t cb_signal_event = NULL;

int32_t ARM_GPIO_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
    int32_t result = ARM_DRIVER_OK;
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;
        // Enable clock port
        PCC->PCCn[PCC_PORTA_INDEX + port] |= PCC_PCCn_CGC_MASK;
        
        // MUX (GPIO)
        PORT_Type *portBase = (PORT_Type*)((uint32_t)PORTA_BASE + (uint32_t)(port * PORT_INSTANCE_OFFSET));
        portBase->PCR[pinNumber] &= ~PORT_PCR_MUX_MASK;
        portBase->PCR[pinNumber] |= PORT_PCR_MUX(1);

        // Register callback function
        if(cb_event != NULL)
        {
            cb_signal_event = cb_event;
        }
    }
    else
    {
        result = ARM_DRIVER_ERROR_PARAMETER;
    }
    return result;
}

int32_t ARM_GPIO_SetDirection(ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction)
{
    int32_t result = ARM_DRIVER_OK;
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;
        
        GPIO_Type *gpioBase = (GPIO_Type*)(PTA_BASE + (port * GPIO_INSTANCE_OFFSET));

        switch (direction)
        {
            case ARM_GPIO_OUTPUT:
                gpioBase->PDDR |= (1U << pinNumber); // Set pin as output
                break;
            case ARM_GPIO_INPUT:
                gpioBase->PDDR &= ~(1U << pinNumber); // Set pin as input
                break;
            default:
                result = ARM_DRIVER_ERROR_PARAMETER;
                break;
        }
    }
    else
    {
        result = ARM_DRIVER_ERROR_PARAMETER;
    }
    return result;
}

int32_t ARM_GPIO_SetOutputMode(ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode)
{
    int32_t result = ARM_DRIVER_OK;
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;

        PORT_Type *portBase = (PORT_Type*)((uint32_t)PORTA_BASE + (uint32_t)(port * PORT_INSTANCE_OFFSET));
        switch (mode)
        {
            case ARM_GPIO_PUSH_PULL:
                portBase->PCR[pinNumber] |= PORT_PCR_PE_MASK; // Enable Pull
                break;
            case ARM_GPIO_OPEN_DRAIN:
                portBase->PCR[pinNumber] &= ~PORT_PCR_PE_MASK; // Disable Pull
                break;
            default:
                result = ARM_DRIVER_ERROR_PARAMETER;
                break;
        }
    }
    else
    {
        result = ARM_DRIVER_ERROR_PARAMETER;
    }
    return result;
}

int32_t ARM_GPIO_SetPullResistor(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
{
    int32_t result = ARM_DRIVER_OK;
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;

        PORT_Type *portBase = (PORT_Type*)((uint32_t)PORTA_BASE + (uint32_t)(port * PORT_INSTANCE_OFFSET));
        
        switch (resistor)
        {
            case ARM_GPIO_PULL_UP:
                portBase->PCR[pinNumber] |= PORT_PCR_PS_MASK; // Pull Up
                break;
            case ARM_GPIO_PULL_DOWN:
                portBase->PCR[pinNumber] &= ~PORT_PCR_PS_MASK; // Pull Down
                break;
            case ARM_GPIO_PULL_NONE:
                break;
            default:
                result = ARM_DRIVER_ERROR_PARAMETER;
                break;
        }
    }
    else
    {
        result = ARM_DRIVER_ERROR_PARAMETER;
    }
    return result;
}

int32_t ARM_GPIO_SetEventTrigger(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
{
    int32_t result = ARM_DRIVER_OK;
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;

        PORT_Type *portBase = (PORT_Type*)((uint32_t)PORTA_BASE + (uint32_t)(port * PORT_INSTANCE_OFFSET));
        
        switch (trigger)
        {
            case ARM_GPIO_TRIGGER_RISING_EDGE:
                portBase->PCR[pinNumber] |= PORT_PCR_IRQC(9); // Rising edge
                break;
            case ARM_GPIO_TRIGGER_FALLING_EDGE:
                portBase->PCR[pinNumber] |= PORT_PCR_IRQC(10); // Falling edge
                break;
            case ARM_GPIO_TRIGGER_EITHER_EDGE:
                portBase->PCR[pinNumber] |= PORT_PCR_IRQC(11); // Either edges
                break;
            case ARM_GPIO_TRIGGER_NONE:
                portBase->PCR[pinNumber] &= ~(PORT_PCR_IRQC_MASK); // None
                break;
            default:
                result = ARM_DRIVER_ERROR_PARAMETER;
                break;
        }
    }
    else
    {
        result = ARM_DRIVER_ERROR_PARAMETER;
    }
    return result;
}

void ARM_GPIO_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val)
{
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;
        
        GPIO_Type *gpioBase = (GPIO_Type*)(PTA_BASE + (port * GPIO_INSTANCE_OFFSET));
        switch (val)
        {
            case 1U:
                gpioBase->PSOR = (1U << pinNumber); // Set pin
                break;
            case 0U:
                gpioBase->PCOR = (1U << pinNumber); // Clear pin
                break;
            default:
                break;
        }
    }
}

void ARM_GPIO_ToggleOutput(ARM_GPIO_Pin_t pin)
{
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;
        
        GPIO_Type *gpioBase = (GPIO_Type*)(PTA_BASE + (port * GPIO_INSTANCE_OFFSET));
        gpioBase->PTOR |=  (1 << pinNumber);
    }
}

uint32_t ARM_GPIO_GetInput(ARM_GPIO_Pin_t pin)
{
    uint32_t status = 0U;
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;

        GPIO_Type *gpioBase = (GPIO_Type*)(PTA_BASE + (port * GPIO_INSTANCE_OFFSET));
        status = (gpioBase->PDIR >> pinNumber) & 1U;
    }
    return status;
}

void GPIO_Check_IRQs(ARM_GPIO_Pin_t pin)
{
    if(PIN_IS_AVAILABLE(pin))
    {
        uint32_t port = (pin & ARM_GPIO_PORT_MASK) >> ARM_GPIO_PORT_SHIFT;
        uint32_t pinNumber = pin & ARM_GPIO_PIN_MASK;
        PORT_Type *portBase = (PORT_Type*)((uint32_t)PORTA_BASE + (uint32_t)(port * PORT_INSTANCE_OFFSET));
        GPIO_Type *gpioBase = (GPIO_Type*)(PTA_BASE + (port * GPIO_INSTANCE_OFFSET));

        if(((portBase->PCR[pinNumber] & PORT_PCR_ISF_MASK) >> PORT_PCR_ISF_SHIFT) != 0)
        {
            portBase->PCR[pinNumber] |= PORT_PCR_ISF_MASK;
            if(cb_signal_event != NULL)
            {
                if((gpioBase->PDOR >> pinNumber) & 1U)
                {
                    cb_signal_event(pin, ARM_GPIO_EVENT_RISING_EDGE);
                }
                else
                {
                    cb_signal_event(pin, ARM_GPIO_EVENT_FALLING_EDGE);
                }
            }
        }
    }
}

__weak void ARM_GPIO_SignalEvent(ARM_GPIO_Pin_t pin, uint32_t event)
{
    
}

