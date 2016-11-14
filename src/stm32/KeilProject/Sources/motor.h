#ifndef __MOTOR__
#define __MOTOR__

#include "include.h"
#include "Barstow/Control.h"

#define TIM_Forward 										TIM4
#define TIM_Backward 										TIM3
#define TIM_Left 												TIM4
#define TIM_Right		 										TIM4
#define TIM_Channel_Forward							TIM_Channel_1
#define TIM_Channel_Backward						TIM_Channel_2
#define TIM_Channel_Left								TIM_Channel_3
#define TIM_Channel_Right								TIM_Channel_4
#define MaxDutyCycle										0.5	// Value that limit the Motor's voltage to 6V Max
#define MinDutyCycle_Direction					0.1		// Min value that allows a sufficent Motor's voltage to turn
#define MinDutyCycle_Move								0.1		// Min value that allows a sufficent Motor's voltage to move
#define GPIO_Motor_Enable								GPIOA
#define GPIO_Pin_Motor_Enable						GPIO_Pin_6

typedef struct {
	GPIO_TypeDef * GPIO;
	int Pin;
} GPIO_Pin_TypeDef;  

void Update_Motors(BarstowControl_Typedef Control);

/**
	*	@brief Init all the motors's PWM and GPIO
	*	@retval None
*/
void Init_All_Motor(void);

/**
	*	@brief Init a single motor's PWM and GPIO
	* @param TIM: Timer of the Motor's PWM
	*	@param Channel: Channel of the Motor's PWM
	*	@retval None
*/
void Init_Single_Motor(TIM_TypeDef * TIM, int Channel);
/**
	*	@brief Make the car turn Left
	*	@parma Rot: set the relative rotation to set on the forward wheels. Value between 0-1
	*	@retval None
*/
void Turn_Left(float Rot);

/**
	*	@brief Make the car turn Right
	*	@parma Rot: set the relative rotation to set on the forward wheels. Value between 0-1
	*	@retval None
*/
void Turn_Right(float Rot);

/**
	*	@brief Stop Turn the front wheels
	*	@retval None
*/
void Stop_Turn(void);

/**
	*	@brief Reset the Direction of the car to 0° to go on straigt line
	*	@retval None
*/
void Reset_Direction(void); // We need to figure it how to do this !!!!

/**
	*	@brief Make the car go forward
	*	@parma Speed: Set the speed to roll (value between 0-1)
	*	@retval None
*/
void Go_Forward(float Speed);

/**
	*	@brief Make the car go forward
	*	@parma Speed: Set the speed to roll (value between 0-1)
	*	@retval None
*/
void Go_Back(float Speed);

/**
	*	@brief Stop the Car
	*	@retval None
*/
void Stop_Car(void);

/**
	*	@brief Set the Duty Cycle of the PWM. The point is to adjust the value to the PWM's allowed range (given by MaxDutyCycle)
	*	@parma Value: Speed or Rotation
	*	@retval Correct duty cycle of the PMW's motors
*/
float Calcul_DutyCycle(float Value);

/**
	*	@brief A procedure function to test the above functions
	*	@retval None
*/
int Motor_Test(int Value); // TODO

/**
	*	@brief Search the GPIO/Pin linked to the PWM's Channel/Timer in no remap mode
	* @param TIM: Timer of the PWM
	*	@param Channel: Channel of the PWM
	*	@retval A GPIO_Pin struct with the right GPIO and Pin of the PWM
*/
GPIO_Pin_TypeDef Find_GPIO_Pin(TIM_TypeDef * TIM, int Channel);
#endif
