#include "us_sensor.h"

// Create the US_Sensor Structures
US_Sensor_Typedef SENSOR_FRONT_L_	= {GPIO_SENSOR_FRONT_L, GPIO_PIN_SENSOR_FRONT_L};
US_Sensor_Typedef SENSOR_FRONT_R_	= {GPIO_SENSOR_FRONT_R, GPIO_PIN_SENSOR_FRONT_R};
US_Sensor_Typedef SENSOR_FRONT_C_	= {GPIO_SENSOR_FRONT_C, GPIO_PIN_SENSOR_FRONT_C};
US_Sensor_Typedef SENSOR_BACK_L_	= {GPIO_SENSOR_BACK_L, GPIO_PIN_SENSOR_BACK_L};
US_Sensor_Typedef SENSOR_BACK_R_ 	= {GPIO_SENSOR_BACK_R, GPIO_PIN_SENSOR_BACK_R};
US_Sensor_Typedef SENSOR_BACK_C_	= {GPIO_SENSOR_BACK_C, GPIO_PIN_SENSOR_BACK_C};

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

void Init_US_Sensor(US_Sensor_Typedef * Sensor){
	//TODO
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
	return 0;
}
