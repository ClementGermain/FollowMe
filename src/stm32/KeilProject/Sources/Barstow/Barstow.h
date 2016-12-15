#ifndef __BARSTOW__
#define __BARSTOW__

// include local library
#include "model.h"
#include "control.h"
#include "../library/gpio.h"
#include "../interface/us_sensor.h"
#include "../interface/EOC_sensor.h"
#include "../interface/motor.h"
#include "../interface/spi.h"
#include "../interface/motor_sensor.h"
#include "../interface/gyrophare.h"


/** @brief Start the Barstow car's programm
	* @retval None
*/
void StartBarstow(void);

#endif
