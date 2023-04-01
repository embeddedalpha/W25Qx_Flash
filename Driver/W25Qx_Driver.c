/*
 * W25Qx_Driver.c
 *
 *  Created on: Feb 27, 2023
 *      Author: Kunal
 */


#include "W25Qx_Driver.h"

uint32_t Last_Address ;

SPI_Config W25QX;
DMA_Init_Typedef W25Qx_DMA, W25Qx_DMA_RX;

struct Status_Register
{
	int SRP ;
	int SEC ;
	int TB ;
	int	BP2 ;
	int BP1 ;
	int BP0 ;
	int WEL ;
	int BUSY;
	int SUS ;
	int CMP ;
	int LB3 ;
	int LB2 ;
	int LB1 ;
	int QE ;
	int SRL;
	int DRV1;
	int DRV2;
	int WPS;
}Status_Register;

struct Status_Register SR;


/*
 * #########################################################################################################
 * ############################################### Internal Operation ######################################
 * #########################################################################################################
 */

static void Read_Status_Register(void)
{
	int x[3];
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x05);
	x[0] = SPI_TRX_Data(W25QX, 0xAA);
	x[1] = SPI_TRX_Data(W25QX, 0xAA);
	x[2] = SPI_TRX_Data(W25QX, 0xAA);
	SPI_CSS_High(W25QX);

	SR.SRP  = ((1 << 7) & x[0]) >> 7;
	SR.SEC  = ((1 << 6) & x[0]) >> 6;
	SR.TB   = ((1 << 5) & x[0]) >> 5;
	SR.BP2  = ((1 << 4) & x[0]) >> 4;
	SR.BP1  = ((1 << 3) & x[0]) >> 3;
	SR.BP0  = ((1 << 2) & x[0]) >> 2;
	SR.WEL  = ((1 << 1) & x[0]) >> 1;
	SR.BUSY = ((1 << 0) & x[0]) >> 0;

	SR.SUS  = ((1 << 7) & x[1]) >> 7;
	SR.CMP  = ((1 << 6) & x[1]) >> 6;
	SR.LB3  = ((1 << 5) & x[1]) >> 5;
	SR.LB2  = ((1 << 4) & x[1]) >> 4;
	SR.LB1  = ((1 << 3) & x[1]) >> 3;
	SR.QE   = ((1 << 1) & x[1]) >> 1;
	SR.SRL  = ((1 << 0) & x[1]) >> 0;

	SR.DRV1  = ((1 << 6) & x[1]) >> 6;
	SR.DRV2  = ((1 << 5) & x[1]) >> 5;
	SR.WPS  = ((1 << 2) & x[1]) >> 2;
}

static int Write_Enable()
{
	int retval;
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x06);
	SPI_CSS_High(W25QX);

	Read_Status_Register();
	if(SR.WEL)
	{
		retval = 1;
	}
	else
	{
		retval = -1;
	}
	return retval;
}

static int Write_Disable()
{
	int retval;
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x04);
	SPI_CSS_High(W25QX);
	Read_Status_Register();
	if(SR.WEL)
	{
		retval = -1;
	}
	else
	{
		retval = 1;
	}
	return retval;
}



/*
 * #########################################################################################################
 * ############################################### Operation ###############################################
 * #########################################################################################################
 */

void W25QX_Init(W25Qx_Typedef *self)
{
	W25QX.SPI = self->SPI_Port;
	W25QX.Frame = SPI_Frame_8Bit;
	W25QX.type = SPI_Full_Duplex_Master;
	W25QX.mode = SPI_Master;
	W25QX.LSB_MSB = 0;
	W25QX.frequency = 3;
	W25QX.nCSS_Port = self->nCSS_Port;
	W25QX.nCSS_pin = self->nCSS_Pin;
	W25QX.pin_remap = self->pin_remap;
	W25QX.polarity = 1;
	W25QX.phase = 1;
	W25QX.DMA_TX = 1;
	W25QX.DMA_RX = 0;
	SPI_Init(W25QX);

/*************************************************************************************/

	W25Qx_Release_Power_Down();
	Delay_us(5);

	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x9F);
	self->Manufacturer_ID = SPI_TRX_Data(W25QX, 0xAA);
	self->Memory_Type = SPI_TRX_Data(W25QX, 0xAA);
	self->Capacity = SPI_TRX_Data(W25QX, 0xAA);
	SPI_CSS_High(W25QX);

	uint64_t x[8];
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x4B);
	SPI_TRX_Data(W25QX, 0xAA);
	SPI_TRX_Data(W25QX, 0xAA);
	SPI_TRX_Data(W25QX, 0xAA);
	SPI_TRX_Data(W25QX, 0xAA);
	for(int i = 0 ; i < 8; i++)
	{
		x[i] = SPI_TRX_Data(W25QX, 0xAA);
	}
	self->Unique_ID = (x[0] << 56) | (x[1] << 48) | (x[2] << 40) | (x[3] << 32) | (x[4] << 24) | (x[5] << 16) |
			(x[6] << 8) | (x[7] << 0);
	SPI_CSS_High(W25QX);

	if(self->Memory_Type == 0x13)	//W25Q80
	{
		self->Last_Address = 0x0FFFFF;
	}
	if(self->Memory_Type == 64)	//W25Q16
	{
		self->Last_Address = 0x1FFFFF;
	}
	if(self->Memory_Type == 0x15)	//W25Q32
	{
		self->Last_Address = 0x3FFFFF;
	}

}

