#include "us_sensor.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"

//promis je clean le code des qu'il fonctionnera 0:) (LEA)

// variable globale qui change � chaque passage dans le handler
enum TRIG_STATE {FALLING,RISING};
enum TRIG_STATE Trigger_State;

BarstowModel_Typedef * Model;

// Create the US_Sensor Structures
US_Sensor_Typedef SENSOR_FRONT_L_	= {GPIO_SENSOR_TRIG_FRONT_L, GPIO_PIN_SENSOR_TRIG_FRONT_L, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};
US_Sensor_Typedef SENSOR_FRONT_R_	= {GPIO_SENSOR_TRIG_FRONT_R, GPIO_PIN_SENSOR_TRIG_FRONT_R, GPIO_SENSOR_ECHO_FRONT_R, GPIO_PIN_SENSOR_ECHO_FRONT_R};
US_Sensor_Typedef SENSOR_FRONT_C_	= {GPIO_SENSOR_TRIG_FRONT_C, GPIO_PIN_SENSOR_TRIG_FRONT_C, GPIO_SENSOR_ECHO_FRONT_C, GPIO_PIN_SENSOR_ECHO_FRONT_C};

// Create the US_Sensor pointeur's structures
US_Sensor_Typedef * SENSOR_FRONT_L 	= &SENSOR_FRONT_L_;
US_Sensor_Typedef * SENSOR_FRONT_R 	= &SENSOR_FRONT_R_;
US_Sensor_Typedef * SENSOR_FRONT_C 	= &SENSOR_FRONT_C_;
US_Sensor_Typedef * US_active;


//Global time (ms)
int Time;
int front_us;
int time_echo;

void Update_US_Sensor(BarstowModel_Typedef * Modele){
	
	Modele->frontRightUSensor.distance 	= (Get_USensor(SENSOR_FRONT_R));
	Modele->frontLeftUSensor.distance 	= (Get_USensor(SENSOR_FRONT_L));
	Modele->frontCenterUSensor.distance = (Get_USensor(SENSOR_FRONT_C));
}

// configure the pins TRIGG and ECHO in output and input  
void Init_US_Sensor(US_Sensor_Typedef * Sensor){
		Init_GPIO_IPU(Sensor->GPIO_Echo, Sensor->GPIO_Pin_Echo);
		Init_GPIO_Out(Sensor->GPIO_Trig, Sensor->GPIO_Pin_Trig); // Validate by Clement
}

void Init_All_US_Sensor(void){
	Init_US_Sensor(SENSOR_FRONT_L);
	Init_US_Sensor(SENSOR_FRONT_R);
	Init_US_Sensor(SENSOR_FRONT_C);
	
	/*** Init timer in count mode ***/

	TIM_TimeBaseInitTypeDef timerInit;
	timerInit.TIM_Period = 0xFFFF;
	timerInit.TIM_Prescaler = 0x0072;
	timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerInit.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM_Echo, &timerInit);
//init interrupt
	//Config_NVIC_TIM2(); //config NVIC pour TIM2

	//Configuration de l'external trigger
	
	Config_EXTI_Rising_Falling(EXTI_Line0);	//config EXTI
	Config_NVIC_EXTI(EXTI_Line0); //config NVIC pour EXTI
		
	Config_EXTI_Rising_Falling(EXTI_Line1);	//config EXTI
	Config_NVIC_EXTI(EXTI_Line1); //config NVIC pour EXTI
		
	Config_EXTI_Rising_Falling(EXTI_Line2);	//config EXTI
	Config_NVIC_EXTI(EXTI_Line2); //config NVIC pour EXTI
		
	GPIO_EXTILineConfig(GPIO_Port_Source_Echo_Front_L, GPIO_Num_Port_Echo_Front_L);	
	GPIO_EXTILineConfig(GPIO_Port_Source_Echo_Front_R, GPIO_Num_Port_Echo_Front_R);
	GPIO_EXTILineConfig(GPIO_Port_Source_Echo_Front_C, GPIO_Num_Port_Echo_Front_C);
}

float Init_Systick(void){
	Time=0;
	float period; //period systick us
	period=Systick_Period(SYSTICK_PERIOD_US);
	Systick_Prio_IT(6,&Periodic_Impulse_3_Front_US);
	//NVIC_SetPriority (SysTick_IRQn, 7);	
	SysTick_On;
	SysTick_Enable_IT;
	return period;
}

//relance le compteur
void Relance_Compteur_Echo(void){
	TIM_Cmd(TIM_Echo, ENABLE);
}

void Capture_echo(void) {
		//capturer le compteur, reset et l'arr�ter
		time_echo = TIM_GetCounter(TIM_Echo);
		TIM_Cmd(TIM_Echo, DISABLE);	
		Reset_counter(TIM_Echo);

	if (time_echo!=0){ //on ne met � jour le mod�le que si time_echo a une valeur correcte --> voir si il faut mettre time_echo>=58 (�q � 1cm) environ
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

	//on s'en sert pas
uint32_t Get_USensor(US_Sensor_Typedef * Sensor){	
	int distance =0;
	//distance = time_echo/58; //cm
		return distance;
}

void Periodic_Impulse_3_Front_US(){
	Time++;

	if (Time%210==10){

	//impulse >10us on Front Left US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_L, GPIO_PIN_SENSOR_TRIG_FRONT_L, 12);
	US_active = SENSOR_FRONT_L;
	}
	
	else if (Time%210==80){
	//impulse 10us on Front Right US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_R, GPIO_PIN_SENSOR_TRIG_FRONT_R, 12);
	US_active = SENSOR_FRONT_R;
	}

	else if (Time%210==150){
	//impulse 10us on Front Center US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_C, GPIO_PIN_SENSOR_TRIG_FRONT_C, 12);
	US_active = SENSOR_FRONT_C;
	}
}

void Start_US_Sensor(){
	//Model = mod;
	Init_Systick();
	Init_All_US_Sensor();
}


void EXTI0_IRQHandler(void) {
			//if ((GPIOA->IDR & 0x0001) == 0x0001) {
	GPIO_TypeDef* GPIO;
	uint16_t Pin;
	uint32_t line;
		
	if (US_active == SENSOR_FRONT_L){
			GPIO = GPIO_SENSOR_ECHO_FRONT_L;
			Pin = GPIO_Num_Port_Echo_Front_L ;
		line=EXTI_Line0;
		}
		else if (US_active == SENSOR_FRONT_C){
			GPIO = GPIO_SENSOR_ECHO_FRONT_C;
			Pin= GPIO_Num_Port_Echo_Front_C;
			line =EXTI_Line1;
			
		}
		else if (US_active == SENSOR_FRONT_R){
			GPIO=GPIO_SENSOR_ECHO_FRONT_R;
			Pin= GPIO_Num_Port_Echo_Front_R;
			line= EXTI_Line2;
			}
				if (GPIO_ReadInputDataBit(GPIO, Pin)) {
				//relance le compteur
				Relance_Compteur_Echo();
			}
			else {
				Capture_echo();
			}
			EXTI_ClearITPendingBit(line);
}
