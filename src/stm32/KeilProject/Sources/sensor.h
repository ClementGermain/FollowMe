#ifndef __SENSOR__
#define __SENSOR__

#include "stdint.h"
#include "Barstow/Model.h"

#define SENSOR_FRONT_L				1
#define SENSOR_FRONT_R				2
#define SENSOR_FRONT_C				3
#define SENSOR_BACK_L					4
#define SENSOR_BACK_R					5
#define SENSOR_BACK_C					6
#define MOTOR_DIRECTION				1
#define MOTOR_LEFT						2
#define MOTOR_RIGHT						3

void Update_Sensor(BarstowModel_Typedef * Modele);

uint32_t Get_USensor(int Sensor);

uint32_t Get_Current(int Motor);

uint32_t Get_Voltage(int Motor);

#endif