void W25Qx_Release_Power_Down(void)
{
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0xAB);
	SPI_CSS_High(W25QX);
}



uint32_t W25Qx_Write_Byte(W25Qx_Typedef *self,uint32_t address, int data)
{
	uint32_t temp;
	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x02);
	SPI_TRX_Data(W25QX, (0xFF0000 & address) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & address) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & address) >> 0);
	SPI_TRX_Data(W25QX, data);
	SPI_CSS_High(W25QX);
	Write_Disable();
	W25Qx_Store_Last_Position(self, address);
	temp = W25Qx_Read_Last_Write_Poisition(self);
	return temp;

}



int W25Qx_Read_Byte(uint32_t address)
{
	int32_t LA = (int32_t)address;
	int read = 0;
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x03);
	SPI_TRX_Data(W25QX, (0xFF0000 & LA) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & LA) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & LA) >> 0);
	read = SPI_TRX_Data(W25QX, 0xAA);
	SPI_CSS_High(W25QX);
	return read;
}




void W25Qx_Store_Last_Position(W25Qx_Typedef *self, uint64_t last_address)
{
	uint32_t write[3];

	last_address = last_address + 0;

	write[0] = (0xFF0000 & last_address) >> 16;
	write[1] = (0x00FF00 & last_address) >> 8;
	write[2] = (0x0000FF & last_address) >> 0;

	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x02);
	SPI_TRX_Data(W25QX, (0xFF0000 & (self->Last_Address -0)) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & (self->Last_Address -0)) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & (self->Last_Address -0)) >> 0);
	SPI_TRX_Data(W25QX, write[0]);
	SPI_CSS_High(W25QX);
	Write_Disable();

	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x02);
	SPI_TRX_Data(W25QX, (0xFF0000 & (self->Last_Address -1)) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & (self->Last_Address -1)) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & (self->Last_Address -1)) >> 0);
	SPI_TRX_Data(W25QX, write[1]);
	SPI_CSS_High(W25QX);
	Write_Disable();

	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x02);
	SPI_TRX_Data(W25QX, (0xFF0000 & (self->Last_Address -2)) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & (self->Last_Address -2)) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & (self->Last_Address -2)) >> 0);
	SPI_TRX_Data(W25QX, write[2]);
	SPI_CSS_High(W25QX);
	Write_Disable();

}

uint32_t W25Qx_Read_Last_Write_Poisition(W25Qx_Typedef *self)
{
	uint32_t ret;
	uint32_t read[3];
	read[0] = W25Qx_Read_Byte(self->Last_Address-0);
	read[1] = W25Qx_Read_Byte(self->Last_Address-1);
	read[2] = W25Qx_Read_Byte(self->Last_Address-2);
	ret =  (read[0] << 16) |  (read[1] << 8) | (read[2] << 0);
	return ret;
}


void W25Qx_Write_Page(W25Qx_Typedef *self,int32_t address, uint8_t *data, int len)
{
	uint32_t LA;
	uint8_t buffer[300];

	if(address == -1)
	{
		LA = W25Qx_Read_Last_Write_Poisition(self);
	}
	else
	{
		LA = (uint32_t)address;
	}

	buffer[0] = 0x02;
	buffer[1] = ((0xFF0000 & LA) >> 16);
	buffer[2] = ((0x00FF00 & LA) >> 8);
	buffer[3] = ((0x0000FF & LA) >> 0);
	DMA_Memory_To_Memory_Transfer(data, 8, 8, (uint32_t)&buffer[4], 1, 1, len);

	Write_Enable();

	W25QX.DMA_TX = 1;
	SPI_Enable_DMA(W25QX);
	W25Qx_DMA.DMA_Port = SPI1_DMA_Port;
	W25Qx_DMA.Stream = SPI1_DMA_TX_STREAM5;
	W25Qx_DMA.channel = SPI1_DMA_TX_CHANNEL;
	W25Qx_DMA.Transfer_Complete_interrupt = 1;
	W25Qx_DMA.circular_buff_enable = 0;
	W25Qx_DMA.memory_data_size = Byte;
	W25Qx_DMA.periphera_data_size = Byte;
	W25Qx_DMA.direction = DMA_Direction_Memory_to_Peripheral;
	DMA_Init(&W25Qx_DMA);

	W25Qx_DMA.Source_Address = (uint32_t)&(self->SPI_Port->DR);
	W25Qx_DMA.Desitnation_Address = (uint32_t)&(buffer[0]);
	W25Qx_DMA.data_length = len+4;
	SPI_CSS_Low(W25QX);
	DMA_Trigger(&W25Qx_DMA,1);
	Delay_us(5);
	SPI_CSS_High(W25QX);
	SPI_Disable_DMA(W25QX);
	DMA_DeInit(&W25Qx_DMA);
	Write_Disable();
	LA = LA + len;
	W25Qx_Store_Last_Position(self, LA);
//	DMA_DeInit(&W25Qx_DMA);

}

