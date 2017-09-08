################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TM/tm_stm32_ahrs_imu.c \
../TM/tm_stm32_buffer.c \
../TM/tm_stm32_delay.c \
../TM/tm_stm32_disco.c \
../TM/tm_stm32_gpio.c \
../TM/tm_stm32_i2c.c \
../TM/tm_stm32_mpu6050.c \
../TM/tm_stm32_rcc.c \
../TM/tm_stm32_usart.c 

OBJS += \
./TM/tm_stm32_ahrs_imu.o \
./TM/tm_stm32_buffer.o \
./TM/tm_stm32_delay.o \
./TM/tm_stm32_disco.o \
./TM/tm_stm32_gpio.o \
./TM/tm_stm32_i2c.o \
./TM/tm_stm32_mpu6050.o \
./TM/tm_stm32_rcc.o \
./TM/tm_stm32_usart.o 

C_DEPS += \
./TM/tm_stm32_ahrs_imu.d \
./TM/tm_stm32_buffer.d \
./TM/tm_stm32_delay.d \
./TM/tm_stm32_disco.d \
./TM/tm_stm32_gpio.d \
./TM/tm_stm32_i2c.d \
./TM/tm_stm32_mpu6050.d \
./TM/tm_stm32_rcc.d \
./TM/tm_stm32_usart.d 


# Each subdirectory must supply rules for building sources it contributes
TM/%.o: ../TM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32F4XX -I"/media/kuba/Windows7/Users/Lenovo/VM_shared/Studia/INZ/Sources/firmware-current/firmware-stm32/Inc" -I"/media/kuba/Windows7/Users/Lenovo/VM_shared/Studia/INZ/Sources/firmware-current/firmware-stm32/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/media/kuba/Windows7/Users/Lenovo/VM_shared/Studia/INZ/Sources/firmware-current/firmware-stm32/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/media/kuba/Windows7/Users/Lenovo/VM_shared/Studia/INZ/Sources/firmware-current/firmware-stm32/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/media/kuba/Windows7/Users/Lenovo/VM_shared/Studia/INZ/Sources/firmware-current/firmware-stm32/Drivers/CMSIS/Include" -I"/media/kuba/Windows7/Users/Lenovo/VM_shared/Studia/INZ/Sources/firmware-current/firmware-stm32/SW4STM32/firmware-stm32/TM"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


