#include "us_sensor.h"
#include "stm32f10x_tim.h"

//promis je clean le code des qu'il fonctionnera 0:) (LEA)


// Create the US_Sensor Structures
US_Sensor_Typedef SENSOR_FRONT_L_	= {GPIO_SENSOR_TRIG_FRONT_L, GPIO_PIN_SENSOR_TRIG_FRONT_L, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};
US_Sensor_Typedef SENSOR_FRONT_R_	= {GPIO_SENSOR_TRIG_FRONT_R, GPIO_PIN_SENSOR_TRIG_FRONT_R, GPIO_SENSOR_ECHO_FRONT_R, GPIO_PIN_SENSOR_ECHO_FRONT_R};
US_Sensor_Typedef SENSOR_FRONT_C_	= {GPIO_SENSOR_TRIG_FRONT_C, GPIO_PIN_SENSOR_TRIG_FRONT_C, GPIO_SENSOR_ECHO_FRONT_C, GPIO_PIN_SENSOR_ECHO_FRONT_C};
US_Sensor_Typedef SENSOR_BACK_L_	= {GPIO_SENSOR_TRIG_BACK_L, GPIO_PIN_SENSOR_TRIG_BACK_L, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};
US_Sensor_Typedef SENSOR_BACK_R_ 	= {GPIO_SENSOR_TRIG_BACK_R, GPIO_PIN_SENSOR_TRIG_BACK_R, GPIO_SENSOR_ECHO_FRONT_R, GPIO_PIN_SENSOR_ECHO_FRONT_R};
US_Sensor_Typedef SENSOR_BACK_C_	= {GPIO_SENSOR_TRIG_BACK_C, GPIO_PIN_SENSOR_TRIG_BACK_C, GPIO_SENSOR_ECHO_FRONT_C, GPIO_PIN_SENSOR_ECHO_FRONT_C};

// Create the US_Sensor pointeur's structures
US_Sensor_Typedef * SENSOR_FRONT_L 	= &SENSOR_FRONT_L_;
US_Sensor_Typedef * SENSOR_FRONT_R 	= &SENSOR_FRONT_R_;
US_Sensor_Typedef * SENSOR_FRONT_C 	= &SENSOR_FRONT_C_;
US_Sensor_Typedef * SENSOR_BACK_L 	= &SENSOR_BACK_L_;
US_Sensor_Typedef * SENSOR_BACK_R 	= &SENSOR_BACK_R_;
US_Sensor_Typedef * SENSOR_BACK_C 	= &SENSOR_BACK_C_;
US_Sensor_Typedef * US_active;


//Global time (ms)
int Time;
int front_us;
int time_echo;

void Update_US_Sensor(BarstowModel_Typedef * Modele){
	
	Modele->frontRightUSensor.distance 	= (Get_USensor(SENSOR_FRONT_R));
	Modele->frontLeftUSensor.distance 	= (Get_USensor(SENSOR_FRONT_L));
	Modele->frontCenterUSensor.distance = (Get_USensor(SENSOR_FRONT_C));
	Modele->rearRightUSensor.distance 	= (Get_USensor(SENSOR_BACK_R));
	Modele->rearLeftUSensor.distance 		= (Get_USensor(SENSOR_BACK_L));
	Modele->rearCenterUSensor.distance 	= (Get_USensor(SENSOR_BACK_C));
}
// configure the pins TRIGG and ECHO in output and input  
void Init_US_Sensor(US_Sensor_Typedef * Sensor){
		Init_GPIO_IPU(Sensor->GPIO_Echo, Sensor->GPIO_Pin_Echo);
		Init_GPIO_Out(Sensor->GPIO_Trig, Sensor->GPIO_Pin_Trig);
}

void Init_All_US_Sensor(void){
	Init_US_Sensor(SENSOR_FRONT_L);
	Init_US_Sensor(SENSOR_FRONT_R);
	Init_US_Sensor(SENSOR_FRONT_C);
//	Init_US_Sensor(SENSOR_BACK_L);
//	Init_US_Sensor(SENSOR_BACK_R);
//	Init_US_Sensor(SENSOR_BACK_C);
	
	/*** Init timer in count mode ***/
	TIM_TimeBaseInitTypeDef timerInit;
		timerInit.TIM_Period = 0xFFFF;
		timerInit.TIM_Prescaler = 0x0000;
		timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
		timerInit.TIM_CounterMode = TIM_CounterMode_Up;
		timerInit.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM_Echo, &timerInit);

	/*** Init Exti and NVIC ***/
	Config_EXTI_Rising_Falling(EXTI_Line0);	//config EXTI
	Config_NVIC_EXTI(EXTI_Line0); 					//config NVIC pour EXTI
	Config_NVIC_TIM2(); 										//config NVIC pour TIM2
}


