################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPI/SPI.c 

OBJS += \
./SPI/SPI.o 

C_DEPS += \
./SPI/SPI.d 


# Each subdirectory must supply rules for building sources it contributes
SPI/%.o SPI/%.su: ../SPI/%.c SPI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VETx -c -I../Inc -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/Console" -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/GPIO" -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/SPI" -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/DMA" -I"D:/Files/STM32F407_Workspace/W25Qx_Flash/Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SPI

clean-SPI:
	-$(RM) ./SPI/SPI.d ./SPI/SPI.o ./SPI/SPI.su

.PHONY: clean-SPI

