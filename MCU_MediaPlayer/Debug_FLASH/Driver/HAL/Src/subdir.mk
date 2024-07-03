################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/HAL/Src/ADC.c \
../Driver/HAL/Src/Driver_UART.c \
../Driver/HAL/Src/Timer.c 

OBJS += \
./Driver/HAL/Src/ADC.o \
./Driver/HAL/Src/Driver_UART.o \
./Driver/HAL/Src/Timer.o 

C_DEPS += \
./Driver/HAL/Src/ADC.d \
./Driver/HAL/Src/Driver_UART.d \
./Driver/HAL/Src/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/HAL/Src/%.o: ../Driver/HAL/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Driver/HAL/Src/ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


