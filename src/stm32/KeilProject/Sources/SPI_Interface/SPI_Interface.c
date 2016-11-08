#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

unsigned char Slave_rev_buff[50];

void Test_SPI()
{
	/***** GPIO *****/
		// Enable clock for GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
		// Configure used pins for GPIOB
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	
		//Enable clock for GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		// Configure used pins for GPIOB
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
	/***** Init SPI *****/		
		// Enable clock for SPI3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		
		// Initialise SPI3
    SPI_InitTypeDef SPI_InitStruct;
		SPI_StructInit(&SPI_InitStruct);
		SPI_Init(SPI3, &SPI_InitStruct);
		
		SPI_Cmd(SPI3, ENABLE); // enable SPI3

}

void SPI3_IRQHandler(void)
{
    //unsigned char rx;
    static unsigned short /*int count = 0,*/ i = 0 ;

		i= (i==50 ? 0 : i);

    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_TXE) == SET)
    {
        SPI3->DR  = 0xFC ;
    }
    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
    {
        Slave_rev_buff[i] = SPI3->DR ;
        i++ ;
    }

}
