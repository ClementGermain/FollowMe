#include "Model.h"
#include "Control.h"
#include "../gpio.h"
#include "../motor.h"
#include "../SPI_Interface/SPI_Interface.h"


void StartBarstow(void)
{
	
	Init_GPIO_Out(GPIOA, GPIO_Pin_5);
	/*!< Init motors. */
	Init_All_Motor();
	
	/*!< Create model and control structure for SPI. */
	int modSize = sizeof(BarstowModel_Typedef);
	int conSize = sizeof(BarstowControl_Typedef);
	int bufferSize = modSize > conSize ? modSize : conSize;
	bufferSize = bufferSize / sizeof(unsigned char) + sizeof(unsigned char);
	
	unsigned char sendBuffer[bufferSize];
	unsigned char receiveBuffer[bufferSize];
	
	BarstowControl_Typedef 	* BarstowControl 	= (BarstowControl_Typedef*) receiveBuffer;
	BarstowModel_Typedef 		* BarstowModel 		= (BarstowModel_Typedef*) sendBuffer;
	
	/*!< Init control structures. */
	BarstowControl->directionMotor.direction=0;
	BarstowControl->directionMotor.speed=0;
	BarstowControl->propulsionMotor.direction=0;
	BarstowControl->propulsionMotor.speed=0;
	
	BarstowModel->directionMotor.current=123;
	
	/*!< Init Ultrasonic sensor. */
	//TODO
	
	/*< Init SPI communication. */
	InitializeSPI2(receiveBuffer,bufferSize, sendBuffer, bufferSize);
	
	while(1)
	{
		Update_Motors(BarstowControl);
		for (int i=0 ; i < 50000 ; i++);
	}
}
