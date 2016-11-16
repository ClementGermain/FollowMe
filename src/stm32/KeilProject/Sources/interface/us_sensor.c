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
Init_GPIO_Out( SENSOR_FRONT_L->GPIO_Echo, SENSOR_FRONT_L ->GPIO_Pin_Echo);
Init_GPIO_Out( SENSOR_FRONT_R ->GPIO_Echo, SENSOR_FRONT_R ->GPIO_Pin_Echo);
Init_GPIO_Out( SENSOR_FRONT_C ->GPIO_Echo, SENSOR_FRONT_C ->GPIO_Pin_Echo);
	
Init_GPIO_Out( SENSOR_BACK_L -> GPIO_Echo, SENSOR_BACK_L -> GPIO_Pin_Echo);
Init_GPIO_Out( SENSOR_BACK_R -> GPIO_Echo, SENSOR_BACK_R -> GPIO_Pin_Echo);
Init_GPIO_Out( SENSOR_BACK_C -> GPIO_Echo, SENSOR_BACK_C -> GPIO_Pin_Echo);

Init_GPIO_In( SENSOR_FRONT_L ->GPIO_Trig, SENSOR_FRONT_L ->GPIO_Pin_Trig);
Init_GPIO_In( SENSOR_FRONT_R -> GPIO_Trig, SENSOR_FRONT_R -> GPIO_Pin_Trig);
Init_GPIO_In( SENSOR_FRONT_C -> GPIO_Trig, SENSOR_FRONT_C -> GPIO_Pin_Trig);	
	
Init_GPIO_In( SENSOR_BACK_L -> GPIO_Trig, SENSOR_BACK_L -> GPIO_Pin_Trig);
Init_GPIO_In( SENSOR_BACK_R ->GPIO_Trig, SENSOR_BACK_R -> GPIO_Pin_Trig);
Init_GPIO_In( SENSOR_BACK_C -> GPIO_Trig, SENSOR_BACK_C -> GPIO_Pin_Trig);	
	
//INIT TIMER A RAJOUTER
//TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState) ...
	
}

void Init_All_US_Sensor(void){
	Init_US_Sensor(SENSOR_FRONT_L);
	Init_US_Sensor(SENSOR_FRONT_R);
	Init_US_Sensor(SENSOR_FRONT_C);
	Init_US_Sensor(SENSOR_BACK_L);
	Init_US_Sensor(SENSOR_BACK_R);
	Init_US_Sensor(SENSOR_BACK_C);
}

uint32_t Get_USensor(US_Sensor_Typedef * Sensor){
	// TO DO
	
	uint32_t distance=0;

	//send a 10us impulsion
	Send_impulse_GPIO(TIM_Trig, TIM_Channel_Trig, (Sensor->GPIO_Trig), (Sensor->GPIO_Pin_Trig), 10);
	
	//wait for a rising edge
	//handler : launch a timer + wait for a falling edge + stop timer --> return time_echo (us)
	//distance = time_echo/58 //cm
	
	return distance;
}
