/*
 * SPI.c
 *
 *  Created on: 18-Jul-2021
 *      Author: Kunal
 */

#include "SPI.h"

void SPI_DeInit(SPI_Config SPI)
{
	if (SPI.SPI == SPI1)
	{
		RCC -> APB2ENR &= ~RCC_APB2ENR_SPI1EN;
	}
	if(SPI.SPI == SPI2)
	{
		RCC -> APB1ENR &= ~RCC_APB1ENR_SPI2EN;
	}
	if(SPI.SPI == SPI3)
	{
		RCC -> APB1ENR &= ~RCC_APB1ENR_SPI3EN;
	}
	SPI.SPI -> CR1 &= ~SPI_CR1_SPE;
}

void SPI_Init(SPI_Config SPI)
{



	if (SPI.SPI == SPI1)   //CK -> PA5 	//MISO -> PA6 	//MOSI -> PA7
	{
		RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN;

		if(SPI.pin_remap == 0)
		{
			GPIO_Pin_Setup(GPIOA, 5, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_CLK);
			GPIO_Pin_Setup(GPIOA, 6, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_MISO);
			GPIO_Pin_Setup(GPIOA, 7, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_MOSI);
		}
		else
		{
			GPIO_Pin_Setup(GPIOB, 3, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_CLK);
			GPIO_Pin_Setup(GPIOB, 4, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_MISO);
			GPIO_Pin_Setup(GPIOB, 5, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_MOSI);
		}
	}
	if(SPI.SPI == SPI2)	//CK -> PB10 	//MISO -> PB14 	//MOSI -> PB15
	{
		RCC -> APB1ENR |= RCC_APB1ENR_SPI2EN;

		if(SPI.pin_remap == 0)
		{
	        GPIO_Pin_Setup(GPIOB, 10, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 5);
			GPIO_Pin_Setup(GPIOC, 2, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 5);
			GPIO_Pin_Setup(GPIOC, 3, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 5);
		}
		else
		{
			GPIO_Pin_Setup(GPIOB, 13, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_CLK);
			GPIO_Pin_Setup(GPIOB, 14, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_MISO);
			GPIO_Pin_Setup(GPIOB, 15, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, SPI1_MOSI);
		}

	}
	if(SPI.SPI == SPI3)	//CK -> PB12 	//MISO -> PB4 	//MOSI -> PB5
	{
		RCC -> APB1ENR |= RCC_APB1ENR_SPI3EN;

		if(SPI.pin_remap == 0)
		{
	        GPIO_Pin_Setup(GPIOC, 10, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 6);
			GPIO_Pin_Setup(GPIOC, 11, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 6);
			GPIO_Pin_Setup(GPIOC, 12, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 6);
		}
		else
		{
			GPIO_Pin_Setup(GPIOB, 3, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 6);
			GPIO_Pin_Setup(GPIOB, 4, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 6);
			GPIO_Pin_Setup(GPIOB, 5, ALTERNATE_FUNCTION_OUTPUT_PUSHPULL, 6);
		}
	}






	SPI.SPI -> CR1 &= ~SPI_CR1_SPE;
	SPI.SPI -> CR1 |= SPI.phase << 0;
	SPI.SPI -> CR1 |= SPI.polarity << 1;
	SPI.SPI -> CR1 |= SPI.mode  << 2;
	SPI.SPI -> CR1 |= SPI.frequency << 3;
	SPI.SPI -> CR1 |= SPI.Frame << 11;
	SPI.SPI -> CR1 |= SPI.CRC_Enable << 13;
	SPI.SPI -> CR1 |= SPI.LSB_MSB << 7;
	SPI.SPI -> CR2 &= ~(SPI_CR2_FRF);

	SPI.SPI -> CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
//	SPI.SPI -> CR2 |= SPI_CR2_SSOE;


	if(SPI.mode == SPI_Master)
	{
		SPI.SPI -> CR1 |= SPI_CR1_MSTR ;
		switch (SPI.type)
		{
			case SPI_Full_Duplex_Master:
				SPI.SPI -> CR1 &= ~SPI_CR1_BIDIMODE;
				break;
			case SPI_Half_Duplex_Master_TX:
				SPI.SPI -> CR1 |= SPI_CR1_BIDIMODE;
				SPI.SPI -> CR1 |= SPI_CR1_BIDIOE;
				break;
			case SPI_Half_Duplex_Master_RX:
				SPI.SPI -> CR1 |= SPI_CR1_BIDIMODE;
				SPI.SPI -> CR1 &= ~SPI_CR1_BIDIOE;
				break;
			default:
				break;
		}

	}

	if(SPI.mode == SPI_Slave)
	{
	SPI.SPI -> CR1 &= ~SPI_CR1_MSTR;
	switch (SPI.type)
	 {
		case SPI_Full_Duplex_Slave:
			SPI.SPI -> CR1 &= ~SPI_CR1_BIDIMODE;
			break;
		case SPI_Half_Duplex_Slave_TX:
			SPI.SPI -> CR1 |= SPI_CR1_BIDIMODE;
			SPI.SPI -> CR1 |= SPI_CR1_BIDIOE;
			break;
		case SPI_Half_Duplex_Slave_RX:
			SPI.SPI -> CR1 |= SPI_CR1_BIDIMODE;
			SPI.SPI -> CR1 &= ~SPI_CR1_BIDIOE;
			break;
		default:
			break;
	 }
	}


	GPIO_Pin_Setup(SPI.nCSS_Port, SPI.nCSS_pin, GENERAL_PURPOSE_OUTPUT_PUSHPULL_PULLUP, NONE);

	GPIO_Pin_High(SPI.nCSS_Port, SPI.nCSS_pin);

	SPI.SPI -> CR1 |= SPI_CR1_SPE;

}

