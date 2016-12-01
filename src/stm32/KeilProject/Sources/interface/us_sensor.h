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
#define GPIO_SENSOR_TRIG_FRONT_R					GPIOC //ok
#define GPIO_SENSOR_TRIG_FRONT_C					GPIOD //ok
//#define GPIO_SENSOR_TRIG_BACK_L						GPIOB
//#define GPIO_SENSOR_TRIG_BACK_R						GPIOB
//#define GPIO_SENSOR_TRIG_BACK_C						GPIOB

#define GPIO_SENSOR_ECHO_FRONT_L					GPIOA //ok
#define GPIO_SENSOR_ECHO_FRONT_R					GPIOA //ok
#define GPIO_SENSOR_ECHO_FRONT_C					GPIOA //ok
//#define GPIO_SENSOR_ECHO_BACK_L						GPIOB
//#define GPIO_SENSOR_ECHO_BACK_R						GPIOB
//#define GPIO_SENSOR_ECHO_BACK_C						GPIOB

#define GPIO_Port_Source_Echo_Front_L 	GPIO_PortSourceGPIOA
#define GPIO_Port_Source_Echo_Front_R		GPIO_PortSourceGPIOA
#define GPIO_Port_Source_Echo_Front_C		GPIO_PortSourceGPIOA

// Not accurate Value, we gonna need to choose them
#define GPIO_PIN_SENSOR_TRIG_FRONT_L			GPIO_Pin_11 //ok
#define GPIO_PIN_SENSOR_TRIG_FRONT_R			GPIO_Pin_12 //ok
#define GPIO_PIN_SENSOR_TRIG_FRONT_C			GPIO_Pin_2 //ok

#define GPIO_PIN_SENSOR_ECHO_FRONT_L			GPIO_Pin_0 //ok
#define GPIO_PIN_SENSOR_ECHO_FRONT_R			GPIO_Pin_1 //ok
#define GPIO_PIN_SENSOR_ECHO_FRONT_C			GPIO_Pin_2 //ok

#define GPIO_Num_Port_Echo_Front_L 		0
#define GPIO_Num_Port_Echo_Front_R		1
#define GPIO_Num_Port_Echo_Front_C		2

#define TIM_Echo													TIM2
#define TIM_Channel_Echo_Front_L					TIM_Channel_1// for counting the distance
#define TIM_Channel_Echo_Front_C					TIM_Channel_2
#define TIM_Channel_Echo_Front_R					TIM_Channel_3

#define INTERRUPT_LINE_SENSOR_ECHO_FRONT_L			EXTI_Line0 //ok
#define INTERRUPT_LINE_SENSOR_ECHO_FRONT_R			EXTI_Line1 //ok
#define INTERRUPT_LINE_SENSOR_ECHO_FRONT_C			EXTI_Line2 //ok



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
	uint32_t * ModelPointer;
	int state;
} US_Sensor_Typedef;

extern US_Sensor_Typedef * SENSOR_FRONT_L;
extern US_Sensor_Typedef * SENSOR_FRONT_R;
extern US_Sensor_Typedef * SENSOR_FRONT_C;
extern US_Sensor_Typedef * SENSOR_BACK_L;
extern US_Sensor_Typedef * SENSOR_BACK_R;
extern US_Sensor_Typedef * SENSOR_BACK_C;
extern US_Sensor_Typedef * US_active; //the Ultrasonic sensor on which we sent an impulse

extern int time_echo;
extern int front_us;
extern BarstowModel_Typedef * Model;


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

/** @brief Init bartstow US Sensors
	* @param Model to update
	* @retval None
*/
void Start_US_Sensor(BarstowModel_Typedef * mod);

//fonction d'interruption
void Capture_echo (void);

//
void Relance_Compteur_Echo(void);

#endif
