#include "sensor.h"

void Update_Sensor(BarstowModel_Typedef * Modele){
	
	Modele->frontRightUSensor.distance = (Get_USensor(SENSOR_FRONT_R));
	Modele->frontLeftUSensor.distance = (Get_USensor(SENSOR_FRONT_L));
	Modele->frontCenterUSensor.distance = (Get_USensor(SENSOR_FRONT_C));
	Modele->rearRightUSensor.distance = (Get_USensor(SENSOR_BACK_R));
	Modele->rearLeftUSensor.distance = (Get_USensor(SENSOR_BACK_L));
	Modele->rearCenterUSensor.distance = (Get_USensor(SENSOR_BACK_C));
	
	Modele->directionMotor.current = Get_Current(MOTOR_DIRECTION);
	Modele->directionMotor.voltage = Get_Voltage(MOTOR_DIRECTION);
	
	Modele->leftWheelMotor.current = Get_Current(MOTOR_LEFT);
	Modele->leftWheelMotor.voltage = Get_Voltage(MOTOR_LEFT);
	
	Modele->rightWheelMotor.current = Get_Current(MOTOR_RIGHT);
	Modele->rightWheelMotor.voltage = Get_Voltage(MOTOR_RIGHT);
}

uint32_t Get_USensor(int Sensor){
	return 0;
}

uint32_t Get_Current(int Motor){
	return 0;
}

uint32_t Get_Voltage(int Motor){
	return 0;
}
