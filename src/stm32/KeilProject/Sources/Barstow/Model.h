#ifndef __MODEL__
#define __MODEL__

#include "stdint.h"


/** @brief 	Strcuture that contains the feedback
	*  				variables of a motor
	* 
*/
typedef struct
{
	uint32_t current; // Electric current in mA
	uint32_t voltage1; // Voltage of first Driver in mV
	uint32_t voltage2; // Voltage of second Driver in mV
	uint32_t speed; // Speed in Tours/min
}MotorModel_Typedef;


/** @brief 	Strcuture that contains the feedback
	*  				variables of an ultrasonic sensor
	* 
*/
typedef struct
{
	uint32_t distance; // Distance to the closest object in cm
}UltrasonicSensor_Typedef;


/** @brief 	Strcuture that contains all the feedback
	*  				variables of the Barstow car
	* 
*/
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
