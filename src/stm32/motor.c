#include "motor.h"

void Init_PWM_All_Motor(void){
	Init_PWM_Single_Motor(TIM_Forward, TIM_Channel_Forward);
	Init_PWM_Single_Motor(TIM_Backward, TIM_Channel_Backward);
	Init_PWM_Single_Motor(TIM_Right, TIM_Channel_Right);
	Init_PWM_Single_Motor(TIM_Left, TIM_Channel_Left);
}

void Init_PWM_Single_Motor(TIM_TypeDef* TIM, int Channel){
	GPIO_Pin_TypeDef GPIO_Pin = Find_GPIO_Pin(TIM, Channel);
	Init_GPIO_PWM(GPIO_Pin.GPIO,GPIO_Pin.Pin);
	Init_PWM(TIM, 0, Channel);
}

void Turn_Left(int Rot){
	Set_PWM_DutyCycle(TIM_Right, 0, TIM_Channel_Right);
	Set_PWM_DutyCycle(TIM_Left, Calcul_DutyCycle(Rot), TIM_Channel_Left);
}

void Turn_Right(int Rot){
	Set_PWM_DutyCycle(TIM_Left, 0, TIM_Channel_Left);
	Set_PWM_DutyCycle(TIM_Right, Calcul_DutyCycle(Rot), TIM_Channel_Right);
}

void Stop_Turn(void){
	Set_PWM_DutyCycle(TIM_Left, 0, TIM_Channel_Left);
	Set_PWM_DutyCycle(TIM_Right, 0, TIM_Channel_Right);
}

void Reset_Direction(void){ 
	// We need to figure it how to do this !!!!
	// Do we really need it ?
}

void Go_Forward(int Speed){
	Set_PWM_DutyCycle(TIM_Backward, 0, TIM_Channel_Backward);
	Set_PWM_DutyCycle(TIM_Forward, Calcul_DutyCycle(Speed), TIM_Channel_Forward);
}

void Go_Backward(int Speed){
	Set_PWM_DutyCycle(TIM_Forward, 0, TIM_Channel_Forward);
	Set_PWM_DutyCycle(TIM_Backward, Calcul_DutyCycle(Speed), TIM_Channel_Backward);
}

void Stop_Car(void){
	Set_PWM_DutyCycle(TIM_Forward, 0, TIM_Channel_Forward);
	Set_PWM_DutyCycle(TIM_Backward, 0, TIM_Channel_Backward);
}

int Calcul_DutyCycle(int Value){
	if (Value >= 0 && Value <= 1)
		return Value * MaxDutyCycle;
	else
		return 0;
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
			case TIM_Channel_2 :
				GPIO_Pin.Pin = GPIO_Pin_9;
			case TIM_Channel_3 :
				GPIO_Pin.Pin = GPIO_Pin_10;
			case TIM_Channel_4 :
				GPIO_Pin.Pin = GPIO_Pin_11;
		}
	}
	else if (TIM == TIM2){
		GPIO_Pin.GPIO = GPIOA;
		switch (Channel){
			case TIM_Channel_1 :
				GPIO_Pin.Pin = GPIO_Pin_0;
			case TIM_Channel_2 :
				GPIO_Pin.Pin = GPIO_Pin_1;
			case TIM_Channel_3 :
				GPIO_Pin.Pin = GPIO_Pin_2;
			case TIM_Channel_4 :
				GPIO_Pin.Pin = GPIO_Pin_3;
		}
	}
	else if (TIM == TIM3){
		switch (Channel){
			case TIM_Channel_1 :
			GPIO_Pin.GPIO = GPIOA;
				GPIO_Pin.Pin = GPIO_Pin_6;
			case TIM_Channel_2 :
			GPIO_Pin.GPIO = GPIOA;
				GPIO_Pin.Pin = GPIO_Pin_7;
			case TIM_Channel_3 :
			GPIO_Pin.GPIO = GPIOB;
				GPIO_Pin.Pin = GPIO_Pin_0;
			case TIM_Channel_4 :
				GPIO_Pin.GPIO = GPIOB;
				GPIO_Pin.Pin = GPIO_Pin_1;
		}
	}
	else if (TIM == TIM4){
		GPIO_Pin.GPIO = GPIOB;
		switch (Channel){
			case TIM_Channel_1 :
				GPIO_Pin.Pin = GPIO_Pin_6;
			case TIM_Channel_2 :
				GPIO_Pin.Pin = GPIO_Pin_7;
			case TIM_Channel_3 :
				GPIO_Pin.Pin = GPIO_Pin_8;
			case TIM_Channel_4 :
				GPIO_Pin.Pin = GPIO_Pin_9;
		}
	}
	return GPIO_Pin;
}
