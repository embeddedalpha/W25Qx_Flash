/*
 * DMA.h
 *
 *  Created on: Nov 23, 2022
 *      Author: Kunal
 */

#ifndef DMA_H_
#define DMA_H_

#include "main.h"
#include "DMA_flags.h"



typedef struct DMA_Interrupt_Typedef{

	bool Half_Transfer;
	bool Transfer_Complete;
	bool Transfer_Error;
	bool FIFO_Overrun_Underrun;
	bool _interrupt;

}DMA_Interrupt_Typedef;


typedef struct DMA_Init_Typedef{

	DMA_TypeDef *DMA_Port;
	DMA_Stream_TypeDef *Stream;
	int channel;
	bool circular_buff_enable;
	DMA_Interrupt_Typedef *interrupt;
	bool Half_Transfer_interrupt;
	bool Transfer_Complete_interrupt;
	bool Transfer_Error_interrupt;
	bool FIFO_Overrun_Underrun_interrupt;
	bool Direct_Mode_Error_interrupt;
	volatile uint32_t Desitnation_Address;
	volatile uint32_t Source_Address;
	int data_length;
	uint8_t direction;
	uint8_t priority;
	uint8_t memory_data_size;
	uint8_t periphera_data_size;

}DMA_Init_Typedef;


enum data_size{
	Byte = 0,
	Half_Word,
	Word,
};

int DMA_Init(DMA_Init_Typedef *DMA_Config);

int DMA_Trigger(DMA_Init_Typedef *DMA_Config);



#endif /* DMA_H_ */
