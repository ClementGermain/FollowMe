#ifndef __PWM__
#define __PWM__

#include "stm32f10x.h"

/** @brief Init a a Timer to run at 1kHZ
*		@parma TIM : TIMx to be use
  * @retval None
*/
int InitializeTimer_1kHz(TIM_TypeDef* TIM);
 
/** @brief Init a PWM on a TIMER
*		@parma TIM : TIMx to be use
* 	@param DutyCycle has to be from 0 to 1
  * @retval None
*/
int InitializePWM(TIM_TypeDef* TIM, float DutyCycle);

#endif
