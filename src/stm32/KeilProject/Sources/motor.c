#include "motor.h"

void Init_All_Motor(void){
	Init_Single_Motor(TIM_Forward, TIM_Channel_Forward);
	Init_Single_Motor(TIM_Backward, TIM_Channel_Backward);
	Init_Single_Motor(TIM_Left, TIM_Channel_Left);
	Init_Single_Motor(TIM_Right, TIM_Channel_Right);
}

void Init_Single_Motor(TIM_TypeDef* TIM, int Channel){
	GPIO_Pin_TypeDef GPIO_Pin = Find_GPIO_Pin(TIM, Channel);
	Init_GPIO_PWM(GPIO_Pin.GPIO, GPIO_Pin.Pin);
	Init_Timer(TIM, 1000);
	Init_PWM(TIM, Channel);
	Set_PWM_DutyCycle(TIM, Channel, 0.0);
}

void Turn_Left(float Rot){
	Set_PWM_DutyCycle(TIM_Right, TIM_Channel_Right, 0.0);
	Set_PWM_DutyCycle(TIM_Left, TIM_Channel_Left, Calcul_DutyCycle(Rot));
}

void Turn_Right(float Rot){
	Set_PWM_DutyCycle(TIM_Left, TIM_Channel_Left, 0.0);
	Set_PWM_DutyCycle(TIM_Right, TIM_Channel_Right, Calcul_DutyCycle(Rot));
}

void Stop_Turn(void){
	Set_PWM_DutyCycle(TIM_Left, TIM_Channel_Left, 0.0);
	Set_PWM_DutyCycle(TIM_Right, TIM_Channel_Right, 0.0);
}

void Reset_Direction(void){ 
	// We need to figure it how to do this !!!!
	// Do we really need it ?
}

void Go_Forward(float Speed){
	Set_PWM_DutyCycle(TIM_Backward, TIM_Channel_Backward, 0.0);
	Set_PWM_DutyCycle(TIM_Forward, TIM_Channel_Forward, Calcul_DutyCycle(Speed));
}

void Go_Back(float Speed){
	Set_PWM_DutyCycle(TIM_Forward, TIM_Channel_Forward, 0.0);
	Set_PWM_DutyCycle(TIM_Backward, TIM_Channel_Backward, Calcul_DutyCycle(Speed));
}

void Stop_Car(void){
	Set_PWM_DutyCycle(TIM_Forward, TIM_Channel_Forward, 0.0);
	Set_PWM_DutyCycle(TIM_Backward, TIM_Channel_Backward, 0.0);
}

float Calcul_DutyCycle(float Value){
	if (Value >= 0.0 && Value <= 1.0)
		return Value * MaxDutyCycle;
	else
		return 0.0;
}

void Motor_test(){
	// TODO
	// A delay function would be usefull here
}

GPIO_Pin_TypeDef Find_GPIO_Pin(TIM_TypeDef * TIM, int Channel){

	GPIO_Pin_TypeDef GPIO_Pin;
	if (TIM == TIM1){
		GPIO_Pin.GPIO = GPIOA;
		switch (Channel){
			case TIM_Channel_1 :
				GPIO_Pin.Pin = GPIO_Pin_8;
				break;
			case TIM_Channel_2 :
				GPIO_Pin.Pin = GPIO_Pin_9;
				break;
			case TIM_Channel_3 :
				GPIO_Pin.Pin = GPIO_Pin_10;
				break;
			case TIM_Channel_4 :
				GPIO_Pin.Pin = GPIO_Pin_11;
				break;
		}
	}
	else if (TIM == TIM2){
		GPIO_Pin.GPIO = GPIOA;
		switch (Channel){
			case TIM_Channel_1 :
				GPIO_Pin.Pin = GPIO_Pin_0;
				break;
			case TIM_Channel_2 :
				GPIO_Pin.Pin = GPIO_Pin_1;
				break;
			case TIM_Channel_3 :
				GPIO_Pin.Pin = GPIO_Pin_2;
				break;
			case TIM_Channel_4 :
				GPIO_Pin.Pin = GPIO_Pin_3;
				break;
		}
	}
	else if (TIM == TIM3){
		switch (Channel){
			case TIM_Channel_1 :
			GPIO_Pin.GPIO = GPIOA;
				GPIO_Pin.Pin = GPIO_Pin_6;
				break;
			case TIM_Channel_2 :
			GPIO_Pin.GPIO = GPIOA;
				GPIO_Pin.Pin = GPIO_Pin_7;
				break;
			case TIM_Channel_3 :
			GPIO_Pin.GPIO = GPIOB;
				GPIO_Pin.Pin = GPIO_Pin_0;
				break;
			case TIM_Channel_4 :
				GPIO_Pin.GPIO = GPIOB;
				GPIO_Pin.Pin = GPIO_Pin_1;
				break;
		}
	}
	else if (TIM == TIM4){
		GPIO_Pin.GPIO = GPIOB;
		switch (Channel){
			case TIM_Channel_1 :
				GPIO_Pin.Pin = GPIO_Pin_6;
				break;
			case TIM_Channel_2 :
				GPIO_Pin.Pin = GPIO_Pin_7;
				break;
			case TIM_Channel_3 :
				GPIO_Pin.Pin = GPIO_Pin_8;
				break;
			case TIM_Channel_4 :
				GPIO_Pin.Pin = GPIO_Pin_9;
				break;
		}
	}
	return GPIO_Pin;
}