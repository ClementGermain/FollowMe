#ifndef __MOTOR_SENSOR__
#define __MOTOR_SENSOR__

// include local library
#include "../barstow/model.h"
#include "../library/adc.h"

#define DIVIDE_BRIDGE_CURRENT									1.0
#define DIVIDE_BRIDGE_VOLTAGE									(1.0/4.3)


// Not accurate Value, we gonna need to choose them
#define GPIO_MOTOR_CURRENT_DIRECTION					GPIOA
#define GPIO_MOTOR_CURRENT_LEFT								GPIOC
#define GPIO_MOTOR_CURRENT_RIGHT							GPIOB
#define GPIO_MOTOR_VOLTAGE_DIRECTION					GPIOC
#define GPIO_MOTOR_VOLTAGE_LEFT								GPIOA
#define GPIO_MOTOR_VOLTAGE_RIGHT							GPIOA

// Not accurate Value, we gonna need to choose them
#define GPIO_PIN_MOTOR_CURRENT_DIRECTION			GPIO_Pin_4
#define GPIO_PIN_MOTOR_CURRENT_LEFT						GPIO_Pin_5
#define GPIO_PIN_MOTOR_CURRENT_RIGHT					GPIO_Pin_1
#define GPIO_PIN_MOTOR_VOLTAGE_DIRECTION			GPIO_Pin_4
#define GPIO_PIN_MOTOR_VOLTAGE_LEFT						GPIO_Pin_5
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
void MotorSensor_Update(BarstowModel_Typedef * Modele);

/** @brief Init the sensor of a single Motor
	* @parma Motor_Sensor: Motor to init (ex : SENSOR_FRONT_R)
	* @retval None
*/
void MotorSensor_InitSingle(Motor_Sensor_Typedef * Motor_Sensor);

/** @brief Init all UltraSound sensors
	* @retval None
*/
void MotorSensor_Init(void);

/** @brief Get the current value of a specific motor
	* @parma Motor_Sensor: Motor to use (ex : MOTOR_LEFT)
	* @retval u32: current of the motor in mA
*/
uint32_t MotorSensor_GetCurrent(Motor_Sensor_Typedef * Motor_Sensor);

/** @brief Get the voltage value of a specific motor
	* @parma Motor_Sensor: Motor to use (ex : MOTOR_LEFT)
	* @retval u32: current of the motor in mV
*/
uint32_t MotorSensor_GetVoltage(Motor_Sensor_Typedef * Motor_Sensor);

/** @brief Find the ADC Channel corresponding to the GPIO_PIN
	* @parma GPIO: do i realy have to say it ???
	* @param Pin: guess ...
	* @retval uint8_t: ADC_Channel
*/
uint32_t MotorSensor_FindChannel(GPIO_TypeDef * GPIO, uint16_t Pin);
#endif