void SPI_Enable_DMA(SPI_Config SPI)
{
	SPI.SPI -> CR2 |= (SPI.DMA_TX << SPI_CR2_TXDMAEN_Pos) | (SPI.DMA_RX << SPI_CR2_RXDMAEN_Pos);
}
void SPI_Disable_DMA(SPI_Config SPI)
{
	SPI.SPI -> CR2 &= ~((SPI.DMA_TX << SPI_CR2_TXDMAEN_Pos) | (SPI.DMA_RX << SPI_CR2_RXDMAEN_Pos));
}


void SPI_CSS_Low(SPI_Config SPI)
{
	GPIO_Pin_Low(SPI.nCSS_Port, SPI.nCSS_pin);
}


void SPI_CSS_High(SPI_Config SPI)
{
	GPIO_Pin_High(SPI.nCSS_Port, SPI.nCSS_pin);
}


//
int SPI_TRX_Data(SPI_Config SPI, int data)
{
	int temp = 0;
	while (!(SPI.SPI->SR & SPI_SR_TXE));
	SPI.SPI -> DR = data;
	while (!(SPI.SPI->SR & SPI_SR_RXNE));
	temp = SPI.SPI -> DR;
	while (!(SPI.SPI->SR & SPI_SR_TXE));
	while (SPI.SPI->SR & SPI_SR_BSY);
	return temp;
}




DMA_TypeDef *SPI_DMA_Check_Base(SPI_Config SPI)
{
	DMA_TypeDef *dummmy_typedef;
	if(SPI.SPI == SPI1)
	{
		dummmy_typedef = DMA2;
	}else if (SPI.SPI == SPI2)
	{
		dummmy_typedef = DMA1;
	}else if (SPI.SPI == SPI3)
	{
		dummmy_typedef = DMA1;
	}

	return dummmy_typedef;
}

