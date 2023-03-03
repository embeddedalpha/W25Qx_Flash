/*
 * W25Qx_Driver.c
 *
 *  Created on: Feb 27, 2023
 *      Author: Kunal
 */


#include "W25Qx_Driver.h"

SPI_Config W25QX;


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
	W25QX.frequency = 0;
	W25QX.nCSS_Port = GPIOA;
	W25QX.nCSS_pin = 15;
	W25QX.pin_remap = self->pin_remap;
	W25QX.polarity = 1;
	W25QX.phase = 1;
	SPI_Init(W25QX);

	W25Qx_Release_Power_Down();
	Delay_us(5);

	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x4B);
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
	if(self->Memory_Type == 0x14)	//W25Q16
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

/*
 * #########################################################################################################
 * ################################################# Read & Write ##########################################
 * #########################################################################################################
 */

void W25Qx_Write_Page(uint32_t address, int *data[], int len)
{
	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x02);
	SPI_TRX_Data(W25QX, (0xFF0000 & address) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & address) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & address) >> 0);
	for(int i = 0; i < len; i++)
	{
		SPI_TRX_Data(W25QX, data[i]);
	}
	SPI_CSS_High(W25QX);
	Write_Disable();
}

void W25Qx_Write_Byte(uint32_t address, int data)
{
	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x02);
	SPI_TRX_Data(W25QX, (0xFF0000 & address) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & address) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & address) >> 0);
	SPI_TRX_Data(W25QX, data);
	SPI_CSS_High(W25QX);
	Write_Disable();
}

int W25Qx_Read_Page(uint32_t address)
{
	int read;
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x03);
	SPI_TRX_Data(W25QX, (0xFF0000 & address) >> 16);
	SPI_TRX_Data(W25QX, (0x00FF00 & address) >> 8);
	SPI_TRX_Data(W25QX, (0x0000FF & address) >> 0);
	read = SPI_TRX_Data(W25QX, 0xAA);
	SPI_CSS_High(W25QX);
	return read;
}

void W25Qx_Store_Last_Position(W25Qx_Typedef *self, uint64_t last_address)
{
	uint32_t write[3];
	write[0] = (0xFF0000 & self->Last_Address) >> 16;
	write[1] = (0x00FF00 & self->Last_Address) >> 8;
	write[2] = (0x0000FF & self->Last_Address) >> 0;
	W25Qx_Write_Byte(self->Last_Address-0, write[0]);
	W25Qx_Write_Byte(self->Last_Address-1, write[1]);
	W25Qx_Write_Byte(self->Last_Address-2, write[2]);

}

uint32_t W25Qx_Read_Last_Write_Poisition(W25Qx_Typedef *self)
{
	uint32_t ret;
	uint32_t read[3];
	read[0] = W25Qx_Read_Page(self->Last_Address-0);
	read[1] = W25Qx_Read_Page(self->Last_Address-1);
	read[2] = W25Qx_Read_Page(self->Last_Address-2);
	ret =  (read[0] << 16) |  (read[1] << 8) | (read[2] << 0);
	return ret;
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

int W25Qx_Block_Erase_4KB(uint32_t address)
{

	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x20);
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

int W25Qx_Block_Erase_32KB(uint32_t address)
{

	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x53);
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



int W25Qx_Block_Erase_64KB(uint32_t address)
{

	Write_Enable();
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0xD8);
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
