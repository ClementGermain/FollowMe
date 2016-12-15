#include "EOC_sensor.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

// Create the EOC_Sensor Structures
EOC_Sensor_Typedef EOC_L_	= {GPIO_EOC_L, GPIO_PIN_EOC_L};
EOC_Sensor_Typedef EOC_R_	= {GPIO_EOC_R, GPIO_PIN_EOC_R};

// Create the EOC_Sensor pointeur's structures
EOC_Sensor_Typedef * EOC_L 	= &EOC_L_;
EOC_Sensor_Typedef * EOC_R 	= &EOC_R_;


void Init_EOC_Sensor(EOC_Sensor_Typedef * EOC_Sensor){
	Init_GPIO_IPU(EOC_Sensor->GPIO, EOC_Sensor->GPIO_Pin);
}

void Init_All_EOC_Sensors(void){
	Init_EOC_Sensor(EOC_L);
	Init_EOC_Sensor(EOC_R);
}

int Is_EOC_Sensor(EOC_Sensor_Typedef * EOC_Sensor){
	return GPIO_ReadInputDataBit(EOC_Sensor->GPIO, EOC_Sensor->GPIO_Pin);
}


void EOC_Sensor_Update(BarstowModel_Typedef * mod){
	mod->leftEocSensor.endOfCourse=Is_EOC_Sensor(EOC_L);
	mod->rightEocSensor.endOfCourse=Is_EOC_Sensor(EOC_R);
}

