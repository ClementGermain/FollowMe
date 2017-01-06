#include "barstow.h"
#include "stm32f10x_dma.h"
#include "DMA_STM32F10x.h"

int bufferSize;
BarstowControl_Typedef 	* BarstowControlBuffer;

void StartBarstow(void)
{	
	/*!< Create model and control structure for SPI. */
	int modSize = sizeof(BarstowModel_Typedef);
	int conSize = sizeof(BarstowControl_Typedef);
	bufferSize = modSize > conSize ? modSize : conSize;
	bufferSize = bufferSize / sizeof(unsigned char) + sizeof(unsigned char);
	
	unsigned char sendBuffer[bufferSize];
	unsigned char receiveBuffer[bufferSize];
	
	BarstowControl_Typedef 	* BarstowControl 	= (BarstowControl_Typedef*) receiveBuffer;
	BarstowModel_Typedef * BarstowModel 			= (BarstowModel_Typedef*) sendBuffer;
	BarstowControlBuffer = BarstowControl;
	
	/*!< Init control structures. */
	BarstowControl->directionMotor.direction=0;
	BarstowControl->directionMotor.speed=0;
	BarstowControl->propulsionMotor.direction=0;
	BarstowControl->propulsionMotor.speed=0;
	BarstowControl->gyro=0;
	
	/*!< Init motors. */
	Init_All_Motor();
	
	/*!< Init Ultrasonic sensor. */
	//Start_US_Sensor(BarstowModel);
	
	/*!< Init EOC sensors. */
	Init_All_EOC_Sensors();
	
	/*< Init Motor Sensors. */
	MotorSensor_Init(BarstowModel);
	
	/*< Init SPI communication. */
	InitializeSPI2(receiveBuffer,bufferSize, sendBuffer, bufferSize);
	
	
	/*< Init Gyrophare. */
	Gyro_Init();
		
	/*!< Entering main loop. */
	int i;
	while(1)
	{
		i++;
		
		/*!< Updating motors. */
		Update_Motors(BarstowControl);
		
		/*!< Updating motors Sensor. */
		MotorSensor_Update(BarstowModel);
		
		/*!< Updating EOC Sensor. */
		EOC_Sensor_Update(BarstowModel);
		
		/*!< Updating Gyro. */
		Gyro_Toggle(BarstowControl);
		
		/*! < Global temporisation. */
		//TODO use more precise delay fonction
		 for (int i=0 ; i < 50000 ; i++);
	}
}

void DMA1_Channel4_Event(uint32_t events){
	if (BarstowControlBuffer->checkValue != CHECK_VALUE){
		DMA_Cmd(DMA1_Channel4, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel4, bufferSize);
		DMA_Cmd(DMA1_Channel4, ENABLE);
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel5, bufferSize);
		DMA_Cmd(DMA1_Channel5, ENABLE);

		BarstowControlBuffer->checkValue = CHECK_VALUE;
	}
}
/*
void DMA1_Channel5_Event(uint32_t events){
	if (BarstowControlBuffer->checkValue != CHECK_VALUE){
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel5, bufferSize);
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}*/
