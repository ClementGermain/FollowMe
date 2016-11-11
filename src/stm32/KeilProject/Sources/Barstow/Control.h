#ifndef __CONTROL_H
#define __CONTROL_H

#include "stdint.h"

typedef struct
{
	uint32_t current;
	uint32_t voltage;
}MotorControl_Typedef;

typedef struct 
{
	MotorControl_Typedef directionMotor;
	MotorControl_Typedef leftWheelMotor;
	MotorControl_Typedef rightWheelMotor;
	
	//int checksum;
	
}BarstowControl_Typedef;

#endif