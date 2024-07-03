/***********************************************************************************
*
* @Name 				        : Driver_GPIO.h
* @Author				        : Tran Ba Than
* @Created date			    : Jun 22 2024
*
************************************************************************************/

#ifndef INC_DRIVER_GPIO_H_
#define INC_DRIVER_GPIO_H_

#include "CMSIS_Common.h"

// Pin Number
#define GPIO_MAX_PINS                    160U
#define PIN_IS_AVAILABLE(n)              ((n) < GPIO_MAX_PINS)

// MASK
#define ARM_GPIO_PIN_MASK                 0x1F
#define ARM_GPIO_PORT_MASK                0x1E0
#define ARM_GPIO_PORT_SHIFT               5

//GPIO Events
#define ARM_GPIO_EVENT_RISING_EDGE      (1UL << 0)
#define ARM_GPIO_EVENT_FALLING_EDGE     (1UL << 1)
#define ARM_GPIO_EVENT_EITHER_EDGE      (1UL << 2)


typedef uint32_t ARM_GPIO_Pin_t;

typedef void(*ARM_GPIO_SignalEvent_t) (ARM_GPIO_Pin_t pin, uint32_t event);

// GPIO Direction
typedef enum
{
  ARM_GPIO_INPUT ,
  ARM_GPIO_OUTPUT
} ARM_GPIO_DIRECTION;

typedef enum
{
  ARM_GPIO_PUSH_PULL ,
  ARM_GPIO_OPEN_DRAIN
} ARM_GPIO_OUTPUT_MODE;

typedef enum
{
  ARM_GPIO_PULL_NONE ,
  ARM_GPIO_PULL_UP ,
  ARM_GPIO_PULL_DOWN
} ARM_GPIO_PULL_RESISTOR;
//GPIO Event Trigger

typedef enum
{
  ARM_GPIO_TRIGGER_NONE ,
  ARM_GPIO_TRIGGER_RISING_EDGE ,
  ARM_GPIO_TRIGGER_FALLING_EDGE ,
  ARM_GPIO_TRIGGER_EITHER_EDGE
} ARM_GPIO_EVENT_TRIGGER;


//Functions
//Setup GPIO Interface.
int32_t ARM_GPIO_Setup(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);
 
//Set GPIO Direction.
int32_t ARM_GPIO_SetDirection(ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction);

//Set GPIO Output Mode.
int32_t ARM_GPIO_SetOutputMode(ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode);

//Set GPIO Pull Resistor.
int32_t ARM_GPIO_SetPullResistor(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor);

//Set GPIO Event Trigger.
int32_t ARM_GPIO_SetEventTrigger(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger);

//Set GPIO Output Level.
void ARM_GPIO_SetOutput(ARM_GPIO_Pin_t pin, uint32_t val);

//Toggle GPIO Output Level.
void ARM_GPIO_ToggleOutput(ARM_GPIO_Pin_t pin);

//Get GPIO Input Level.
uint32_t ARM_GPIO_GetInput(ARM_GPIO_Pin_t pin);

//Signal GPIO Events.
void ARM_GPIO_SignalEvent(ARM_GPIO_Pin_t pin, uint32_t event);

//Function Struct
typedef struct
{
    int32_t (*Setup)(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);
    int32_t (*SetDirection)(ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction);
    int32_t (*SetOutputMode)(ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode);
    int32_t (*SetPullResistor)(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor);
    int32_t (*SetEventTrigger)(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger);
    void (*SetOutput)(ARM_GPIO_Pin_t pin, uint32_t val);
    void (*ToggleOutput)(ARM_GPIO_Pin_t pin);
    uint32_t (*GetInput)(ARM_GPIO_Pin_t pin);
} ARM_DRIVER_GPIO;

void GPIO_Check_IRQs(ARM_GPIO_Pin_t pin);

#endif /* INC_DRIVER_GPIO_H_ */
