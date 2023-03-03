/*
 * Console.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Kunal
 */

#ifndef CONSOLE_CONSOLE_H_
#define CONSOLE_CONSOLE_H_

#include "main.h"
#include "GPIO.h"


#define Printf_DMA	DMA2_Stream7_IRQHandler()


  void Console_Init(USART_TypeDef *port,int32_t baudrate);
  void printConsole(USART_TypeDef *port,char *msg, ...);
  void printConsole2(USART_TypeDef *port,char *msg);


#endif /* CONSOLE_CONSOLE_H_ */
