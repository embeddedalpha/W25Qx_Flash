/*
 * SPI.h
 *
 *  Created on: 18-Jul-2021
 *      Author: Kunal
 */

#ifndef SPI_SPI_H_
#define SPI_SPI_H_

#include "main.h"
#include "GPIO.h"
#include "DMA.h"




#define SPI_Full_Duplex_Master    0
#define SPI_Half_Duplex_Master_TX 1
#define SPI_Half_Duplex_Master_RX 2

#define SPI_Full_Duplex_Slave     3
#define SPI_Half_Duplex_Slave_TX  4
#define SPI_Half_Duplex_Slave_RX  5

#define SPI_Frame_8Bit 0
#define SPI_Frame_16Bit 1

#define SPI_Master 1
#define SPI_Slave 0




typedef struct SPI_Config
{
	SPI_TypeDef *SPI;
	bool pin_remap;
	uint8_t type;
	uint8_t mode;
	int frequency;
	uint8_t phase;
	uint8_t polarity;
	uint8_t LSB_MSB;
	uint8_t CRC_Enable;
	bool DMA_TX;
	bool DMA_RX;
	uint8_t Frame;
	GPIO_TypeDef *nCSS_Port;
	uint8_t nCSS_pin;
	DMA_Init_Typedef DMA_Struct;
}SPI_Config;


void SPI_DeInit(SPI_Config SPI);
void SPI_Init(SPI_Config SPI);
void SPI_CSS_High(SPI_Config SPI);
void SPI_CSS_Low(SPI_Config SPI);
int SPI_TRX_Data(SPI_Config SPI, int data);
int SPI_Receive_Data(SPI_Config SPI);

DMA_TypeDef *SPI_DMA_Check_Base(SPI_Config SPI);
DMA_Stream_TypeDef *SPI_DMA_Check_Stream_TX(SPI_Config SPI);
DMA_Stream_TypeDef *SPI_DMA_Check_Stream_RX(SPI_Config SPI);
int SPI_DMA_Check_Channel_TX(SPI_Config SPI);
int SPI_DMA_Check_Channel_RX(SPI_Config SPI);


#endif /* SPI_SPI_H_ */
