#include "barstow.h"
#include "stm32f10x_dma.h"
#include <stddef.h>
#include "Board_LED.h"

int bufferSize;
BarstowControl_Typedef 	* BarstowControl;
BarstowModel_Typedef * BarstowModel;

/*!< Local event function for spi sync */
void DMA1_Channel4_Event(void);
void DMA1_Channel5_Event(void);

void StartBarstow(void)
{	
	/*!< Create model and control structure for SPI. */
	int modSize = sizeof(BarstowModel_Typedef);
	int conSize = sizeof(BarstowControl_Typedef);
	bufferSize = modSize > conSize ? modSize : conSize;
	bufferSize = bufferSize / sizeof(unsigned char) + sizeof(unsigned char);
	
	unsigned char sendBuffer[bufferSize];
	unsigned char receiveBuffer[bufferSize];
	
	BarstowControl 	= (BarstowControl_Typedef*) receiveBuffer;
	BarstowModel 			= (BarstowModel_Typedef*) sendBuffer;
	
	/*!< Init control structures. */
	BarstowControl->directionMotor.direction=0;
	BarstowControl->directionMotor.speed=0;
	BarstowControl->propulsionMotor.direction=0;
	BarstowControl->propulsionMotor.speed=0;
	BarstowControl->gyro=0;
	
	/*!< Init Systic. */
	InitSystick();
	
	/*!< Init motors. */
	Init_All_Motor();
	
	/*!< Init Ultrasonic sensor. */
	//Start_US_Sensor(BarstowModel);
	
	/*!< Init EOC sensors. */
	//Init_All_EOC_Sensors();
	
	/*< Init Motor Sensors. */
	MotorSensor_Init(BarstowModel);
	
	/*< Init SPI communication. */
	InitializeSPI2(receiveBuffer,bufferSize, sendBuffer, bufferSize, DMA1_Channel4_Event, DMA1_Channel5_Event);
	
	/*< Init Gyrophare. */
	//Gyro_Init();
	
	/*< Debug code.*/
	BarstowModel->frontCenterUSensor.distance = 200;
	BarstowModel->frontRightUSensor.distance = 300;
		
	/*!< Entering main loop. */
	while(1)
	{
		BarstowModel->frontLeftUSensor.distance +=1;	
		
		/*!< Updating motors. */
		Update_Motors(BarstowControl);
		
		/*!< Updating motors Sensor. */
		MotorSensor_Update(BarstowModel);
		
		/*!< Updating EOC Sensor. */
		//EOC_Sensor_Update(BarstowModel);
		
		/*!< Updating Gyro. */
		//Gyro_Toggle(BarstowControl);
		
		/*! < Global temporisation. */
		//TODO use more precise delay fonction
		
		for (int i=0 ; i < 50000 ; i++);
	}
}

void DMA1_Channel4_Event(){
	/*if (BarstowControlBuffer->checkValue != CHECK_VALUE){
		DMA_Cmd(DMA1_Channel4, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel4, bufferSize);
		DMA_Cmd(DMA1_Channel4, ENABLE);
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel5, bufferSize);
		DMA_Cmd(DMA1_Channel5, ENABLE);

		BarstowControlBuffer->checkValue = CHECK_VALUE;
	}*/
}

void DMA1_Channel5_Event(){
	/*if (BarstowControlBuffer->checkValue != CHECK_VALUE){
		DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel5, bufferSize);
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}*/
}

void SysticHandler(void){
	Update_Direction_Motors(BarstowControl);
}
	
float InitSystick(void){
	Time=0;
	float period; //period systick us
	period=Systick_Period(SYSTIC_PERIOD);
	Systick_Prio_IT(6,&SysticHandler);
	SysTick_On;
	SysTick_Enable_IT;
	
	return period;
}



