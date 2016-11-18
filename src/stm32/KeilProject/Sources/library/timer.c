#include "timer.h"

int AUTORELOAD_VALUE_TIM1 = 0;
int AUTORELOAD_VALUE_TIM2 = 0;
int AUTORELOAD_VALUE_TIM3 = 0;
int AUTORELOAD_VALUE_TIM4 = 0;

void Init_Timer(TIM_TypeDef* TIM, uint16_t Frequency){

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


void Timer_Configure(TIM_TypeDef* TIM, uint16_t Duree_us){
	int ARR_max=65535;
	int fclk=72; // clock = 72 Mhz
	int PSC_calc;
	int ARR_calc;
	//int duree_reelle_us;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	// DeInit
	TIM_DeInit(TIM);
	
	PSC_calc = (Duree_us * fclk / ARR_max) +1 ;	// +1 pour arrondi;
	ARR_calc= (Duree_us * fclk /PSC_calc) +1;
		// 
	TIM_TimeBaseStructure.TIM_Period = ARR_calc -1  ;
  TIM_TimeBaseStructure.TIM_Prescaler = PSC_calc -1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// clock and Init 
	 TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);
	 /* TIM enable counter */
  TIM_Cmd(TIM, ENABLE);
}

void Init_PWM(TIM_TypeDef* TIM, uint16_t Channel)
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


void Set_PWM_DutyCycle(TIM_TypeDef* TIM, uint16_t Channel, float DutyCycle){	
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

void Init_timer_Gated_mode(TIM_TypeDef* TIM){
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	int ARR_max=65535;
	int fclk=72; // clock = 72 Mhz
	int PSC =72;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	
  TIM_TimeBaseStructure.TIM_Period = ARR_max - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = PSC ;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);
	TIM_SelectSlaveMode(TIM,TIM_SlaveMode_Gated); // SMS
	TIM_SelectInputTrigger(TIM, TIM_TS_TI1FP1); // TS -> internal trigger 1
	
	 /* Set the default configuration */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = TIM_TS_TI1FP1;
	
	TIM_ICInit(TIM,&TIM_ICInitStructure);
	 //TIM_EncoderInterfaceConfig(TIM, TIM_EncoderMode_TI1,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//Counter counts on TI1FP1 edge depending on TI2FP2 level.
	 /* TIM enable counter */
  TIM_Cmd(TIM, ENABLE); //CEN
}




// configure interruption trigger

void Configure_Interruption(TIM_TypeDef* TIMx){
/*	TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource);
TIM_ITConfig(TIM, TIM_IT_CC1, ENABLE); // we want an IT when we are on the falling edge, to prevent  we meed to take the result
*/	
	
}

