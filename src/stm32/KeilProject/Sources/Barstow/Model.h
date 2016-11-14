#ifndef __MODEL_H
#define __MODEL_H

#include "stdint.h"

typedef struct
{
	uint32_t current;
	uint32_t voltage;
}MotorModel_Typedef;

typedef struct
{
	uint32_t distance;
}UltrasonicSensor_Typedef;

typedef struct 
{
	MotorModel_Typedef directionMotor;
	MotorModel_Typedef leftWheelMotor;
	MotorModel_Typedef rightWheelMotor;
	
	UltrasonicSensor_Typedef frontLeftUSensor;
	UltrasonicSensor_Typedef frontRightUSensor;
	UltrasonicSensor_Typedef frontCenterUSensor;
	UltrasonicSensor_Typedef rearLeftUSensor;
	UltrasonicSensor_Typedef rearRightUSensor;
	UltrasonicSensor_Typedef rearCenterUSensor;
	
	//uint32_t checksum; 
	
}BarstowModel_Typedef;

#endif
