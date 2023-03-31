/*
 * DMA_Directives.h
 *
 *  Created on: Mar 8, 2023
 *      Author: Kunal
 */

#ifndef DMA_DIRECTIVES_H_
#define DMA_DIRECTIVES_H_


#define vacant		0
#define occupied	1

typedef struct DMA_Stream{
	bool Channel_0;
	bool Channel_1;
	bool Channel_2;
	bool Channel_3;
	bool Channel_4;
	bool Channel_5;
	bool Channel_6;
	bool Channel_7;
}DMAx_Stream;

typedef struct DMA_Flag_Instance{
	DMAx_Stream *Stream_0_Flag;
	DMAx_Stream *Stream_1_Flag;
	DMAx_Stream *Stream_2_Flag;
	DMAx_Stream *Stream_3_Flag;
	DMAx_Stream *Stream_4_Flag;
	DMAx_Stream *Stream_5_Flag;
	DMAx_Stream *Stream_6_Flag;
	DMAx_Stream *Stream_7_Flag;
}DMA_Flag_Instance;

typedef struct DMA_Flags{
	DMA_Flag_Instance *DMA_1_Flag;
	DMA_Flag_Instance *DMA_2_Flag;
}DMA_Flags;



#define DMA_Direction_Peripheral_to_Memory	0
#define DMA_Direction_Memory_to_Peripheral	1
#define DMA_Direction_Memory_to_Memory		2




typedef struct DMA_Init_Typedef{

	DMA_TypeDef *DMA_Port;
	DMA_Stream_TypeDef *Stream;
	int channel;
	bool circular_buff_enable;
	bool Half_Transfer_interrupt;
	bool Transfer_Complete_interrupt;
	bool Transfer_Error_interrupt;
	bool FIFO_Overrun_Underrun_interrupt;
	bool Direct_Mode_Error_interrupt;
	uint32_t Desitnation_Address;
	uint32_t Source_Address;
	int data_length;
	uint8_t direction;
	uint8_t priority;
	uint8_t memory_data_size;
	uint8_t periphera_data_size;
	DMA_Flags *Flags;
	int stream_temp;
}DMA_Init_Typedef;


enum data_size{
	Byte = 0,
	Half_Word,
	Word,
};

#endif /* DMA_DIRECTIVES_H_ */
