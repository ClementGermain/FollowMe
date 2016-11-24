#include "us_sensor.h"


// Create the US_Sensor Structures
US_Sensor_Typedef SENSOR_FRONT_L_	= {GPIO_SENSOR_TRIG_FRONT_L, GPIO_PIN_SENSOR_TRIG_FRONT_L, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};
US_Sensor_Typedef SENSOR_FRONT_R_	= {GPIO_SENSOR_TRIG_FRONT_R, GPIO_PIN_SENSOR_TRIG_FRONT_R, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};
US_Sensor_Typedef SENSOR_FRONT_C_	= {GPIO_SENSOR_TRIG_FRONT_C, GPIO_PIN_SENSOR_TRIG_FRONT_C, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};
US_Sensor_Typedef SENSOR_BACK_L_	= {GPIO_SENSOR_TRIG_BACK_L, GPIO_PIN_SENSOR_TRIG_BACK_L, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};
US_Sensor_Typedef SENSOR_BACK_R_ 	= {GPIO_SENSOR_TRIG_BACK_R, GPIO_PIN_SENSOR_TRIG_BACK_R, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};
US_Sensor_Typedef SENSOR_BACK_C_	= {GPIO_SENSOR_TRIG_BACK_C, GPIO_PIN_SENSOR_TRIG_BACK_C, GPIO_SENSOR_ECHO_FRONT_L, GPIO_PIN_SENSOR_ECHO_FRONT_L};

// Create the US_Sensor pointeur's structures
US_Sensor_Typedef * SENSOR_FRONT_L 	= &SENSOR_FRONT_L_;
US_Sensor_Typedef * SENSOR_FRONT_R 	= &SENSOR_FRONT_R_;
US_Sensor_Typedef * SENSOR_FRONT_C 	= &SENSOR_FRONT_C_;
US_Sensor_Typedef * SENSOR_BACK_L 	= &SENSOR_BACK_L_;
US_Sensor_Typedef * SENSOR_BACK_R 	= &SENSOR_BACK_R_;
US_Sensor_Typedef * SENSOR_BACK_C 	= &SENSOR_BACK_C_;
US_Sensor_Typedef * US_active;

BarstowModel_Typedef * Modele;


//Global time (ms)
int Time;
int front_us =0;
int time_echo =0;

void Update_US_Sensor(BarstowModel_Typedef * Modele){
	
	Modele->frontRightUSensor.distance = (Get_USensor(SENSOR_FRONT_R));
	Modele->frontLeftUSensor.distance = (Get_USensor(SENSOR_FRONT_L));
	Modele->frontCenterUSensor.distance = (Get_USensor(SENSOR_FRONT_C));
	Modele->rearRightUSensor.distance = (Get_USensor(SENSOR_BACK_R));
	Modele->rearLeftUSensor.distance = (Get_USensor(SENSOR_BACK_L));
	Modele->rearCenterUSensor.distance = (Get_USensor(SENSOR_BACK_C));
}
// configure the pins TRIGG and ECHO in output and input  
void Init_US_Sensor(US_Sensor_Typedef * Sensor){
	if (Sensor == SENSOR_FRONT_L){
		//Init_GPIO_In(SENSOR_FRONT_L->GPIO_Echo, SENSOR_FRONT_L->GPIO_Pin_Echo);
		Init_GPIO_Out(SENSOR_FRONT_L->GPIO_Trig, SENSOR_FRONT_L->GPIO_Pin_Trig);
	}
	else if (Sensor ==  SENSOR_FRONT_R){
		//Init_GPIO_In(SENSOR_FRONT_R->GPIO_Echo, SENSOR_FRONT_R->GPIO_Pin_Echo);
		Init_GPIO_Out(SENSOR_FRONT_R->GPIO_Trig, SENSOR_FRONT_R->GPIO_Pin_Trig);
	}
	else if (Sensor == SENSOR_FRONT_C){
		//Init_GPIO_In(SENSOR_FRONT_C->GPIO_Echo, SENSOR_FRONT_C->GPIO_Pin_Echo);
		Init_GPIO_Out(SENSOR_FRONT_C->GPIO_Trig, SENSOR_FRONT_C->GPIO_Pin_Trig);	
	}
	else if (Sensor == SENSOR_BACK_L){
		Init_GPIO_In(SENSOR_BACK_L->GPIO_Echo, SENSOR_BACK_L->GPIO_Pin_Echo);
		//Init_GPIO_Out(SENSOR_BACK_L->GPIO_Trig, SENSOR_BACK_L->GPIO_Pin_Trig);
	}
	else if (Sensor == SENSOR_BACK_R){
		Init_GPIO_In(SENSOR_BACK_R->GPIO_Echo, SENSOR_BACK_R->GPIO_Pin_Echo);
		//Init_GPIO_Out(SENSOR_BACK_R->GPIO_Trig, SENSOR_BACK_R->GPIO_Pin_Trig);
	}
	else if (Sensor == SENSOR_BACK_C){
		Init_GPIO_In(SENSOR_BACK_C->GPIO_Echo, SENSOR_BACK_C->GPIO_Pin_Echo);
		//Init_GPIO_Out(SENSOR_BACK_C->GPIO_Trig, SENSOR_BACK_C->GPIO_Pin_Trig);
	}
	// Init timer 2  in gated mode
	Init_Gated_mode(TIM_Echo);
}

void Init_All_US_Sensor(void){
	Init_US_Sensor(SENSOR_FRONT_L);
	Init_US_Sensor(SENSOR_FRONT_R);
	Init_US_Sensor(SENSOR_FRONT_C);
//	Init_US_Sensor(SENSOR_BACK_L);
//	Init_US_Sensor(SENSOR_BACK_R);
//	Init_US_Sensor(SENSOR_BACK_C);
}


float Init_Systick(void){
	Time=0;
	float period; //period systick us
	period=Systick_Period(SYSTICK_PERIOD_US);
	Systick_Prio_IT(2,Periodic_Impulse_3_Front_US);
	SysTick_On;
	SysTick_Enable_IT;
	return period;
}


// interrupt function : tous les 2 fronts, on regarde la valeur du compteur et on MAJ le mod�le
void Capture_echo(void) {
	front_us++;
	if (front_us==2){
		time_echo = TIM_GetCounter(TIM_Echo);
		Reset_counter(TIM_Echo);
		front_us=0;
	}
	
	if (time_echo!=0){ //on ne met � jour le mod�le que si time_echo a une valeur correcte
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
}

	
	void Test_Get_USensor(void) {
	front_us=0;
	
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
	}

uint32_t Get_USensor(US_Sensor_Typedef * Sensor){	
	int distance =0;
	//distance = time_echo/58; //cm
		return distance;
}

void Periodic_Impulse_3_Front_US(){
	Time++;

	if (Time%210==10){
	//impulse 10us on Front Left US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_L, GPIO_PIN_SENSOR_TRIG_FRONT_L, 10);
	US_active = SENSOR_FRONT_L;
	}

	else if (Time%210==80){
	//impulse 10us on Front Right US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_R, GPIO_PIN_SENSOR_TRIG_FRONT_R, 10);
	US_active = SENSOR_FRONT_R;
	}

	else if (Time%210==150){
	//impulse 10us on Front Center US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_C, GPIO_PIN_SENSOR_TRIG_FRONT_C, 10);
	US_active = SENSOR_FRONT_C;
	}
}

void Test_US_Sensor(void){
	Init_All_US_Sensor();
	Init_Systick();
	Test_Get_USensor();
}