void W25Qx_Read_Page(W25Qx_Typedef *self,int32_t starting_address, int32_t ending_address, uint8_t *data, uint32_t length)
{
	W25QX.DMA_TX = 1;
	W25QX.DMA_RX = 1;
	SPI_Enable_DMA(W25QX);

	uint32_t source_dummy = 0xAA;
	uint8_t tx_buffer[300];


	uint32_t ret_value;

	if(ending_address == -1)
	{
		ending_address = starting_address + length;
		ret_value = ending_address;
	}
	if(length == -1)
	{
		length = ending_address - starting_address;
		ret_value = length;
	}

	tx_buffer[0] = 0x03;
	tx_buffer[1] = ((0xFF0000 & starting_address) >> 16);
	tx_buffer[2] = ((0x00FF00 & starting_address) >> 8);
	tx_buffer[3] = ((0x0000FF & starting_address) >> 0);
	DMA_Memory_To_Memory_Transfer(&source_dummy, 8, 8, (uint32_t)&tx_buffer[4], 0, 1, length);


	W25Qx_DMA.DMA_Port = SPI1_DMA_Port;
	W25Qx_DMA.Stream = SPI1_DMA_TX_STREAM5;
	W25Qx_DMA.channel = SPI1_DMA_TX_CHANNEL;
	W25Qx_DMA.Transfer_Complete_interrupt = 1;
	W25Qx_DMA.circular_buff_enable = 0;
	W25Qx_DMA.memory_data_size = Byte;
	W25Qx_DMA.periphera_data_size = Byte;
	W25Qx_DMA.direction = DMA_Direction_Memory_to_Peripheral;
	DMA_Init(&W25Qx_DMA);

	W25Qx_DMA.Source_Address = (uint32_t)&(self->SPI_Port->DR);
	W25Qx_DMA.Desitnation_Address = (uint32_t)&(tx_buffer[0]);
	W25Qx_DMA.data_length = length+5;

	DMA_Set_Target(&W25Qx_DMA);

	W25Qx_DMA_RX.DMA_Port = SPI1_DMA_Port;
	W25Qx_DMA_RX.Stream = SPI1_DMA_RX_STREAM2;
	W25Qx_DMA_RX.channel = SPI1_DMA_RX_CHANNEL;
	W25Qx_DMA_RX.Transfer_Complete_interrupt = 1;
	W25Qx_DMA_RX.circular_buff_enable = 0;
	W25Qx_DMA_RX.memory_data_size = Half_Word;
	W25Qx_DMA_RX.periphera_data_size = Half_Word;
	W25Qx_DMA_RX.direction = DMA_Direction_Peripheral_to_Memory;
	DMA_Init(&W25Qx_DMA_RX);

	W25Qx_DMA_RX.Source_Address = (uint32_t)&(self->SPI_Port->DR);
	W25Qx_DMA_RX.Desitnation_Address = (uint32_t)&(tx_buffer[0]);
	W25Qx_DMA_RX.data_length = length+5;
	DMA_Set_Target(&W25Qx_DMA_RX);


	SPI_CSS_Low(W25QX);
	DMA_Enable(&W25Qx_DMA);
	DMA_Enable(&W25Qx_DMA_RX);

	while(DMA_Check_TX_Complete_Flag(&W25Qx_DMA) == 5);
	while(DMA_Check_TX_Complete_Flag(&W25Qx_DMA_RX) == 2);

	SPI_CSS_High(W25QX);
	DMA_Disable(&W25Qx_DMA);
	DMA_Disable(&W25Qx_DMA_RX);

	DMA_Memory_To_Memory_Transfer(&tx_buffer[5], 8, 8, data, 1, 1, length);

	return ret_value;
}

