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

#define NULL_ADDRESS -1


typedef struct W25Qx_Typedef{
	SPI_TypeDef *SPI_Port;
	GPIO_TypeDef *nCSS_Port;
	uint8_t nCSS_Pin;
	bool pin_remap;
	uint8_t Manufacturer_ID;
	uint8_t Memory_Type;
	uint8_t Capacity;
	uint64_t Unique_ID;
	uint64_t Last_Address;
}W25Qx_Typedef;

//val

enum Blocks
{
	Block_4KB  = 0x20,
	Block_32KB = 0x52,
	Block_64KB = 0xD8,
};

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ TESTED FUNCTIONS $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

/***************************************************************************************
 * @name:		W25QX_Init
 * @brief:		Initializes the W25Qx flash chip. Clock Speed is at highest
 * @in_param:	.SPI_Port = SPI1
 * 				.pin_remap = true
 * @out_param:	.Manufacturer_ID
 * 				.Memory_Type
 * 				.Capacity
 * 				.Unique_ID
 * 				.Last_Address (Not used in main program)
 * @tests:		Performed all functions without failing
 */
void W25QX_Init(W25Qx_Typedef *self);

/***************************************************************************************
 * @name:		W25Qx_Release_Power_Down
 * @brief:		Releases W25Qx from power down mode.
 * @in_param:	none
 * @out_param:	none
 * @tests:		Performed all functions without failing
 */
void W25Qx_Release_Power_Down(void);

/***************************************************************************************
 * @name:		W25QX_Init
 * @brief:		Writes a byte to the memory at a particular location
 * @in_param:	W25Qx_Typedef -> structure
 * 				address       -> address of the data to be stored
 * 				data		  -> byte to be stored
 * @out_param:	last_address of the data
 * @tests:		Performed all functions without failing
 */
uint32_t W25Qx_Write_Byte(W25Qx_Typedef *self,uint32_t address, int data);

/***************************************************************************************
 * @name:		W25Qx_Store_Last_Position
 * @brief:		Stores the last memory address of the previous data at the end of the
 * 				chip memory
 * @in_param:	W25Qx_Typedef -> structure
 * 				address       -> address of the data to be stored
 * @out_param:	none
 * @tests:		Performed all functions without failing
 */
void W25Qx_Store_Last_Position(W25Qx_Typedef *self, uint64_t last_address);

/***************************************************************************************
 * @name:		W25Qx_Read_Last_Write_Poisition
 * @brief:		Reads the last memory address of the previous data which is store at
 * 				the end of the chip memory
 * @in_param:	W25Qx_Typedef -> structure
 * @out_param:	address of the last stored data
 * @tests:		Performed all functions without failing
 */
uint32_t W25Qx_Read_Last_Write_Poisition(W25Qx_Typedef *self);

/***************************************************************************************
 * @name:		W25Qx_Chip_Erase
 * @brief:		Erases the entire chip
 * @in_param:	none
 * @out_param:	none
 * @tests:		Performed all functions without failing
 */
int W25Qx_Chip_Erase(void);

/***************************************************************************************
 * @name:		W25Qx_Block_Erase
 * @brief:		Erases a certain block of the chip that can be selected by passing a
 * 				suitable value given in the enum Blocks.
 * @in_param:	enum Blocks -> Block
 * 				address     -> starting address of the block to be erased
 * @out_param:	1
 * @tests:		Performed all functions without failing
 */
int W25Qx_Block_Erase(enum Blocks Block, uint32_t address);

/***************************************************************************************
 * @name:		W25Qx_Read_Byte
 * @brief:		Reads one byte pointed by the memory
 * @in_param:	address     -> particular address of the byte to be read
 * @out_param:	1
 * @tests:		Performed all functions without failing
 */
int W25Qx_Read_Byte(uint32_t address);

/**************************************************************************************
 * @name:		W25Qx_Write_Page
 * @brief:		Writes a chunk of data to the memory pointed at an address. To write a
 * 				block of data, call this function and pass the instance of the chip
 * 				created earlier, the starting address of the block or if the starting
 * 				address is not known, pass -1 so that the last address can be used as
 * 				a starting address, pointer to the data block and the length of the
 * 				data.
 * @in_param:	W25Qx_Typedef -> structure
 * 				address       -> Starting address for the block or -1
 * 				data[]		  -> pointer of the data to be sent to the chip
 * 				length		  -> size of the data to be sent
 * @out_param:	none
 * @tests:		Not yet tested.
 */
void W25Qx_Write_Page(W25Qx_Typedef *self,int32_t address, uint8_t *data, int len);

/**************************************************************************************
 * @name:		W25Qx_Read_Page
 * @brief:		Reads a block of data from the memory starting from the starting
 * 				address. The user has to have either the ending address or the length
 * 				of the block they wish to access. The absence of the parameter should
 * 				be replaced with -1.
 * @in_param:	W25Qx_Typedef		-> structure
 * 				starting_address	-> Starting address for the block
 * 				ending_address		-> Ending address for the block / -1
 * 				data				-> pointer of the data to be sent to the chip
 * 				length				-> length of the block / -1
 * @out_param:	none
 * @tests:		Not yet tested.
 */
void W25Qx_Read_Page(W25Qx_Typedef *self,int32_t starting_address, int32_t ending_address, uint8_t *data, uint32_t length);

/**************************************************************************************
 * @name:		W25Qx_Fast_Read_Page
 * @brief:		Reads fast a block of data from the memory starting from the starting
 * 				address. The user has to have either the ending address or the length
 * 				of the block they wish to access. The absence of the parameter should
 * 				be replaced with -1.
 * 				Difference between W25Qx_Read_Page and this function is the clock
 * 				frequency of reading data.
 * @in_param:	W25Qx_Typedef		-> structure
 * 				starting_address	-> Starting address for the block
 * 				ending_address		-> Ending address for the block / -1
 * 				data				-> pointer of the data to be sent to the chip
 * 				length				-> length of the block / -1
 * @out_param:	none
 * @tests:		Not yet tested.
 *
 */
void W25Qx_Fast_Read_Page(W25Qx_Typedef *self,int32_t starting_address, int32_t ending_address, uint8_t *data, uint32_t length);





/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ NOT TESTED FUNCTIONS $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


#endif /* W25QX_DRIVER_H_ */
