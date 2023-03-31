/*
 * DMA.h
 *
 *  Created on: Nov 23, 2022
 *      Author: Kunal
 */

#ifndef DMA_H_
#define DMA_H_

#include "main.h"
#include "DMA_Directives.h"



void DMA_DeInit(DMA_Init_Typedef *DMA_Config);

int DMA_Init(DMA_Init_Typedef *DMA_Config);

void DMA_Memory_To_Memory_Transfer(uint32_t *source,
						  uint8_t source_data_size, uint8_t dest_data_size,
						  uint32_t *destination, bool source_increment,
						  bool destination_increment, uint16_t length);

int DMA_Trigger(DMA_Init_Typedef *DMA_Config, bool blocking_enable);
void DMA_Enable(DMA_Init_Typedef *DMA_Config);
void DMA_Disable(DMA_Init_Typedef *DMA_Config);
void DMA_Set_Target(DMA_Init_Typedef *DMA_Config);

#endif /* DMA_H_ */
