#include "pwm.h"

int InitializeTimer_1kHz(TIM_TypeDef* TIM){

	int PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
  TIM_TimeBaseStructure.TIM_Period = 11250;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);
	
	return 1;
}

int InitializePWM(TIM_TypeDef* TIM, float RapportCycl)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = (int) ( ((float) 11250) * RapportCycl);
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
 
  TIM_OC1Init(TIM, &TIM_OCInitStructure); 
  TIM_OC1PreloadConfig(TIM, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM, ENABLE);
  
  /* TIM enable counter */
  TIM_Cmd(TIM, ENABLE);
	
	return 1;
}
