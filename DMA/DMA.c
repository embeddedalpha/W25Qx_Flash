/*
 * DMA.c
 *
 *  Created on: Nov 23, 2022
 *      Author: Kunal
 */


#include "DMA.h"



int DMA_Init(DMA_Init_Typedef *DMA_Config)
{
	int ret_value = 0;
	if(DMA_Config->DMA_Port == DMA1)
	{
		RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	}
	else if(DMA_Config->DMA_Port == DMA2)
	{
		RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	}
	else
	{
		ret_value = -1;
	}

	DMA_Config -> Stream->CR &= ~DMA_SxCR_EN; //Disable DMA Stream
	DMA_Config -> Stream->CR |= (DMA_Config->channel) << DMA_SxCR_CHSEL_Pos; //Select Channel
	DMA_Config -> Stream->CR |= DMA_SxCR_MINC; //Enable memory increment mode
	//Disable Interrupt
	DMA_Config -> Stream->CR  &= ~(DMA_SxCR_HTIE | DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);
	DMA_Config -> Stream->FCR &= ~(DMA_SxFCR_FEIE);
	//Enable Interrupt
	DMA_Config -> Stream->CR  |= (DMA_Config->Half_Transfer_interrupt     << DMA_SxCR_HTIE_Pos);
	DMA_Config -> Stream->CR  |= (DMA_Config->Transfer_Complete_interrupt << DMA_SxCR_TCIE_Pos);
	DMA_Config -> Stream->CR  |= (DMA_Config->Transfer_Error_interrupt    << DMA_SxCR_TEIE_Pos);
	DMA_Config -> Stream->CR  |= (DMA_Config->Direct_Mode_Error_interrupt << DMA_SxCR_DIR_Pos);
	DMA_Config -> Stream->FCR |= (DMA_Config->FIFO_Overrun_Underrun_interrupt << DMA_SxFCR_FEIE_Pos);

	switch (DMA_Config->direction)
	{
		case DMA_Direction_Peripheral_to_Memory:
		{
			DMA_Config->Stream->M0AR = (uint32_t)(DMA_Config->Desitnation_Address);
			DMA_Config->Stream->PAR  = (uint32_t)(DMA_Config->Source_Address);
		}
			break;
		case DMA_Direction_Memory_to_Peripheral:
		{
			DMA_Config->Stream->M0AR =  (uint32_t)(DMA_Config->Source_Address);
			DMA_Config->Stream->PAR  =  (uint32_t)(DMA_Config->Desitnation_Address);
		}
			break;
		case DMA_Direction_Memory_to_Memory:
		{
			DMA_Config->Stream->M0AR =  (uint32_t)(DMA_Config->Source_Address);
			DMA_Config->Stream->PAR  =  (uint32_t)(DMA_Config->Desitnation_Address);
		}
			break;
		default:
		{
			ret_value = -1;
		}
			break;
	}

	//Priority
	DMA_Config -> Stream->CR &= ~DMA_SxCR_PL;
	DMA_Config -> Stream->CR |=  DMA_Config->priority << DMA_SxCR_PL_Pos;

	//Direction
	DMA_Config -> Stream->CR &= ~DMA_SxCR_DIR;
	DMA_Config -> Stream->CR |=  DMA_Config->direction << DMA_SxCR_DIR_Pos;

	return ret_value;

}




int DMA_Trigger(DMA_Init_Typedef *DMA_Config)
{
	int ret_value = 0;

//	DMA_Config->Stream->CR &= ~DMA_SxCR_EN;



	DMA_Config->DMA_Port->HIFCR |= DMA_HIFCR_CTCIF7;





	DMA_Config->Stream->NDTR = DMA_Config->data_length;

	DMA_Config->Stream->CR |= DMA_SxCR_EN;


	return ret_value;
}

