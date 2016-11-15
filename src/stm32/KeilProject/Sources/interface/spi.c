#include "spi.h"

void InitializeSPI2(unsigned char * receiveBuffer, int receiveLen, unsigned char * sendBuffer, int sendLen)
{
	/********** GPIO configuration *************************/
	/*<! Configure used pins for GPIOB. */
	Init_GPIO(GPIOB, 	GPIO_Pin_14, GPIO_Mode_AF_PP);
	Init_GPIO(GPIOB, 	GPIO_Pin_12 |
										GPIO_Pin_15 | 
										GPIO_Pin_13, GPIO_Mode_IN_FLOATING); 
	
	/********* SPI_Slave configuration ********************/
	
	/*<! Reset SPI Interface. */
	SPI_I2S_DeInit(SPI2);

	/*<! SPI2 configuration. */
	SPI_InitTypeDef SPI_InitStructure; /*<! Variable used to setup the SPI. */	
	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	/************************* NVIC ***********************/
	/*<! Enable DMA1 channel4 IRQ Channel. */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*<! Enable DMA1 channel5 IRQ Channel. */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_Init(&NVIC_InitStructure);



	/*<! ==Configure DMA1 - Channel4== (SPI -> memory). */
	DMA_DeInit(DMA1_Channel4); //Set DMA registers to default values
	DMA_InitTypeDef DMA_InitStructure; /*<! Variable used to setup the DMA. */
	DMA_StructInit(&DMA_InitStructure);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR; //Address of peripheral the DMA must map to
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)receiveBuffer; //Variable to which received data will be stored
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = receiveLen; //Buffer size
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure); //Initialise the DMA
	DMA_Cmd(DMA1_Channel4, ENABLE); //Enable the DMA1 - Channel4         

	/*<! ==Configure DMA1 - Channel5== (memory -> SPI). */
	DMA_DeInit(DMA1_Channel5); //Set DMA registers to default values
	DMA_StructInit(&DMA_InitStructure);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR; //Address of peripheral the DMA must map to
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)sendBuffer; //Variable from which data will be transmitted
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_BufferSize = sendLen; //Buffer size
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure); //Initialise the DMA
	DMA_Cmd(DMA1_Channel5, ENABLE); //Enable the DMA1 - Channel5         

	/*<! Enable SPI2. */
	SPI_Cmd(SPI2, ENABLE);

	/*<! Enable the SPI2 RX & TX DMA requests. */
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);
	
	/*<! Enable DMA interrupts. */
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);
}
