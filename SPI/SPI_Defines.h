/*
 * SPI_Defines.h
 *
 *  Created on: Mar 12, 2023
 *      Author: Kunal
 */

#ifndef SPI_DEFINES_H_
#define SPI_DEFINES_H_

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

/**************************************************************************************/
#define SPI1_DMA_Port			DMA2
#define SPI1_DMA_TX_STREAM3		DMA2_Stream3
#define SPI1_DMA_TX_STREAM5		DMA2_Stream5
#define SPI1_DMA_RX_STREAM0		DMA2_Stream0
#define SPI1_DMA_RX_STREAM2		DMA2_Stream2
#define SPI1_DMA_TX_CHANNEL		3
#define SPI1_DMA_RX_CHANNEL		3
/**************************************************************************************/
#define SPI2_DMA_Port			DMA1
#define SPI2_DMA_TX_STREAM4		DMA1_Stream4
#define SPI2_DMA_RX_STREAM3		DMA1_Stream3
#define SPI2_DMA_TX_CHANNEL		0
#define SPI2_DMA_RX_CHANNEL		0
/**************************************************************************************/
#define SPI3_DMA_Port			DMA1
#define SPI3_DMA_TX_STREAM5		DMA1_Stream5
#define SPI3_DMA_TX_STREAM7		DMA1_Stream7
#define SPI3_DMA_RX_STREAM0		DMA1_Stream0
#define SPI3_DMA_RX_STREAM2		DMA1_Stream2
#define SPI3_DMA_TX_CHANNEL		0
#define SPI3_DMA_RX_CHANNEL		0
/**************************************************************************************/










#endif /* SPI_DEFINES_H_ */
