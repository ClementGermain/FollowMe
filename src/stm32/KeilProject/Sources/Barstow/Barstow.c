#include "Model.h"
#include "Control.h"
#include "../SPI_Interface/SPI_Interface.h"




void StartBarstow(void)
{
	/*!< Create model and control structure for SPI. */
	BarstowControl_Typedef 	BarstowControl;
	BarstowModel_Typedef 		BarstowModel;
	
	/*!< Init Ultrasonic sensor. */
	
	//TODO
	//Use &(BarstowModel.??USensor)
	
	
	/*< Init PWM and motors. */
	//TODO
	//Use &(BarstowModel.?)
	
	/*< Init SPI communication. */
	void InitializeSPI2(void);
	
	
	return;
}