//
//DMA_Stream_TypeDef *SPI_DMA_Check_Stream_TX(SPI_Config SPI)
//{
//	DMA_Stream_TypeDef *dummy_stream;
//
////***********************************************************************************************************
//	if(SPI.SPI == SPI1)
//	{
//		if(SPI.DMA_TX == 1)
//		{
//			if((DMA2_STREAM3_CHANNEL3_FLAG == 0) && (DMA2_STREAM5_CHANNEL3_FLAG == 0))
//			{
//				dummy_stream = DMA2_Stream3;
//				DMA2_STREAM3_CHANNEL3_FLAG = 1;
//			}
//			if((DMA2_STREAM3_CHANNEL3_FLAG == 0) && (DMA2_STREAM5_CHANNEL3_FLAG == 1))
//			{
//				dummy_stream = DMA2_Stream5;
//				DMA2_STREAM5_CHANNEL3_FLAG = 1;
//			}
//			if((DMA2_STREAM3_CHANNEL3_FLAG == 1) && (DMA2_STREAM5_CHANNEL3_FLAG == 1))
//			{
////				dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable Stream is being used by some other peripheral \r\n");
//#endif
//			}
//		}
//		else
//		{
////			dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
//
////***********************************************************************************************************
//	else if(SPI.SPI == SPI2)
//	{
//		if(SPI.DMA_TX == 1)
//		{
//			if((DMA1_STREAM4_CHANNEL0_FLAG == 0))
//			{
//				dummy_stream = DMA1_Stream4;
//				DMA1_STREAM4_CHANNEL0_FLAG = 1;
//			}
//			else
//			{
////				dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable Stream is being used by some other peripheral \r\n");
//#endif
//			}
//		}
//		else
//		{
////			dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
////***********************************************************************************************************
//	if(SPI.SPI == SPI3)
//	{
//		if(SPI.DMA_TX == 1)
//		{
//			if((DMA1_STREAM5_CHANNEL0_FLAG == 0) && (DMA1_STREAM7_CHANNEL0_FLAG == 0))
//			{
//				dummy_stream = DMA2_Stream3;
//				DMA1_STREAM5_CHANNEL0_FLAG = 1;
//			}
//			if((DMA1_STREAM5_CHANNEL0_FLAG == 0) && (DMA1_STREAM7_CHANNEL0_FLAG == 1))
//			{
//				dummy_stream = DMA2_Stream5;
//				DMA1_STREAM7_CHANNEL0_FLAG = 1;
//			}
//			if((DMA1_STREAM5_CHANNEL0_FLAG == 1) && (DMA1_STREAM7_CHANNEL0_FLAG == 1))
//			{
////				dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable Stream is being used by some other peripheral \r\n");
//#endif
//			}
//		}
//		else
//		{
////			dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
//
//
//	return dummy_stream;
//
//}
//
//DMA_Stream_TypeDef *SPI_DMA_Check_Stream_RX(SPI_Config SPI)
//{
//	DMA_Stream_TypeDef *dummy_stream;
//
////***********************************************************************************************************
//	if(SPI.SPI == SPI1)
//	{
//		if(SPI.DMA_RX == 1)
//		{
//			if((DMA2_STREAM0_CHANNEL3_FLAG == 0) && (DMA2_STREAM2_CHANNEL3_FLAG == 0))
//			{
//				dummy_stream = DMA2_Stream0;
//				DMA2_STREAM3_CHANNEL3_FLAG = 1;
//			}
//			if((DMA2_STREAM0_CHANNEL3_FLAG == 0) && (DMA2_STREAM2_CHANNEL3_FLAG == 1))
//			{
//				dummy_stream = DMA2_Stream3;
//				DMA2_STREAM2_CHANNEL3_FLAG = 1;
//			}
//			if((DMA2_STREAM0_CHANNEL3_FLAG == 1) && (DMA2_STREAM2_CHANNEL3_FLAG == 1))
//			{
////				dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable Stream is being used by some other peripheral \r\n");
//#endif
//			}
//		}
//		else
//		{
////			dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
//
////***********************************************************************************************************
//	else if(SPI.SPI == SPI2)
//	{
//		if(SPI.DMA_TX == 1)
//		{
//			if((DMA1_STREAM3_CHANNEL0_FLAG == 0))
//			{
//				dummy_stream = DMA1_Stream4;
//				DMA1_STREAM3_CHANNEL0_FLAG = 1;
//			}
//			else
//			{
////				dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable Stream is being used by some other peripheral \r\n");
//#endif
//			}
//		}
//		else
//		{
////			dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
////***********************************************************************************************************
//	if(SPI.SPI == SPI3)
//	{
//		if(SPI.DMA_TX == 1)
//		{
//			if((DMA1_STREAM0_CHANNEL0_FLAG == 0) && (DMA1_STREAM7_CHANNEL0_FLAG == 0))
//			{
//				dummy_stream = DMA1_Stream0;
//				DMA1_STREAM0_CHANNEL0_FLAG = 1;
//			}
//			if((DMA1_STREAM0_CHANNEL0_FLAG == 0) && (DMA1_STREAM2_CHANNEL0_FLAG == 1))
//			{
//				dummy_stream = DMA1_Stream2;
//				DMA1_STREAM2_CHANNEL0_FLAG = 1;
//			}
//			if((DMA1_STREAM0_CHANNEL0_FLAG == 1) && (DMA1_STREAM2_CHANNEL0_FLAG == 1))
//			{
////				dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable Stream is being used by some other peripheral \r\n");
//#endif
//			}
//		}
//		else
//		{
////			dummy_stream = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
//
//
//	return dummy_stream;
//
//}
//
//
//int SPI_DMA_Check_Channel_TX(SPI_Config SPI)
//{
//	int dummy_channel = 0;
//	if(SPI.SPI == SPI1)
//	{
//		if(SPI.DMA_TX == 1)
//		{
//			dummy_channel = 3;
//#if SPI_Debug_Flag
//			printConsole(USART1, "SPI1 TX DMA Channel = 3  \r\n");
//#endif
//		}
//		else
//		{
//			dummy_channel = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
//	else if(SPI.SPI == SPI2)
//	{
//		if(SPI.DMA_TX == 1)
//		{
//			dummy_channel = 0;
//#if SPI_Debug_Flag
//			printConsole(USART1, "SPI2 TX DMA Channel = 0  \r\n");
//#endif
//		}
//		else
//		{
//			dummy_channel = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
//	else if(SPI.SPI == SPI3)
//	{
//		if(SPI.DMA_TX == 1)
//		{
//			dummy_channel = 0;
//#if SPI_Debug_Flag
//			printConsole(USART1, "SPI3 TX DMA Channel = 3  \r\n");
//#endif
//		}
//		else
//		{
//			dummy_channel = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_TX before calling this function \r\n");
//#endif
//		}
//	}
//
//	return dummy_channel;
//}
//
//int SPI_DMA_Check_Channel_RX(SPI_Config SPI)
//{
//	int dummy_channel = 0;
//	if(SPI.SPI == SPI1)
//	{
//		if(SPI.DMA_RX == 1)
//		{
//			dummy_channel = 3;
//#if SPI_Debug_Flag
//			printConsole(USART1, "SPI1 RX DMA Channel = 3  \r\n");
//#endif
//		}
//		else
//		{
//			dummy_channel = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_RX before calling this function \r\n");
//#endif
//		}
//	}
//	else if(SPI.SPI == SPI2)
//	{
//		if(SPI.DMA_RX == 1)
//		{
//			dummy_channel = 0;
//#if SPI_Debug_Flag
//			printConsole(USART1, "SPI2 RX DMA Channel = 0  \r\n");
//#endif
//		}
//		else
//		{
//			dummy_channel = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_RX before calling this function \r\n");
//#endif
//		}
//	}
//	else if(SPI.SPI == SPI3)
//	{
//		if(SPI.DMA_RX == 1)
//		{
//			dummy_channel = 0;
//#if SPI_Debug_Flag
//			printConsole(USART1, "SPI3 RX DMA Channel = 3  \r\n");
//#endif
//		}
//		else
//		{
//			dummy_channel = -1;
//#if SPI_Debug_Flag
//			printConsole(USART1, "Enable SPI.DMA_RX before calling this function \r\n");
//#endif
//		}
//	}
//
//	return dummy_channel;
//}
