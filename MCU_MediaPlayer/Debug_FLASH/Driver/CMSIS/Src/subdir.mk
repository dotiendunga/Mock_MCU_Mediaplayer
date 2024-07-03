################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/CMSIS/Src/Core_Interrupt.c \
../Driver/CMSIS/Src/Core_Systick.c \
../Driver/CMSIS/Src/Driver_GPIO.c 

OBJS += \
./Driver/CMSIS/Src/Core_Interrupt.o \
./Driver/CMSIS/Src/Core_Systick.o \
./Driver/CMSIS/Src/Driver_GPIO.o 

C_DEPS += \
./Driver/CMSIS/Src/Core_Interrupt.d \
./Driver/CMSIS/Src/Core_Systick.d \
./Driver/CMSIS/Src/Driver_GPIO.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/CMSIS/Src/%.o: ../Driver/CMSIS/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Driver/CMSIS/Src/Core_Interrupt.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


