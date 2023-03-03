/*
 * Console.c
 *
 *  Created on: Jun 14, 2022
 *      Author: Kunal
 */



#include "Console.h"
//#include "DMA/DMA.h"


int callback = 0;



 void Console_Init(USART_TypeDef *port,int32_t baudrate)
{
	if(port == USART1)
	{
		RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
		RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN;
		port -> CR3 |= USART_CR3_DMAT;
		DMA2_Stream7 -> CR |= 4 << 25  | 0 << 16 | 0 << 13 | 0 << 11 | 1 << 10 | 0 << 8 | 1 << 6 |
				1 << 4 | 3 << 16;
		GPIO_Pin_Setup(GPIOA, 9, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, USART1_TX);
		GPIO_Pin_Setup(GPIOA, 10, ALTERNATE_FUNCTION_OUTPUT_OPENDRAIN, USART1_RX);

		port ->BRR |= (int)(84000000 / (16 * baudrate)) << 4;
		port -> CR1 |= USART_CR1_TE | USART_CR1_RE;
		port -> CR1 |= USART_CR1_UE;


	}
	else if(port == USART6)
	{
		RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;
		GPIO_Pin_Setup(GPIOC, 6, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, USART6_TX);
		GPIO_Pin_Setup(GPIOC, 7, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, USART6_RX);
		port ->CR1 |= USART_CR1_UE;
		port->BRR = (int)(84000000 / (16 * baudrate)) << 4;
		port ->CR1 |= USART_CR1_TE ;
		port ->CR1 |=  USART_CR1_RE  ;
	}
	else if(port == USART2)
	{
		RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
		GPIO_Pin_Setup(GPIOA, 2, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, USART2_TX);
		GPIO_Pin_Setup(GPIOA, 3, ALTERNATE_FUNCTION_OUTPUT_OPENDRAIN, USART2_RX);
		port ->CR1 |= USART_CR1_UE;
		port->BRR = (int)(84000000 / ((16 * baudrate)*2)) << 4;
		port ->CR1 |= USART_CR1_TE ;
		port ->CR1 |=  USART_CR1_RE  ;
	}
	else if(port == USART3)
	{
		RCC -> APB1ENR |= RCC_APB1ENR_USART3EN;
		GPIO_Pin_Setup(GPIOB, 10, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, USART3_TX);
		GPIO_Pin_Setup(GPIOB, 11, ALTERNATE_FUNCTION_OUTPUT_OPENDRAIN, USART3_RX);
		port ->CR1 |= USART_CR1_UE;
		port->BRR = (int)(84000000 / ((16 * baudrate)*2)) << 4;
		port ->CR1 |= USART_CR1_TE ;
		port ->CR1 |=  USART_CR1_RE  ;
	}
	else if(port == UART4)
	{
		RCC -> APB1ENR |= RCC_APB1ENR_UART4EN;
		GPIO_Pin_Setup(GPIOA, 0, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, USART4_TX);
		GPIO_Pin_Setup(GPIOA, 1, ALTERNATE_FUNCTION_OUTPUT_OPENDRAIN, USART4_RX);
		port ->CR1 |= USART_CR1_UE;
		port->BRR = (int)(84000000 / ((16 * baudrate)*2)) << 4;
		port ->CR1 |= USART_CR1_TE ;
		port ->CR1 |=  USART_CR1_RE  ;
	}
	else if(port == UART5)
	{
		RCC -> APB1ENR |= RCC_APB1ENR_UART5EN;
		GPIO_Pin_Setup(GPIOC, 12, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, USART5_TX);
		GPIO_Pin_Setup(GPIOD, 2, ALTERNATE_FUNCTION_OUTPUT_OPENDRAIN, USART5_RX);
		port ->CR1 |= USART_CR1_UE;
		port->BRR = (int)(84000000 / ((16 * baudrate)*2)) << 4;
		port ->CR1 |= USART_CR1_TE ;
		port ->CR1 |=  USART_CR1_RE  ;
	}



}


 void printConsole(USART_TypeDef *port,char *msg, ...)
{

char buff[1000];
//	#ifdef DEBUG_UART
	va_list args;
	va_start(args, msg);
	vsprintf(buff, msg, args);
	DMA2_Stream7 -> PAR = (uint32_t)&port -> DR;
	DMA2_Stream7 -> M0AR = (uint32_t)&buff[0];
	DMA2_Stream7 -> NDTR = (uint16_t)strlen(buff);
	DMA2_Stream7 -> CR |= DMA_SxCR_EN;
	while((DMA2->HISR & DMA_HIFCR_CTCIF7) == 0){}
	DMA2 ->HIFCR |= DMA_HIFCR_CTCIF7;

//	for(int i = 0; i<= strlen(buff)-1; i++)
//	{
//		port -> DR = buff[i];
////		port -> CR1 |= USART_CR1_SBK;
//		while (!(port->SR & USART_SR_TXE));
//	}

//	#endif
}

 void printConsole2(USART_TypeDef *port,char *msg)
{

char buff[10000];
	uint16_t len = sprintf(buff,msg);
	DMA2_Stream7 -> PAR = (uint32_t)&port -> DR;
	DMA2_Stream7 -> M0AR = (uint32_t)&buff[0];
	DMA2_Stream7 -> NDTR = (uint16_t)len;
	DMA2_Stream7 -> CR |= DMA_SxCR_EN;
	while((DMA2->HISR & DMA_HIFCR_CTCIF7) == 0){}
	DMA2 ->HIFCR |= DMA_HIFCR_CTCIF7;
}


  char * Console_Return(char *s)
{
	int ch;
	char *p = s;

	while( (ch = USART1 -> DR) != '\n' && ch != EOF)
	{
		*s = (char)ch;
		s++;
	}
	*s = '\0';
	return p;
}

