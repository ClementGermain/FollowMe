#ifndef __GYROPHARE__
#define __GYROPHARE__

#include "../Barstow/control.h"

#define GYRO_GPIO					GPIOC
#define GYRO_GPIO_PIN			GPIO_Pin_9

/** @brief Init the GPIO Pin of the Gyro
	* @retval None
*/
void Gyro_Init(void);

/** @brief Enable of Disable the Gyrophare based on the Barstow Control struct
	* @retval None
*/
void Gyro_Toggle(BarstowControl_Typedef * Barstow_Control);

#endif
