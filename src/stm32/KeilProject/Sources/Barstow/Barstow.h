#ifndef __BARSTOW_H
#define __BARSTOW_H

// include local library
#include "Model.h"
#include "Control.h"
#include "../gpio.h"
#include "../motor.h"
#include "../SPI_Interface/SPI_Interface.h"


/** @brief Start the Barstow car's programm
	* @retval None
*/
void StartBarstow(void);

#endif
