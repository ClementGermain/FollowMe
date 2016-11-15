#ifndef __MOTOR_SENSOR__
#define __MOTOR_SENSOR__

// include local library
#include "../barstow/model.h"
#include "../library/adc.h"

// Not accurate Value, we gonna need to choose them
#define GPIO_MOTOR_CURRENT_DIRECTION					GPIOB
#define GPIO_MOTOR_CURRENT_LEFT								GPIOB
#define GPIO_MOTOR_CURRENT_RIGHT							GPIOB
#define GPIO_MOTOR_VOLTAGE_DIRECTION					GPIOB
#define GPIO_MOTOR_VOLTAGE_LEFT								GPIOB
#define GPIO_MOTOR_VOLTAGE_RIGHT							GPIOB

// Not accurate Value, we gonna need to choose them
#define GPIO_PIN_MOTOR_CURRENT_DIRECTION			GPIO_Pin_1
#define GPIO_PIN_MOTOR_CURRENT_LEFT						GPIO_Pin_2
#define GPIO_PIN_MOTOR_CURRENT_RIGHT					GPIO_Pin_3
#define GPIO_PIN_MOTOR_VOLTAGE_DIRECTION			GPIO_Pin_1
#define GPIO_PIN_MOTOR_VOLTAGE_LEFT						GPIO_Pin_2
#define GPIO_PIN_MOTOR_VOLTAGE_RIGHT					GPIO_Pin_3

// Structure that contain all GPIO informations about each motor's sensor
// Need to be set up above
typedef struct{
	GPIO_TypeDef * GPIO_Current;
	int GPIO_Pin_Current;
	GPIO_TypeDef * GPIO_Voltage;
	int GPIO_Pin_Voltage;
} Motor_Sensor_Typedef;

extern Motor_Sensor_Typedef * MOTOR_DIRECTION;
extern Motor_Sensor_Typedef * MOTOR_LEFT;
extern Motor_Sensor_Typedef * MOTOR_RIGHT;

/** @brief Update all the motor sensor in a Modele struct
	* @parma Modele: Pointeur to the modele struct to be used
	* @retval None
*/
void Update_Motor_Sensor(BarstowModel_Typedef * Modele);

/** @brief Init the sensor of a single Motor
	* @parma Motor_Sensor: Motor to init (ex : SENSOR_FRONT_R)
	* @retval None
*/
void Init_Motor_Sensor(Motor_Sensor_Typedef * Motor_Sensor);

/** @brief Init all UltraSound sensors
	* @retval None
*/
void Init_All_Motor_Sensor(void);

/** @brief Get the current value of a specific motor
	* @parma Motor_Sensor: Motor to use (ex : MOTOR_LEFT)
	* @retval u32: current of the motor in mA
*/
uint32_t Get_Current(Motor_Sensor_Typedef * Motor_Sensor);

/** @brief Get the voltage value of a specific motor
	* @parma Motor_Sensor: Motor to use (ex : MOTOR_LEFT)
	* @retval u32: current of the motor in mV
*/
uint32_t Get_Voltage(Motor_Sensor_Typedef * Motor_Sensor);

#endif
