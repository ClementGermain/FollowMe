#include "stm32f10x.h"
#include "gpio.h"
#include "pwm.h"
#include "motor.h"
#include "SPI_Interface/SPI_Interface.h"
#include "Barstow/Barstow.h"

// Uncomment the line below according to the target STM32 device used

//#define STM32F10X_LD 					// STM32F10X_LD: STM32 Low density devices
//#define STM32F10X_LD_VL				// STM32F10X_LD_VL: STM32 Low density Value Line devices
//#define STM32F10X_MD 					// STM32F10X_MD: STM32 Medium density devices
//#define STM32F10X_MD_VL 			// STM32F10X_MD_VL: STM32 Medium density Value Line devices
//#define STM32F10X_HD 					// STM32F10X_HD: STM32 High density devices
//#define STM32F10X_HD_VL 			// STM32F10X_HD_VL: STM32 High density value line devices
//#define STM32F10X_XL					// STM32F10X_XL: STM32 XL-density devices
//#define STM32F10X_CL					// STM32F10X_CL: STM32 Connectivity line devices

void RCC_Configuration(void);

int main(void)
{
	// RCC Configuration
  RCC_Configuration();

  StartBarstow();

  while (1)
  {	// Test the motors
	Motor_Test();
	}
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);

  /* GPIO clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_AFIO, ENABLE);
}
