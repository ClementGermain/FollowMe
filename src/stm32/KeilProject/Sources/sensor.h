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

/** @brief Update all the sensor in a Modele struct
    * @parma Modele: Pointeur to the modele struct to be used
    * @retval None
*/
void Update_Sensor(BarstowModel_Typedef * Modele);

/** @brief Get the distance value of a specific ultra sound sensor
    * @parma Sensor: Sensor to use (ex : SENSOR_FRONT_R)
    * @retval u32: distance of a obstacle to the sensor in centimeters
*/
uint32_t Get_USensor(int Sensor);

/** @brief Get the current value of a specific motor
    * @parma Sensor: Sensor to use (ex : MOTOR_LEFT)
    * @retval u32: current of the motor in mA
*/
uint32_t Get_Current(int Motor);

/** @brief Get the voltage value of a specific motor
    * @parma Sensor: Sensor to use (ex : MOTOR_LEFT)
    * @retval u32: current of the motor in mV
*/
uint32_t Get_Voltage(int Motor);

#endif
