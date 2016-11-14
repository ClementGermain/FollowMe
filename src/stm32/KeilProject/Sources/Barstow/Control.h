#ifndef __CONTROL_H
#define __CONTROL_H

#include "stdint.h"

#define MOTOR_DIRECTION_FORWARD		(+1)
#define MOTOR_DIRECTION_BACKWARD 	(-1)
#define MOTOR_DIRECTION_LEFT		(+1)
#define MOTOR_DIRECTION_RIGHT		(-1)
#define MOTOR_DIRECTION_STOP		(0)
typedef struct
{
	// 'speed' must be in range [0, 1]
	float speed;
	// 'direction' is either 1, 0, or -1, see above
	int direction; 
} MotorControl_Typedef;

typedef struct 
{
	MotorControl_Typedef directionMotor;
	MotorControl_Typedef propulsionMotor;
	
	//int checksum;
	
}BarstowControl_Typedef;

#endif
