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
	Init_GPIO_In(Motor_Sensor->GPIO_Current, Motor_Sensor->GPIO_Pin_Current);
	Init_GPIO_In(Motor_Sensor->GPIO_Voltage, Motor_Sensor->GPIO_Pin_Voltage);
}

void Init_All_Motor_Sensor(void){
	ADC_Configuration(ADC1);
	Init_Motor_Sensor(MOTOR_DIRECTION);
	Init_Motor_Sensor(MOTOR_LEFT);
	Init_Motor_Sensor(MOTOR_RIGHT);
}

uint32_t Get_Current(Motor_Sensor_Typedef * Motor_Sensor){
	return ADC_Read(ADC1, Find_Channel(Motor_Sensor->GPIO_Current, Motor_Sensor->GPIO_Pin_Current)) / DIVIDE_BRIDGE_CURRENT;
}

uint32_t Get_Voltage(Motor_Sensor_Typedef * Motor_Sensor){
	return ADC_Read(ADC1, Find_Channel(Motor_Sensor->GPIO_Voltage, Motor_Sensor->GPIO_Pin_Voltage)) / DIVIDE_BRIDGE_VOLTAGE;
}

uint32_t Find_Channel(GPIO_TypeDef * GPIO, uint16_t Pin){
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
