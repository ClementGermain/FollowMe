#ifndef __EOC_SENSOR__
#define __EOC_SENSOR__

// include local library
#include "../barstow/model.h"
#include "../library/gpio.h"

#define GPIO_EOC_L					GPIOD
#define GPIO_EOC_R					GPIOD

#define GPIO_PIN_EOC_L			GPIO_Pin_0
#define GPIO_PIN_EOC_R			GPIO_Pin_1


// Structure that contain all GPIO informations about each UltraSound sensor
// Need to be set up above
typedef struct{
	GPIO_TypeDef * GPIO;
	uint16_t GPIO_Pin;
	uint32_t * ModelPointer;
	int state;
} EOC_Sensor_Typedef;

extern EOC_Sensor_Typedef * EOC_L;
extern EOC_Sensor_Typedef * EOC_R;


/** @brief Init GPIO of a end-of-course sensor
	* @param Sensor: EOC Sensor to init (ex : EOC_R)
	* @retval None
*/
void Init_EOC_Sensor(EOC_Sensor_Typedef * EOC_Sensor);

/** @brief Init both end-of-course sensors
	* @retval None
*/
void Init_All_EOC_Sensors(void);


/** @brief Detects an end-of-course of one sensor
	* @param Sensor: EOC Sensor to use (ex : EOC_R)
	* @retval int: 1 if sensor detects a end-of-course, 0 else
*/
int Is_EOC_Sensor(EOC_Sensor_Typedef * EOC_Sensor);


/** @brief update model with EOC values
	* @param Model to update
	* @retval None
*/
void EOC_Sensor_Update(BarstowModel_Typedef * mod);


#endif
