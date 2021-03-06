#include "stm32f10x_rcc.h"
#include "library/gpio.h"
#include "library/timer.h"
#include "interface/motor.h"
#include "interface/spi.h"
#include "barstow/barstow.h"
#include "library/delay.h"
#include "interface/us_sensor.h"


/** @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void);

int main(void)
{ 
	/*<! RCC Configuration */
  RCC_Configuration();
	
	/*<! Start Barstow car's program. */
	StartBarstow();
	while(1){}
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
													RCC_APB2Periph_ADC1 |
													RCC_APB2Periph_ADC2 |
													RCC_APB2Periph_ADC3 |
													RCC_APB2Periph_GPIOB |
													RCC_APB2Periph_GPIOA |
													RCC_APB2Periph_GPIOC |
													RCC_APB2Periph_GPIOD |
													RCC_APB2Periph_AFIO , ENABLE);
  
	RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_DMA1, 	ENABLE);
  
}
