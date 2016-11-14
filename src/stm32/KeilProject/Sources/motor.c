#include "motor.h"
#include "delay.h"

void Update_Motors(BarstowControl_Typedef * Control){
	
	switch (Control->propulsionMotor.direction){
		case MOTOR_DIRECTION_STOP:
			Stop_Car();
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			break;
		case MOTOR_DIRECTION_BACKWARD:
			Go_Back(Control->propulsionMotor.speed);
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			break;
		case MOTOR_DIRECTION_FORWARD:
			Go_Forward(Control->propulsionMotor.speed);
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			break;
	}
			
	switch(Control->directionMotor.direction){
		case MOTOR_DIRECTION_STOP:
			Stop_Turn();
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			break;
		case MOTOR_DIRECTION_RIGHT:
			Turn_Right(Control->directionMotor.speed);
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
		break;
		case MOTOR_DIRECTION_LEFT:
			Turn_Left(Control->directionMotor.speed);
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
		break;
	}
}

void Init_All_Motor(void){
	// Put a hight level on GPIO_Pin_Motor_Enable to enable motor's driver
	Init_GPIO_Out(GPIO_Motor_Enable, GPIO_Pin_Motor_Enable);
	GPIO_SetBits(GPIO_Motor_Enable, GPIO_Pin_Motor_Enable);
	
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

void Motor_Test(void){
    Turn_Left(0.3);
		Delay();
    Turn_Right(0.3);
		Delay();
		Stop_Turn();
		Delay();
		Turn_Left(0.5);
		Delay();
    Turn_Right(0.5);
		Delay();
		Stop_Turn();
		Delay();
		Go_Forward(0.3);
		Delay();
    Go_Back(0.3);
		Delay();
		Stop_Car();
		Delay();
		Go_Forward(0.5);
		Delay();
    Go_Back(0.5);
		Delay();
		Stop_Car();
		Delay();
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
