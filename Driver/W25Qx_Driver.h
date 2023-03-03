/*
 * W25Qx_Driver.h
 *
 *  Created on: Feb 27, 2023
 *      Author: Kunal
 */

#ifndef W25QX_DRIVER_H_
#define W25QX_DRIVER_H_

#include "main.h"
#include "SPI.h"
#include "Console.h"




typedef struct W25Qx_Typedef{
	SPI_TypeDef *SPI_Port;
	bool pin_remap;
	uint8_t Manufacturer_ID;
	uint8_t Memory_Type;
	uint8_t Capacity;
	uint64_t Unique_ID;
	uint64_t Last_Address;
}W25Qx_Typedef;



void W25QX_Init(W25Qx_Typedef *self);
void W25Qx_Release_Power_Down(void);
void W25Qx_Write_Page(uint32_t address, int *data[], int len);
void W25Qx_Write_Byte(uint32_t address, int data);
int W25Qx_Read_Page(uint32_t address);
void W25Qx_Store_Last_Position(W25Qx_Typedef *self, uint64_t last_address);
uint32_t W25Qx_Read_Last_Write_Poisition(W25Qx_Typedef *self);
int W25Qx_Chip_Erase(void);
int W25Qx_Block_Erase_4KB(uint32_t address);
int W25Qx_Block_Erase_32KB(uint32_t address);
int W25Qx_Block_Erase_64KB(uint32_t address);


#endif /* W25QX_DRIVER_H_ */
