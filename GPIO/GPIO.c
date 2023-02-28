/*
 * GPIO.c
 *
 *  Created on: Nov 2, 2021
 *      Author: Kunal
 */


#include "GPIO.h"


int GPIO_Pin_Setup(GPIO_TypeDef *PORT, uint8_t pin, uint8_t function, uint8_t alternate_function)
{

	if(PORT == GPIOA)
	{
		RCC -> AHB1ENR   |= RCC_AHB1ENR_GPIOAEN;
	} else 	if(PORT == GPIOB)
	{
		RCC -> AHB1ENR   |= RCC_AHB1ENR_GPIOBEN;
	}else 	if(PORT == GPIOC)
	{
		RCC -> AHB1ENR   |= RCC_AHB1ENR_GPIOCEN;
	}else 	if(PORT == GPIOD)
	{
		RCC -> AHB1ENR   |= RCC_AHB1ENR_GPIODEN;
	}else 	if(PORT == GPIOE)
	{
		RCC -> AHB1ENR   |= RCC_AHB1ENR_GPIOEEN;
	}else 	if(PORT == GPIOH)
	{
		RCC -> AHB1ENR   |= RCC_AHB1ENR_GPIOHEN;
	}else
	{
		return -1;
	}

//	PORT -> MODER   &= ~((0xFF) << (2 * pin));
//	PORT -> OTYPER  &= ~((0xFF) << (1 * pin));
//	PORT -> OSPEEDR &= ~((0xFF) << (2 * pin));
//	PORT -> PUPDR   &= ~((0xFF) << (2 * pin));


	PORT -> MODER	&= ~(0x3 << (2 * pin));
	PORT -> MODER   |= ((0xC0  & function)>>6) << (2 * pin);

	PORT -> OTYPER	&= ~(0x1 << (1 * pin));
	PORT -> OTYPER  |= ((0x30  & function)>>4) << (1 * pin);

	PORT -> OSPEEDR	&= ~(0x3 << (2 * pin));
	PORT -> OSPEEDR |= ((0x0C  & function)>>2) << (2 * pin);

	PORT -> PUPDR	&= ~(0x03 << (2 * pin));
	PORT -> PUPDR   |= ((0x03  & function)>>0) << (2 * pin);

	if(pin < 8)PORT -> AFR[0] |= ( alternate_function << (4 * (pin)));
	else	   PORT -> AFR[1] |= ( alternate_function << (4 * (pin - 8)));

	if (alternate_function == NONE) {}

	return 0;
}


void GPIO_Interrupt_Setup(int pin, int edge_select)
{
	EXTI ->IMR |= 1 << pin;
	switch (edge_select) {
		case 0:
			EXTI ->RTSR |= 1 << pin;
			break;
		case 1:
			EXTI ->FTSR |= 1 << pin;
			break;
		case 2:
			EXTI ->RTSR |= 1 << pin;
			EXTI ->FTSR |= 1 << pin;
			break;
	}

	if(pin == 0)
	{
		NVIC_SetPriority(EXTI0_IRQn,0);
		NVIC_EnableIRQ(EXTI0_IRQn);
	}
	else if(pin == 1)
	{
		NVIC_SetPriority(EXTI1_IRQn,0);
		NVIC_EnableIRQ(EXTI1_IRQn);
	}
	else if(pin == 2)
	{
		NVIC_SetPriority(EXTI2_IRQn,0);
		NVIC_EnableIRQ(EXTI2_IRQn);
	}
	else if(pin == 3)
	{
		NVIC_SetPriority(EXTI3_IRQn,0);
		NVIC_EnableIRQ(EXTI3_IRQn);
	}
	else if(pin == 4)
	{
		NVIC_SetPriority(EXTI4_IRQn,0);
		NVIC_EnableIRQ(EXTI4_IRQn);
	}
	else if(pin >= 5 || pin <= 9)
	{
		NVIC_SetPriority(EXTI9_5_IRQn,0);
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	}
	else if(pin >= 10 || pin <= 15)
	{
		NVIC_SetPriority(EXTI15_10_IRQn,0);
		NVIC_EnableIRQ(EXTI15_10_IRQn);
	}



}

void GPIO_Pin_High(GPIO_TypeDef *Port, int pin)
{
	Port -> ODR = 1 << pin;
}


void GPIO_Pin_Low(GPIO_TypeDef *Port, int pin)
{
	Port -> ODR &= ~(1 << pin);
}
