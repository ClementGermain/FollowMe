#include "motor.h"

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
	// A dealy function would be usefull here
}