#include "stm32f10x_rcc.h"
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../interface/motor.h"
#include "../interface/spi.h"
#include "barstow.h"

/** @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void);

int main(void)
{
	/*<! RCC Configuration */
  RCC_Configuration();
	
//	/*<! Start Barstow car's program. */
//	StartBarstow();
	int Duree_us;
	int value=0;
	Duree_us = 500000;
	Timer_Configure(TIM2, Duree_us);
	
	while(1) {
		if((TIM2->SR & 0x1) == 0x1) 
{TIM2->SR = TIM2->SR &~ 0x1; }
		
				
		}
}


void RCC_Configuration(void)
{
  /*<! PCLK1 = HCLK/4 */
  RCC_PCLK1Config(RCC_HCLK_Div4);

  /*<! Enable clock for needed peripherals. */
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM2 | 
													RCC_APB1Periph_TIM3 | 
													RCC_APB1Periph_TIM4 |
													RCC_APB1Periph_SPI2 , ENABLE);

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_TIM1 |
													RCC_APB2Periph_GPIOB |
													RCC_APB2Periph_GPIOA |
													RCC_APB2Periph_AFIO , ENABLE);
  
	RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_DMA1, 	ENABLE);
  
}
