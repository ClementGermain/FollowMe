#include "barstow.h"

void StartBarstow(void)
{	
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
	BarstowModel_Typedef * BarstowModel 		= (BarstowModel_Typedef*) sendBuffer;
	
	/*!< Init control structures. */
	BarstowControl->directionMotor.direction=0;
	BarstowControl->directionMotor.speed=0;
	BarstowControl->propulsionMotor.direction=0;
	BarstowControl->propulsionMotor.speed=0;
	
	/*!< Init Ultrasonic sensor. */
	//TODO
	BarstowModel->frontCenterUSensor.distance = 0.0f; //Avoid warning, must be removed when sensors are implemented
	
	/*< Init SPI communication. */
	InitializeSPI2(receiveBuffer,bufferSize, sendBuffer, bufferSize);
	
	/*!< Entering main loop. */
	while(1)
	{
		/*!< Updating motors. */
		Update_Motors(BarstowControl);
		
		/*! < Global temporisation. */
		//TODO use more precise delay fonction
		for (int i=0 ; i < 50000 ; i++);
	}
}
