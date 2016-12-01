#include "gyrophare.h"
#include "../library/gpio.h"

void Gyro_Init(){
	Init_GPIO_Out(GYRO_GPIO, GYRO_GPIO_PIN);
}

void Gyro_Toggle(BarstowControl_Typedef * Barstow_Control){
	static int current_gyro_state = 0;
	int wanted_gyro_state = Barstow_Control->gyro;
	if (current_gyro_state != wanted_gyro_state){
		current_gyro_state = wanted_gyro_state;
		
		switch (wanted_gyro_state){
			case 1:
				GPIO_SetBits(GYRO_GPIO, GYRO_GPIO_PIN);
				break;
			case 0:
				GPIO_ResetBits(GYRO_GPIO, GYRO_GPIO_PIN);
				break;
		}
	}
}

