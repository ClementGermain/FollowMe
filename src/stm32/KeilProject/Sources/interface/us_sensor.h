#ifndef __US_SENSOR__
#define __US_SENSOR__

// include local library
#include "../barstow/model.h"
#include "../library/adc.h"
#include "../library/gpio.h"
#include "../library/timer.h"
#include "../library/Timer_Systick.h"

// Not accurate Value, we gonna need to choose them
#define GPIO_SENSOR_TRIG_FRONT_L					GPIOC //ok
#define GPIO_SENSOR_TRIG_FRONT_R					GPIOA //ok
#define GPIO_SENSOR_TRIG_FRONT_C					GPIOD //ok
#define GPIO_SENSOR_TRIG_BACK_L						GPIOB
#define GPIO_SENSOR_TRIG_BACK_R						GPIOB
#define GPIO_SENSOR_TRIG_BACK_C						GPIOB

#define GPIO_SENSOR_ECHO_FRONT_L					GPIOC //ok
#define GPIO_SENSOR_ECHO_FRONT_R					GPIOA //ok
#define GPIO_SENSOR_ECHO_FRONT_C					GPIOC //ok
#define GPIO_SENSOR_ECHO_BACK_L						GPIOB
#define GPIO_SENSOR_ECHO_BACK_R						GPIOB
#define GPIO_SENSOR_ECHO_BACK_C						GPIOB

// Not accurate Value, we gonna need to choose them
#define GPIO_PIN_SENSOR_TRIG_FRONT_L			GPIO_Pin_11 //ok
#define GPIO_PIN_SENSOR_TRIG_FRONT_R			GPIO_Pin_14 //ok
#define GPIO_PIN_SENSOR_TRIG_FRONT_C			GPIO_Pin_2 //ok
#define GPIO_PIN_SENSOR_TRIG_BACK_L				GPIO_Pin_1
#define GPIO_PIN_SENSOR_TRIG_BACK_R				GPIO_Pin_2
#define GPIO_PIN_SENSOR_TRIG_BACK_C				GPIO_Pin_3

#define GPIO_PIN_SENSOR_ECHO_FRONT_L			GPIO_Pin_10 //ok
#define GPIO_PIN_SENSOR_ECHO_FRONT_R			GPIO_Pin_13 //ok
#define GPIO_PIN_SENSOR_ECHO_FRONT_C			GPIO_Pin_12 //ok
#define GPIO_PIN_SENSOR_ECHO_BACK_L				GPIO_Pin_4
#define GPIO_PIN_SENSOR_ECHO_BACK_R				GPIO_Pin_5
#define GPIO_PIN_SENSOR_ECHO_BACK_C				GPIO_Pin_6

#define TIM_Echo													TIM2
#define TIM_Channel_Echo_1								TIM_Channel_1// for counting the distance
#define TIM_Channel_Echo_2								TIM_Channel_2
#define TIM_Channel_Echo_3								TIM_Channel_3


#define SYSTICK_PERIOD_US 1000 //1 ms

//Global Time (ms)
extern int Time;

// Structure that contain all GPIO informations about each UltraSound sensor
// Need to be set up above
typedef struct{
	GPIO_TypeDef * GPIO_Trig;
	uint16_t GPIO_Pin_Trig;
	GPIO_TypeDef * GPIO_Echo;
	uint16_t GPIO_Pin_Echo;
} US_Sensor_Typedef;

extern US_Sensor_Typedef * SENSOR_FRONT_L;
extern US_Sensor_Typedef * SENSOR_FRONT_R;
extern US_Sensor_Typedef * SENSOR_FRONT_C;
extern US_Sensor_Typedef * SENSOR_BACK_L;
extern US_Sensor_Typedef * SENSOR_BACK_R;
extern US_Sensor_Typedef * SENSOR_BACK_C;

/** @brief Update all the UltraSound sensor in a Modele struct
	* @param Modele: Pointeur to the modele struct to be used
	* @retval None
*/
void Update_US_Sensor(BarstowModel_Typedef * Modele);

/** @brief Init a single UltraSound sensor
	* @param Sensor: Sensor to init (ex : SENSOR_FRONT_R)
	* @retval None
*/
void Init_US_Sensor(US_Sensor_Typedef * Sensor);

/** @brief Init all UltraSound sensors
	* @retval None
*/
void Init_All_US_Sensor(void);

/** @brief Init and launch Systick Timer that calls Periodic_Impulse_3US() every period
	* @param None
	* @retval float : actual period of Systick
*/
float Init_Systick(void);

/** @brief Get the distance value of a specific ultra sound sensor
	* @param Sensor: Sensor to use (ex : SENSOR_FRONT_R)
	* @retval u32: distance of a obstacle to the sensor in centimeters
*/
uint32_t Get_USensor(US_Sensor_Typedef * Sensor);

/** @brief Send 10us impulse on each front motor, on after the other, with 70ms between each.
	* @param none
	* @retval None
*/
void Periodic_Impulse_3_Front_US(void);

/** @brief Test procedure for Front US Sensors
	* @param none
	* @retval None
*/
void Test_US_Sensor(void);


void My_function_TIF (void);

#endif
