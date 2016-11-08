#ifndef __PWM__
#define __PWM__

#include "stm32f10x.h"

/** @brief Init a a Timer to run at 1kHZ
	*	@parma TIM: TIMx to be use
  * @retval None
*/
void Init_Timer(TIM_TypeDef* TIM);
 
/** @brief Init a PWM on a TIMER
	*	@parma TIM: TIMx to be use
	*	@param Channel: select the right channel (ex. TIM_Channel_3)
  * @retval None
*/
void Init_PWM(TIM_TypeDef* TIM, int Channel);

/** @brief Change the duty cycle of a PWM
	*	@parma TIM: TIMx to be set
	*	@param DutyCycle: has to be from 0 to 1
	*	@param Channel: select the right channel (ex. TIM_Channel_3)
  * @retval None
*/
void Set_PWM_DutyCycle(TIM_TypeDef* TIM, float DutyCycle, int Channel); // TODO
#endif
