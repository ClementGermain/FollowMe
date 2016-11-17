#ifndef __DELAY__
#define __DELAY__

// include stm32 library
#include "stm32f10x_tim.h"

/** @brief Wait a unprecise moment before going to the next instruction (about 0.5s)
	* @retval None
*/
void Delay(void);

/** @brief Wait a precise moment before going to the next instruction 
	* @param time to wait in us
	* @retval None
*/
void Delay_us(int time_us); // FONCTION A TESTER

#endif

