#include "motor_sensor.h"

// Create the Motor_Sensor Structures
Motor_Sensor_Typedef MOTOR_DIRECTION_ =	{ GPIO_MOTOR_CURRENT_DIRECTION,
																					GPIO_PIN_MOTOR_CURRENT_DIRECTION,
																					GPIO_MOTOR_VOLTAGE_DIRECTION_1,
																					GPIO_PIN_MOTOR_VOLTAGE_DIRECTION_1,
																					GPIO_MOTOR_VOLTAGE_DIRECTION_2,
																					GPIO_PIN_MOTOR_VOLTAGE_DIRECTION_2 };

Motor_Sensor_Typedef MOTOR_LEFT_ = 			{ GPIO_MOTOR_CURRENT_LEFT,
																					GPIO_PIN_MOTOR_CURRENT_LEFT,
																					GPIO_MOTOR_VOLTAGE_LEFT_1,
																					GPIO_PIN_MOTOR_VOLTAGE_LEFT_1,
																					GPIO_MOTOR_VOLTAGE_LEFT_2,
																					GPIO_PIN_MOTOR_VOLTAGE_LEFT_2 };

Motor_Sensor_Typedef MOTOR_RIGHT_ = 		{ GPIO_MOTOR_CURRENT_RIGHT,
																					GPIO_PIN_MOTOR_CURRENT_RIGHT,
																					GPIO_MOTOR_VOLTAGE_RIGHT_1,
																					GPIO_PIN_MOTOR_VOLTAGE_RIGHT_1,
																					GPIO_MOTOR_VOLTAGE_RIGHT_2,
																					GPIO_PIN_MOTOR_VOLTAGE_RIGHT_2};

// Create the Motor_Sensor pointeur's structures
Motor_Sensor_Typedef * MOTOR_DIRECTION = 	&MOTOR_DIRECTION_;
Motor_Sensor_Typedef * MOTOR_LEFT = 			&MOTOR_LEFT_;
Motor_Sensor_Typedef * MOTOR_RIGHT = 			&MOTOR_RIGHT_;

void MotorSensor_Update(BarstowModel_Typedef * Modele){

MotorSensor_SetCurrent(&(Modele->directionMotor), MOTOR_DIRECTION);
MotorSensor_SetVoltage(&(Modele->directionMotor), MOTOR_DIRECTION);
MotorSensor_SetVoltage(&(Modele->directionMotor), MOTOR_DIRECTION);

MotorSensor_SetCurrent(&(Modele->leftWheelMotor), MOTOR_LEFT);
MotorSensor_SetVoltage(&(Modele->leftWheelMotor), MOTOR_LEFT);
MotorSensor_SetVoltage(&(Modele->leftWheelMotor), MOTOR_LEFT);

MotorSensor_SetCurrent(&(Modele->rightWheelMotor), MOTOR_RIGHT);
MotorSensor_SetVoltage(&(Modele->rightWheelMotor), MOTOR_RIGHT);
MotorSensor_SetVoltage(&(Modele->rightWheelMotor), MOTOR_RIGHT);
}

void MotorSensor_InitSingle( MotorModel_Typedef * MotorModel , Motor_Sensor_Typedef * Motor_Sensor){
	
	MotorModel->current = 0;
	MotorModel->voltage1 = 0;
	MotorModel->voltage2 = 0;
	
	Init_GPIO_In(Motor_Sensor->GPIO_Current, Motor_Sensor->GPIO_Pin_Current);
	Init_GPIO_In(Motor_Sensor->GPIO_Voltage_1, Motor_Sensor->GPIO_Pin_Voltage_1);
	Init_GPIO_In(Motor_Sensor->GPIO_Voltage_2, Motor_Sensor->GPIO_Pin_Voltage_2);
}

void MotorSensor_Init(BarstowModel_Typedef * BarstowModele){
	ADC_Configuration(ADC1);
	
	MotorSensor_InitSingle(&(BarstowModele->directionMotor) , MOTOR_DIRECTION);
	MotorSensor_InitSingle(&(BarstowModele->leftWheelMotor) , MOTOR_LEFT);
	MotorSensor_InitSingle(&(BarstowModele->rightWheelMotor) , MOTOR_RIGHT);
}

void MotorSensor_SetCurrent(MotorModel_Typedef * Motor_Modele, Motor_Sensor_Typedef * Motor_Sensor){
	Motor_Modele->current = ADC_Read(ADC1, MotorSensor_FindChannel(Motor_Sensor->GPIO_Current, Motor_Sensor->GPIO_Pin_Current)) * 1000 * 3.3 / (DIVIDE_BRIDGE_CURRENT * 0xFFF);
}

void MotorSensor_SetVoltage(MotorModel_Typedef * Motor_Modele, Motor_Sensor_Typedef * Motor_Sensor){
Motor_Modele->voltage1 = ADC_Read(ADC1, MotorSensor_FindChannel(Motor_Sensor->GPIO_Voltage_1, Motor_Sensor->GPIO_Pin_Voltage_1)) * 1000 * 3.3 / (DIVIDE_BRIDGE_VOLTAGE * 0xFFF);
Motor_Modele->voltage2 = ADC_Read(ADC1, MotorSensor_FindChannel(Motor_Sensor->GPIO_Voltage_2, Motor_Sensor->GPIO_Pin_Voltage_2)) * 1000 * 3.3 / (DIVIDE_BRIDGE_VOLTAGE * 0xFFF);
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
