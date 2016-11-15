#include "motor_sensor.h"

// Create the Motor_Sensor Structures
Motor_Sensor_Typedef MOTOR_DIRECTION_ =	{ GPIO_MOTOR_CURRENT_DIRECTION,
																					GPIO_PIN_MOTOR_CURRENT_DIRECTION,
																					GPIO_MOTOR_VOLTAGE_DIRECTION,
																					GPIO_PIN_MOTOR_VOLTAGE_DIRECTION };

Motor_Sensor_Typedef MOTOR_LEFT_ = 			{ GPIO_MOTOR_CURRENT_DIRECTION,
																					GPIO_PIN_MOTOR_CURRENT_DIRECTION,
																					GPIO_MOTOR_VOLTAGE_DIRECTION,
																					GPIO_PIN_MOTOR_VOLTAGE_DIRECTION };

Motor_Sensor_Typedef MOTOR_RIGHT_ = 		{ GPIO_MOTOR_CURRENT_DIRECTION,
																					GPIO_PIN_MOTOR_CURRENT_DIRECTION,
																					GPIO_MOTOR_VOLTAGE_DIRECTION,
																					GPIO_PIN_MOTOR_VOLTAGE_DIRECTION };

// Create the Motor_Sensor pointeur's structures
Motor_Sensor_Typedef * MOTOR_DIRECTION = 	&MOTOR_DIRECTION_;
Motor_Sensor_Typedef * MOTOR_LEFT = 			&MOTOR_LEFT_;
Motor_Sensor_Typedef * MOTOR_RIGHT = 			&MOTOR_RIGHT_;

void Update_Sensor(BarstowModel_Typedef * Modele){
	
	Modele->directionMotor.current = Get_Current(MOTOR_DIRECTION);
	Modele->directionMotor.voltage = Get_Voltage(MOTOR_DIRECTION);
	
	Modele->leftWheelMotor.current = Get_Current(MOTOR_LEFT);
	Modele->leftWheelMotor.voltage = Get_Voltage(MOTOR_LEFT);
	
	Modele->rightWheelMotor.current = Get_Current(MOTOR_RIGHT);
	Modele->rightWheelMotor.voltage = Get_Voltage(MOTOR_RIGHT);
}

void Init_Motor_Sensor(Motor_Sensor_Typedef * Motor_Sensor){
	//TODO
}

void Init_All_Motor_Sensor(void){
	//TODO
}

uint32_t Get_Current(Motor_Sensor_Typedef * Motor_Sensor){
	return 0;
}

uint32_t Get_Voltage(Motor_Sensor_Typedef * Motor_Sensor){
	return 0;
}
