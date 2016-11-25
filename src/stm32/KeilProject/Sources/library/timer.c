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
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
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
/*	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	int ARR_max=65535;
//	int fclk=72; // clock = 72 Mhz
	int PSC =72;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	
  TIM_TimeBaseStructure.TIM_Period = ARR_max - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = PSC ;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);
	TIM_SelectSlaveMode(TIM,TIM_SlaveMode_Gated); // SMS
	TIM_SelectInputTrigger(TIM, TIM_TS_TI1FP1); // TS -> internal trigger 1
	
	 // Set the default configuration
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = TIM_TS_TI1FP1;
	
	TIM_ICInit(TIM,&TIM_ICInitStructure); //CC1P=0; CC1S = input capture source = 001 
	TIM_Cmd(TIM, ENABLE); //CEN
	*/
}

void Init_Gated_mode(TIM_TypeDef* TIM){
/*
	//Init_Timer(TIM,1000000);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	int ARR_max=65535;
//	int fclk=72; // clock = 72 Mhz
	int PSC =72;
	
  TIM_TimeBaseStructure.TIM_Period = ARR_max - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = PSC ;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM, &TIM_TimeBaseStructure);
	TIM_SelectSlaveMode(TIM,TIM_SlaveMode_Gated); // SMS
	*/
}


void Init_Channel_trigger(TIM_TypeDef* TIM, u8 num_Channel) {
TIM_ICInitTypeDef TIM_ICInitStructure;
	
	if (num_Channel ==TIM_Channel_1) {
			
		 //clear UIF bit (?)
		 TIM->SR = TIM->SR & ~ TIM_SR_UIF;

	 /* Set the default configuration */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;

TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = 	TIM_ICSelection_TRC ;
		//TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;

  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  //TIM_ICInitStructure.TIM_ICFilter = TIM_TS_TI1FP1;
	TIM_ICInitStructure.TIM_ICFilter = TIM_TS_ETRF ;
	TIM_ICInit(TIM,&TIM_ICInitStructure); //CC1P=0; CC1S = input capture source = 001 

	TIM_SelectInputTrigger(TIM, TIM_TS_ETRF ); // TS -> external trigger

	TIM_Cmd(TIM, ENABLE); //CEN
	}
	
	 else if (num_Channel ==TIM_Channel_2) {
	TIM_SelectInputTrigger(TIM, TIM_TS_TI1FP1); // TS -> internal trigger 1
	
	 /* Set the default configuration */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		 		 TIM_ICInitStructure.TIM_ICSelection = 	TIM_ICSelection_TRC ;
		 //TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = TIM_TS_TI2FP2;
	
	TIM_ICInit(TIM,&TIM_ICInitStructure); //CC1P=0; CC1S = input capture source = 001 
	TIM_Cmd(TIM, ENABLE); //CEN
	}
	 
	 else if (num_Channel ==TIM_Channel_3) {
	TIM_SelectInputTrigger(TIM, TIM_TS_TI1FP1); // TS -> internal trigger 1
	
	 /* Set the default configuration */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
 TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = 	TIM_ICSelection_TRC ;
		 //TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = TIM_TS_TI1F_ED; // to check
	
	TIM_ICInit(TIM,&TIM_ICInitStructure); //CC1P=0; CC1S = input capture source = 001 
	TIM_Cmd(TIM, ENABLE); //CEN
 
	}
	
	
	
} 


//function pointers
void (* pFnc2) (void);
void (* pFnc3) (void);
void (* pFnc4) (void);

void TIM2_IRQHandler (void)
{
    TIM2->SR = TIM2->SR &~ 0x40; // RaZ du flag d'interruption
    pFnc2(); // exécution de la fonction lors d'une interruption
}

void TIM3_IRQHandler (void)
{
    TIM3->SR = TIM3->SR &~ 0x40; // RaZ du flag d'interruption
    pFnc3();
}

void TIM4_IRQHandler (void)
{
    TIM4->SR = TIM4->SR &~ 0x40; // RaZ du flag d'interruption
    pFnc4();
}



// configure interruption  FLAG TIF to detect an edge on the trigger

void Timer_Active_IT( TIM_TypeDef *TIM, u8 Priority, void (*IT_function) (void)){
	/*
	NVIC_InitTypeDef		NVIC_InitStructure;
	
	// TIF interupt configuration at the NVIC LEVEL + priority
	if (TIM == TIM2)
	{
		NVIC->IP[28] = Priority << 4; //to reach the modifiable bits
		pFnc2 = IT_function; // function made by the handler during the interrupt
		NVIC->ISER[0] = NVIC->ISER[0] | (0x1 << 28);
	}
	else if (TIM == TIM3)
	{
		NVIC->IP[29] = Priority << 4;
		pFnc3 = IT_function;
		NVIC->ISER[0] = NVIC->ISER[0] | (0x1 << 29);
	  //AFIO->EXTICR1 =  AFIO_EXTICR1_EXTI0 ;
	}
	else if (TIM == TIM4)
	{
		NVIC->IP[30] = Priority << 4;
		pFnc4 = IT_function;
		NVIC->ISER[0] = NVIC->ISER[0] | (0x1 << 30);
	}
	
	// configuration de l'interruption overflow du timer (enable TIF)
	TIM->DIER = 0x40 | TIM->DIER ;
	
//	TIM->DIER = TIM_IT_Trigger;
	
	//TIM_ITConfig(TIM, TIM_IT_Trigger, ENABLE);
	

		//enable TIM2 interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/

	
}
	
void Config_EXTI_Rising(uint32_t EXTI_Line){
	EXTI_InitTypeDef		EXTI_InitStructure;
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

 void Config_EXTI_Falling(uint32_t EXTI_Line){
	EXTI_InitTypeDef		EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
 void Config_EXTI_Rising_Falling(uint32_t EXTI_Line){
	EXTI_InitTypeDef		EXTI_InitStructure;
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void Config_NVIC_EXTI(uint32_t EXTI_Line){
	NVIC_InitTypeDef		NVIC_InitStructure;
	
	//nvic : external Trigger
	if (EXTI_Line == EXTI_Line0){
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	}
	//FAIRE LES AUTRES CAS
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Config_NVIC_TIM2(){
	
	NVIC_InitTypeDef		NVIC_InitStructure;

	//NVIC : TIM interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Reset_counter(TIM_TypeDef *TIM){
	TIM_SetCounter(TIM,0);
}

