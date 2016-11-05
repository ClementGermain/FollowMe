
#include "stm32f10x.h"
#include "gpio.h"
#include "pwm.h"
 
void RCC_Configuration(void);
 
int main(void)
{
	// RCC Configuration
  RCC_Configuration();
 
  // Configure the GPIO ports 
  Init_GPIO_PWM(GPIOB, GPIO_Pin_6);
	
	// Configure the PWM
	InitializeTimer_1kHz(TIM4);
	InitializePWM(TIM4, 0.5);
 
  while (1)
  {}
}
 
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* PCLK1 = HCLK/4 */
  RCC_PCLK1Config(RCC_HCLK_Div4);
 
  /* TIM clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 
  /* GPIO clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_AFIO, ENABLE);
}
