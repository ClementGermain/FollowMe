#include "stm32f10x_rcc.h"
#include "gpio.h"
#include "pwm.h"
#include "motor.h"
#include "SPI_Interface/SPI_Interface.h"
#include "Barstow/Barstow.h"

/** @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void);

int main(void)
{
	// RCC Configuration
  RCC_Configuration();
	
	StartBarstow();
}


void RCC_Configuration(void)
{
  /* PCLK1 = HCLK/4 */
  RCC_PCLK1Config(RCC_HCLK_Div4);

  /* TIM clock enable */
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM2 | 
													RCC_APB1Periph_TIM3 | 
													RCC_APB1Periph_TIM4 |
													RCC_APB1Periph_SPI2 , ENABLE);

  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_TIM1 |
													RCC_APB2Periph_GPIOB |
													RCC_APB2Periph_GPIOA |
													RCC_APB2Periph_AFIO , ENABLE);
  }
