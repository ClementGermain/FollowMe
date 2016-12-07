#ifndef __CONTROL__
#define __CONTROL__

#include "stdint.h"

/** Motor direction defines
*/
#define MOTOR_DIRECTION_FORWARD		(+1)
#define MOTOR_DIRECTION_BACKWARD 	(-1)
#define MOTOR_DIRECTION_LEFT		(+1)
#define MOTOR_DIRECTION_RIGHT		(-1)
#define MOTOR_DIRECTION_STOP		(0)

/** @brief 	Structure that contains the
	*					set point for a motor
	* 				
*/
typedef struct
{
	float speed;		/*<! Speed value must be in range [0, 1]. */
	int direction;	/*<! Value taken from @ref Motor direction defines. */ 
} MotorControl_Typedef;


/** @brief 	Structure that contains all the
	*					set points fot each actuator of
	*					the Barstow car
	* 				
*/
typedef struct 
{
	MotorControl_Typedef directionMotor;
	MotorControl_Typedef propulsionMotor;
	int gyro;
	
	//int checksum;
	
}BarstowControl_Typedef;

#endif
