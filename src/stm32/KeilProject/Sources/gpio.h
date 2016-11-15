#ifndef __GPIO__
#define __GPIO__

// include stm32 library
#include "stm32f10x_gpio.h"

/** @brief Init a GPIO Pin in a specific mode 
	* @param GPIO: GPIOA or GPIOB
	* @param Pin: Pin adress (like GPIO_Pin_6)
	* @param Mode: Mode to be use on the GPIO (see GPIOMode_TypeDef) for more informations about the GPIO's modes)
	* @retval None
*/
void Init_GPIO(GPIO_TypeDef * GPIO, int Pin, GPIOMode_TypeDef Mode);

/** @brief Init a GPIO in an Out Push Pull Mode (LED use for example)
	* @param GPIO: GPIOA or GPIOB
	* @param Pin: Pin adress (like GPIO_Pin_6)
	* @retval None
*/
void Init_GPIO_Out(GPIO_TypeDef * GPIO, int Pin);

/** @brief Init a GPIO in Alternate Fonction Push Pull Mode (PWM use for example)
	* @param GPIO: GPIOA or GPIOB
	* @param Pin: Pin adress (like GPIO_Pin_6)
	* @retval None
*/
void Init_GPIO_PWM(GPIO_TypeDef * GPIO, int Pin);

/** @brief Toggle an Output GPIO
	* @param GPIO: GPIOA or GPIOB
	* @param Pin: Pin adress (like GPIO_Pin_6)
	* @retval None
*/
void Toggle_LED(GPIO_TypeDef * GPIO, int Pin);

#endif
