################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/W25Qx_Driver.c 

OBJS += \
./Driver/W25Qx_Driver.o 

C_DEPS += \
./Driver/W25Qx_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/%.o Driver/%.su: ../Driver/%.c Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VETx -c -I../Inc -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/Console" -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/GPIO" -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/SPI" -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/DMA" -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Driver

clean-Driver:
	-$(RM) ./Driver/W25Qx_Driver.d ./Driver/W25Qx_Driver.o ./Driver/W25Qx_Driver.su

.PHONY: clean-Driver