void W25Qx_Fast_Read_Page(W25Qx_Typedef *self,int32_t starting_address, int32_t ending_address, uint8_t *data, uint32_t length)
{
	W25QX.DMA_TX = 1;
	W25QX.DMA_RX = 1;
	SPI_Enable_DMA(W25QX);

	uint32_t source_dummy = 0xAA;
	uint8_t tx_buffer[300];


	uint32_t ret_value;

	if(ending_address == -1)
	{
		ending_address = starting_address + length;
		ret_value = ending_address;
	}
	if(length == -1)
	{
		length = ending_address - starting_address;
		ret_value = length;
	}

	tx_buffer[0] = 0x0B;
	tx_buffer[1] = ((0xFF0000 & starting_address) >> 16);
	tx_buffer[2] = ((0x00FF00 & starting_address) >> 8);
	tx_buffer[3] = ((0x0000FF & starting_address) >> 0);
	tx_buffer[4] = 0xAA;
	DMA_Memory_To_Memory_Transfer(&source_dummy, 8, 8, (uint32_t)&tx_buffer[5], 0, 1, length);


	W25Qx_DMA.DMA_Port = SPI1_DMA_Port;
	W25Qx_DMA.Stream = SPI1_DMA_TX_STREAM5;
	W25Qx_DMA.channel = SPI1_DMA_TX_CHANNEL;
	W25Qx_DMA.Transfer_Complete_interrupt = 1;
	W25Qx_DMA.circular_buff_enable = 0;
	W25Qx_DMA.memory_data_size = Byte;
	W25Qx_DMA.periphera_data_size = Byte;
	W25Qx_DMA.direction = DMA_Direction_Memory_to_Peripheral;
	DMA_Init(&W25Qx_DMA);

	W25Qx_DMA.Source_Address = (uint32_t)&(self->SPI_Port->DR);
	W25Qx_DMA.Desitnation_Address = (uint32_t)&(tx_buffer[0]);
	W25Qx_DMA.data_length = length+6;

	DMA_Set_Target(&W25Qx_DMA);

	W25Qx_DMA_RX.DMA_Port = SPI1_DMA_Port;
	W25Qx_DMA_RX.Stream = SPI1_DMA_RX_STREAM2;
	W25Qx_DMA_RX.channel = SPI1_DMA_RX_CHANNEL;
	W25Qx_DMA_RX.Transfer_Complete_interrupt = 1;
	W25Qx_DMA_RX.circular_buff_enable = 0;
	W25Qx_DMA_RX.memory_data_size = Half_Word;
	W25Qx_DMA_RX.periphera_data_size = Half_Word;
	W25Qx_DMA_RX.direction = DMA_Direction_Peripheral_to_Memory;
	DMA_Init(&W25Qx_DMA_RX);

	W25Qx_DMA_RX.Source_Address = (uint32_t)&(self->SPI_Port->DR);
	W25Qx_DMA_RX.Desitnation_Address = (uint32_t)&(tx_buffer[0]);
	W25Qx_DMA_RX.data_length = length+6;
	DMA_Set_Target(&W25Qx_DMA_RX);


	SPI_CSS_Low(W25QX);
//	Delay_us(10);
	DMA_Enable(&W25Qx_DMA);
	DMA_Enable(&W25Qx_DMA_RX);

	while((DMA2 -> LISR & (DMA_LISR_TCIF2_Msk)) == 0){}
	DMA2 -> LIFCR |= DMA_LIFCR_CTCIF2;
	DMA2 -> LIFCR |= DMA_LIFCR_CHTIF2;

	while((DMA2 -> HISR & (DMA_HISR_TCIF5_Msk)) == 0){}
	DMA2 -> HIFCR |= DMA_HIFCR_CTCIF5;
	DMA2 -> HIFCR |= DMA_HIFCR_CHTIF5;

	SPI_CSS_High(W25QX);
	DMA_Disable(&W25Qx_DMA);
	DMA_Disable(&W25Qx_DMA_RX);

	DMA_Memory_To_Memory_Transfer(&tx_buffer[6], 8, 8, data, 1, 1, length);

	return ret_value;
}


/*
 * #########################################################################################################
 * ################################################# Chip Erase ############################################
 * #########################################################################################################
 */

int W25Qx_Chip_Erase(void)
{
	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x60);
	SPI_CSS_High(W25QX);
	do{
		Read_Status_Register();
	}while(SR.BUSY == 1);
	Write_Disable();
	return 1;
}

int W25Qx_Block_Erase(enum Blocks Block, uint32_t address)
{
	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, Block);
	SPI_TRX_Data(W25QX, (0xFF0000 & address) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & address) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & address) >> 0);
	SPI_CSS_High(W25QX);
	do{
		Read_Status_Register();
	}while(SR.BUSY == 1);
	Write_Disable();
	return 1;

}





/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ NOT TESTED FUNCTIONS $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/



