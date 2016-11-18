#ifndef __PWM__
#define __PWM__

// include stm32 library
#include "stm32f10x_tim.h"

// include local library
#include "gpio.h"

// define period timers
extern int AUTORELOAD_VALUE_TIM1;
extern int AUTORELOAD_VALUE_TIM2;
extern int AUTORELOAD_VALUE_TIM3;
extern int AUTORELOAD_VALUE_TIM4;

/** @brief Init a a Timer to run at 1kHZ
	*	@param TIM: TIMx to be use
	* @param Frequency: Frequency to set the Timer
  * @retval None
*/
void Init_Timer(TIM_TypeDef* TIM, uint16_t Frequency);
 
/** @brief Init a PWM on a TIMER
	*	@param TIM: TIMx to be use
	*	@param Channel: select the right channel (ex. TIM_Channel_3)
  * @retval None
*/
void Init_PWM(TIM_TypeDef* TIM, uint16_t Channel);

/** @brief Change the duty cycle of a PWM
	*	@param TIM:	TIMx to be set
	*	@param Channel: select the right channel (ex. TIM_Channel_3)
	*	@param DutyCycle: has to be from 0 to 1
  * @retval None
*/
void Set_PWM_DutyCycle(TIM_TypeDef* TIM, uint16_t Channel, float DutyCycle); // TODO

/** @brief Configure and active interruption on a TIMER
	*	@param TIM:	TIMx to be set
	*	@param Priority: Priority of the interruption
	*	@param IT_function: adress of the handler
  * @retval None
*/
//void Config_IT( TIM_TypeDef* TIM, u8 Priority,void (*IT_function) (void));

/** @brief Configure a timer to count during Duree_us 
	*	@param TIM:	TIMx to be set, 
	*	@param Duree_us
  * @retval None
*/

void Timer_Configure(TIM_TypeDef* TIM, uint16_t Duree_us);

void Init_timer_Gated_mode(TIM_TypeDef* TIM);


 void Timer_Active_IT( TIM_TypeDef *TIM, u8 Priority, void (*IT_function) (void));

#endif