float Init_Systick(void){
	Time=0;
	float period; //period systick us
	period=Systick_Period(SYSTICK_PERIOD_US);
	Systick_Prio_IT(6,Periodic_Impulse_3_Front_US);
	SysTick_On;
	SysTick_Enable_IT;
	return period;
}


// interrupt function : tous les 2 fronts, on regarde la valeur du compteur et on MAJ le modèle


/*void Capture_echo(void) {
	//front_us++;
	//if (front_us==2){
		time_echo = TIM_GetCounter(TIM_Echo);
		Reset_counter(TIM_Echo);
	//front_us=0;
	//}
	
	if (time_echo!=0){ //on ne met à jour le modèle que si time_echo a une valeur correcte
		if (US_active == SENSOR_FRONT_L){
			Modele->frontLeftUSensor.distance = (time_echo/58);
		}
		else if (US_active == SENSOR_FRONT_C){
			Modele->frontCenterUSensor.distance = (time_echo/58);
		}
		else if (US_active == SENSOR_FRONT_R){
			Modele->frontRightUSensor.distance = (time_echo/58);
		}
	}
	time_echo=0;
}*/


//réinitialise le compteur et le lance
void Relance_Compteur_Echo(void){
	TIM_Cmd(TIM_Echo, ENABLE);
}


void Capture_echo(void) {
	
		//capturer le compteur, reset et l'arrêter
		time_echo = TIM_GetCounter(TIM_Echo);
		Reset_counter(TIM_Echo);
		TIM_Cmd(TIM2, DISABLE);

	if (time_echo!=0){ //on ne met à jour le modèle que si time_echo a une valeur correcte --> voir si il faut mettre time_echo>=58 (éq à 1cm) environ
		if (US_active == SENSOR_FRONT_L){
			//Modele->frontLeftUSensor.distance = (time_echo/58);
		}
		else if (US_active == SENSOR_FRONT_C){
			//Modele->frontCenterUSensor.distance = (time_echo/58);
		}
		else if (US_active == SENSOR_FRONT_R){
			//Modele->frontRightUSensor.distance = (time_echo/58);
		}
	}
	time_echo=0;
}

	
	void Test_Get_USensor(void) {
	/*front_us=0;
	
	if (US_active == SENSOR_FRONT_L){
		Init_Channel_trigger(TIM_Echo, TIM_Channel_Echo_Front_L);
	}
	else if (US_active == SENSOR_FRONT_C){
		Init_Channel_trigger(TIM_Echo, TIM_Channel_Echo_Front_C);
	}
	else if (US_active == SENSOR_FRONT_R){
		Init_Channel_trigger(TIM_Echo, TIM_Channel_Echo_Front_R);
	}
	
	// Configure IT with My function in us_sensor.c
	Timer_Active_IT( TIM_Echo	,5, Capture_echo);
	*/}

	//on s'en sert pas
uint32_t Get_USensor(US_Sensor_Typedef * Sensor){	
	int distance =0;
	//distance = time_echo/58; //cm
		return distance;
}

void Periodic_Impulse_3_Front_US(){
	Time++;

	if (Time%210==10){
	
		/*
	if (GPIO_SENSOR_ECHO_FRONT_L==GPIOA){
		if (GPIO_PIN_SENSOR_ECHO_FRONT_L == GPIO_Pin_0){
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, 0);
		}
	}
		*/
		
	//rajouter les autres cas
	
	//impulse 10us on Front Left US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_L, GPIO_PIN_SENSOR_TRIG_FRONT_L, 10);
	//US_active = SENSOR_FRONT_L;

		
	//Timer_Active_IT( TIM_Echo	,5, Capture_echo);
	//Init_Channel_trigger(TIM_Echo, TIM_Channel_Echo_Front_L);

	
	}
/* Quand le premier US marchera il faudra décommenter ici pour les 2 autres
	
	else if (Time%210==80){
	//impulse 10us on Front Right US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_R, GPIO_PIN_SENSOR_TRIG_FRONT_R, 10);
	//US_active = SENSOR_FRONT_R;
	Init_Channel_trigger(TIM_Echo, TIM_Channel_Echo_Front_R);
	Timer_Active_IT( TIM_Echo	,5, Capture_echo);
	}

	else if (Time%210==150){
	//impulse 10us on Front Center US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_C, GPIO_PIN_SENSOR_TRIG_FRONT_C, 10);
	//US_active = SENSOR_FRONT_C;
	Init_Channel_trigger(TIM_Echo, TIM_Channel_Echo_Front_C);
	Timer_Active_IT( TIM_Echo	,5, Capture_echo);
	}
	*/
}

void Test_US_Sensor(void){
	Init_All_US_Sensor();
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, 0);
	Init_Systick();
}
