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

//Global time (ms)
int Time;

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
			Init_GPIO_In(SENSOR_FRONT_L->GPIO_Echo, SENSOR_FRONT_L->GPIO_Pin_Echo);
			Init_GPIO_Out(SENSOR_FRONT_L->GPIO_Trig, SENSOR_FRONT_L->GPIO_Pin_Trig);
	}
	else if (Sensor ==  SENSOR_FRONT_R){
		Init_GPIO_In(SENSOR_FRONT_R->GPIO_Echo, SENSOR_FRONT_R->GPIO_Pin_Echo);
		Init_GPIO_Out(SENSOR_FRONT_R->GPIO_Trig, SENSOR_FRONT_R->GPIO_Pin_Trig);
	}
	else if (Sensor == SENSOR_FRONT_C){
		Init_GPIO_In(SENSOR_FRONT_C->GPIO_Echo, SENSOR_FRONT_C->GPIO_Pin_Echo);
		Init_GPIO_Out(SENSOR_FRONT_C->GPIO_Trig, SENSOR_FRONT_C->GPIO_Pin_Trig);	
	}
	else if (Sensor == SENSOR_BACK_L){
		Init_GPIO_In(SENSOR_BACK_L->GPIO_Echo, SENSOR_BACK_L->GPIO_Pin_Echo);
		Init_GPIO_Out(SENSOR_BACK_L->GPIO_Trig, SENSOR_BACK_L->GPIO_Pin_Trig);
	}
	else if (Sensor == SENSOR_BACK_R){
		Init_GPIO_In(SENSOR_BACK_R->GPIO_Echo, SENSOR_BACK_R->GPIO_Pin_Echo);
		Init_GPIO_Out(SENSOR_BACK_R->GPIO_Trig, SENSOR_BACK_R->GPIO_Pin_Trig);
	}
	else if (Sensor == SENSOR_BACK_C){
		Init_GPIO_In(SENSOR_BACK_C->GPIO_Echo, SENSOR_BACK_C->GPIO_Pin_Echo);
		Init_GPIO_Out(SENSOR_BACK_C->GPIO_Trig, SENSOR_BACK_C->GPIO_Pin_Trig);
	}
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

// interrupt function

	void My_function_TIF (void) {
		front_us++;
		if (front_us==2){
	time_echo = TIM_GetCounter( TIM_Echo);
	Reset_counter(TIM_Echo);
	front_us=0;}
	
		
	}
	


uint32_t Get_USensor(US_Sensor_Typedef * Sensor){	
	front_us=0;
	uint32_t distance=0;
	// Init timer 2  in gated mode
	Init_timer_Gated_mode( TIM_Echo	);
	// Configure IT with My function in us_sensor.c
	Timer_Active_IT( TIM_Echo	,5, My_function_TIF);
	distance = time_echo/58; //cm
	
	return distance;
}

void Periodic_Impulse_3_Front_US(){
	Time++;

	if (Time%210==10){
	//impulse 10us on Front Left US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_L, GPIO_PIN_SENSOR_TRIG_FRONT_L, 10);
	}

	else if (Time%210==80){
	//impulse 10us on Front Right US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_R, GPIO_PIN_SENSOR_TRIG_FRONT_R, 10);
	}

	else if (Time%210==150){
	//impulse 10us on Front Center US
	Send_impulse_GPIO(GPIO_SENSOR_TRIG_FRONT_C, GPIO_PIN_SENSOR_TRIG_FRONT_C, 10);
	}
}

void Test_US_Sensor(void){
	Init_All_US_Sensor();
	Init_Systick();
}



