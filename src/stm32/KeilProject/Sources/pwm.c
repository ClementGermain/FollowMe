#include "pwm.h"

int AUTORELOAD_VALUE_TIM1 = 0;
int AUTORELOAD_VALUE_TIM2 = 0;
int AUTORELOAD_VALUE_TIM3 = 0;
int AUTORELOAD_VALUE_TIM4 = 0;

void Init_Timer(TIM_TypeDef* TIM, int Frequency){

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	int AutoReload_Value_Calcul = 72000000 /(2* Frequency);
	
	if (TIM == TIM1)
		AUTORELOAD_VALUE_TIM1 = AutoReload_Value_Calcul;
	else if (TIM == TIM2)
		AUTORELOAD_VALUE_TIM2 = AutoReload_Value_Calcul;
	else if (TIM == TIM3)
		AUTORELOAD_VALUE_TIM3 = AutoReload_Value_Calcul;
	else if (TIM == TIM4)
		AUTORELOAD_VALUE_TIM4 = AutoReload_Value_Calcul;
	
  TIM_TimeBaseStructure.TIM_Period = AutoReload_Value_Calcul - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);
}

void Init_PWM(TIM_TypeDef* TIM, int Channel)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	switch(Channel){
		case TIM_Channel_1:
			TIM_OC1Init(TIM, &TIM_OCInitStructure); 
			TIM_OC1PreloadConfig(TIM, TIM_OCPreload_Enable);
		case TIM_Channel_2:
			TIM_OC2Init(TIM, &TIM_OCInitStructure); 
			TIM_OC2PreloadConfig(TIM, TIM_OCPreload_Enable);
		case TIM_Channel_3:
			TIM_OC3Init(TIM, &TIM_OCInitStructure); 
			TIM_OC3PreloadConfig(TIM, TIM_OCPreload_Enable);
		case TIM_Channel_4:
			TIM_OC4Init(TIM, &TIM_OCInitStructure); 
			TIM_OC4PreloadConfig(TIM, TIM_OCPreload_Enable);
	}
	
  TIM_ARRPreloadConfig(TIM, ENABLE);
  
  /* TIM enable counter */
  TIM_Cmd(TIM, ENABLE);
}


void Set_PWM_DutyCycle(TIM_TypeDef* TIM, int Channel, float DutyCycle){	
	int period = 0;
	if (TIM == TIM1)
		period = AUTORELOAD_VALUE_TIM1;
	else if (TIM == TIM2)
		period = AUTORELOAD_VALUE_TIM2;
	else if (TIM == TIM3)
		period = AUTORELOAD_VALUE_TIM3;
	else if (TIM == TIM4)
		period = AUTORELOAD_VALUE_TIM4;
	
	uint16_t CCR = (uint16_t) ( ((float) period) * DutyCycle);
	
	switch (Channel){
		case TIM_Channel_1:
			TIM->CCR1 = CCR;
			break;
		case TIM_Channel_2:
			TIM->CCR2 = CCR;
			break;
		case TIM_Channel_3:
			TIM->CCR3 = CCR;
			break;
		case TIM_Channel_4:
			TIM->CCR4 = CCR;
			break;
	}
}
