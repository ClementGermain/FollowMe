#include "motor_sensor.h"

// Create the Motor_Sensor Structures
Motor_Sensor_Typedef MOTOR_DIRECTION_ =	{ GPIO_MOTOR_CURRENT_DIRECTION,
																					GPIO_PIN_MOTOR_CURRENT_DIRECTION,
																					GPIO_MOTOR_VOLTAGE_DIRECTION,
																					GPIO_PIN_MOTOR_VOLTAGE_DIRECTION };

Motor_Sensor_Typedef MOTOR_LEFT_ = 			{ GPIO_MOTOR_CURRENT_LEFT,
																					GPIO_PIN_MOTOR_CURRENT_LEFT,
																					GPIO_MOTOR_VOLTAGE_LEFT,
																					GPIO_PIN_MOTOR_VOLTAGE_LEFT };

Motor_Sensor_Typedef MOTOR_RIGHT_ = 		{ GPIO_MOTOR_CURRENT_RIGHT,
																					GPIO_PIN_MOTOR_CURRENT_RIGHT,
																					GPIO_MOTOR_VOLTAGE_RIGHT,
																					GPIO_PIN_MOTOR_VOLTAGE_RIGHT};

// Create the Motor_Sensor pointeur's structures
Motor_Sensor_Typedef * MOTOR_DIRECTION = 	&MOTOR_DIRECTION_;
Motor_Sensor_Typedef * MOTOR_LEFT = 			&MOTOR_LEFT_;
Motor_Sensor_Typedef * MOTOR_RIGHT = 			&MOTOR_RIGHT_;

void MotorSensor_Update(BarstowModel_Typedef * Modele){
	
	Modele->directionMotor.current = MotorSensor_GetCurrent(MOTOR_DIRECTION);
	Modele->directionMotor.voltage = MotorSensor_GetVoltage(MOTOR_DIRECTION);
	
	Modele->leftWheelMotor.current = MotorSensor_GetCurrent(MOTOR_LEFT);
	Modele->leftWheelMotor.voltage = MotorSensor_GetVoltage(MOTOR_LEFT);
	
	Modele->rightWheelMotor.current = MotorSensor_GetCurrent(MOTOR_RIGHT);
	Modele->rightWheelMotor.voltage = MotorSensor_GetVoltage(MOTOR_RIGHT);
}

void MotorSensor_InitSingle(Motor_Sensor_Typedef * Motor_Sensor){
	Init_GPIO_In(Motor_Sensor->GPIO_Current, Motor_Sensor->GPIO_Pin_Current);
	Init_GPIO_In(Motor_Sensor->GPIO_Voltage, Motor_Sensor->GPIO_Pin_Voltage);
}

void MotorSensor_Init(void){
	ADC_Configuration(ADC1);
	MotorSensor_InitSingle(MOTOR_DIRECTION);
	MotorSensor_InitSingle(MOTOR_LEFT);
	MotorSensor_InitSingle(MOTOR_RIGHT);
}

uint32_t MotorSensor_GetCurrent(Motor_Sensor_Typedef * Motor_Sensor){
	return ADC_Read(ADC1, MotorSensor_FindChannel(Motor_Sensor->GPIO_Current, Motor_Sensor->GPIO_Pin_Current)) * 1000 / (DIVIDE_BRIDGE_CURRENT * 0xFFF);
}

uint32_t MotorSensor_GetVoltage(Motor_Sensor_Typedef * Motor_Sensor){

	uint32_t val = ADC_Read(ADC1, MotorSensor_FindChannel(Motor_Sensor->GPIO_Voltage, Motor_Sensor->GPIO_Pin_Voltage));
	val = val * 1000;
	val = val * 3.3 / 0xFFF;
	return val / DIVIDE_BRIDGE_VOLTAGE;
}

uint32_t MotorSensor_FindChannel(GPIO_TypeDef * GPIO, uint16_t Pin){
	uint8_t channel;
	if (GPIO == GPIOA){
		switch (Pin){
			case GPIO_Pin_0:
				channel = ADC_Channel_0;
				break;
			case GPIO_Pin_1:
				channel = ADC_Channel_1;
				break;
			case GPIO_Pin_2:
				channel = ADC_Channel_2;
				break;
			case GPIO_Pin_3:
				channel = ADC_Channel_3;
				break;
			case GPIO_Pin_4:
				channel = ADC_Channel_4;
				break;
			case GPIO_Pin_5:
				channel = ADC_Channel_5;
				break;
			case GPIO_Pin_6:
				channel = ADC_Channel_6;
				break;
			case GPIO_Pin_7:
				channel = ADC_Channel_7;
				break;
			}
		}
		else if (GPIO == GPIOB){
		switch (Pin){
			case GPIO_Pin_0:
				channel = ADC_Channel_8;
				break;
			case GPIO_Pin_1:
				channel = ADC_Channel_9;
				break;
			}
		}
		else if (GPIO == GPIOC){
		switch (Pin){
			case GPIO_Pin_4:
				channel = ADC_Channel_14;
				break;
			case GPIO_Pin_5:
				channel = ADC_Channel_15;
				break;
			}
		}
		return channel;
}