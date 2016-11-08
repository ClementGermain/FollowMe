#ifndef __PWM__
#define __PWM__

#include "include.h"

// define period timers
extern int AUTORELOAD_VALUE_TIM1;
extern int AUTORELOAD_VALUE_TIM2;
extern int AUTORELOAD_VALUE_TIM3;
extern int AUTORELOAD_VALUE_TIM4;

/** @brief Init a a Timer to run at 1kHZ
	*	@parma TIM: TIMx to be use
	* @parma Frequency: Frequency to set the Timer
  * @retval None
*/
void Init_Timer(TIM_TypeDef* TIM, int Frequency);
 
/** @brief Init a PWM on a TIMER
	*	@parma TIM: TIMx to be use
	*	@param Channel: select the right channel (ex. TIM_Channel_3)
  * @retval None
*/
void Init_PWM(TIM_TypeDef* TIM, int Channel);

/** @brief Change the duty cycle of a PWM
	*	@parma TIM: TIMx to be set
	*	@param Channel: select the right channel (ex. TIM_Channel_3)
	*	@param DutyCycle: has to be from 0 to 1
  * @retval None
*/
void Set_PWM_DutyCycle(TIM_TypeDef* TIM, int Channel, float DutyCycle); // TODO
#endif
