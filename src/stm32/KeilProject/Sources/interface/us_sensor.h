#ifndef __US_SENSOR__
#define __US_SENSOR__

// include local library
#include "../barstow/model.h"
#include "../library/adc.h"

// Not accurate Value, we gonna need to choose them
#define GPIO_SENSOR_TRIG_FRONT_L					GPIOA
#define GPIO_SENSOR_TRIG_FRONT_R					GPIOA
#define GPIO_SENSOR_TRIG_FRONT_C					GPIOA
#define GPIO_SENSOR_TRIG_BACK_L						GPIOB
#define GPIO_SENSOR_TRIG_BACK_R						GPIOB
#define GPIO_SENSOR_TRIG_BACK_C						GPIOB

#define GPIO_SENSOR_ECHO_FRONT_L					GPIOA
#define GPIO_SENSOR_ECHO_FRONT_R					GPIOA
#define GPIO_SENSOR_ECHO_FRONT_C					GPIOA
#define GPIO_SENSOR_ECHO_BACK_L						GPIOB
#define GPIO_SENSOR_ECHO_BACK_R						GPIOB
#define GPIO_SENSOR_ECHO_BACK_C						GPIOB

// Not accurate Value, we gonna need to choose them
#define GPIO_PIN_SENSOR_TRIG_FRONT_L			GPIO_Pin_1
#define GPIO_PIN_SENSOR_TRIG_FRONT_R			GPIO_Pin_2
#define GPIO_PIN_SENSOR_TRIG_FRONT_C			GPIO_Pin_3
#define GPIO_PIN_SENSOR_TRIG_BACK_L				GPIO_Pin_1
#define GPIO_PIN_SENSOR_TRIG_BACK_R				GPIO_Pin_2
#define GPIO_PIN_SENSOR_TRIG_BACK_C				GPIO_Pin_3

#define GPIO_PIN_SENSOR_ECHO_FRONT_L			GPIO_Pin_4
#define GPIO_PIN_SENSOR_ECHO_FRONT_R			GPIO_Pin_5
#define GPIO_PIN_SENSOR_ECHO_FRONT_C			GPIO_Pin_6
#define GPIO_PIN_SENSOR_ECHO_BACK_L				GPIO_Pin_4
#define GPIO_PIN_SENSOR_ECHO_BACK_R				GPIO_Pin_5
#define GPIO_PIN_SENSOR_ECHO_BACK_C				GPIO_Pin_6

// Structure that contain all GPIO informations about each UltraSound sensor
// Need to be set up above
typedef struct{
	GPIO_TypeDef * GPIO_trig;
	int GPIO_Pin_trig;
	GPIO_TypeDef * GPIO_echo;
	int GPIO_Pin_echo;
} US_Sensor_Typedef;

extern US_Sensor_Typedef * SENSOR_FRONT_L;
extern US_Sensor_Typedef * SENSOR_FRONT_R;
extern US_Sensor_Typedef * SENSOR_FRONT_C;
extern US_Sensor_Typedef * SENSOR_BACK_L;
extern US_Sensor_Typedef * SENSOR_BACK_R;
extern US_Sensor_Typedef * SENSOR_BACK_C;

/** @brief Update all the UltraSound sensor in a Modele struct
	* @parma Modele: Pointeur to the modele struct to be used
	* @retval None
*/
void Update_US_Sensor(BarstowModel_Typedef * Modele);

/** @brief Init a single UltraSound sensor
	* @parma Sensor: Sensor to init (ex : SENSOR_FRONT_R)
	* @retval None
*/
void Init_US_Sensor(US_Sensor_Typedef * Sensor);

/** @brief Init all UltraSound sensors
	* @retval None
*/
void Init_All_US_Sensor(void);

/** @brief Get the distance value of a specific ultra sound sensor
	* @parma Sensor: Sensor to use (ex : SENSOR_FRONT_R)
	* @retval u32: distance of a obstacle to the sensor in centimeters
*/
uint32_t Get_USensor(US_Sensor_Typedef * Sensor);

#endif
