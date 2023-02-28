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

void W25QX_Init(SPI_TypeDef *PORT, bool remap_pin);
void W25QX_Read(int *MF, int *ID);

void W25Qx_Write_Page(uint32_t address, int *data[], int len);
//int W25Qx_Read_Page(int i);
int W25Qx_Chip_Erase(void);

int W25Qx_Block_Erase_32KB(uint32_t address);
int W25Qx_Read_Page(uint32_t address);

#endif /* W25QX_DRIVER_H_ */
