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







void W25Qx_Release_Power_Down(void)
{
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0xAB);
	SPI_CSS_High(W25QX);
}


void W25QX_Init(SPI_TypeDef *PORT, bool remap_pin)
{
	W25QX.SPI = PORT;
	W25QX.Frame = SPI_Frame_8Bit;
	W25QX.type = SPI_Full_Duplex_Master;
	W25QX.mode = SPI_Master;
	W25QX.LSB_MSB = 0;
	W25QX.frequency = 5;
	W25QX.nCSS_Port = GPIOA;
	W25QX.nCSS_pin = 15;
	W25QX.pin_remap = remap_pin;
	W25QX.polarity = 1;
	W25QX.phase = 1;
	SPI_Init(W25QX);

	W25Qx_Release_Power_Down();
	Delay_us(5);

	Read_Status_Register();


}

void W25QX_Read(int *MF, int *ID)
{
	int x[3];
	SPI_CSS_Low(W25QX);
	SPI_TRX_Data(W25QX, 0x9F);
	x[0] = SPI_TRX_Data(W25QX, 0xAA);
	x[1] = SPI_TRX_Data(W25QX, 0xAA);
	x[2] = SPI_TRX_Data(W25QX, 0xAA);
	*MF = x[0];
	*ID = (x[1] << 8) | x[2];
	SPI_CSS_High(W25QX);
}




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
