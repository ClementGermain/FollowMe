#include "gpio.h"

void Init_GPIO(GPIO_TypeDef * GPIO, uint16_t Pin, GPIOMode_TypeDef Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = Mode;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
  GPIO_Init(GPIO, &GPIO_InitStructure);
}

void Init_GPIO_Out(GPIO_TypeDef * GPIO, uint16_t Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
  GPIO_Init(GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO,Pin);
}

void Init_GPIO_In(GPIO_TypeDef * GPIO, int Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
  GPIO_Init(GPIO, &GPIO_InitStructure); 
}


void Init_GPIO_IPU(GPIO_TypeDef * GPIO, int Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
  GPIO_Init(GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO,Pin);
}

void Init_GPIO_PWM(GPIO_TypeDef * GPIO, uint16_t Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
  GPIO_Init(GPIO, &GPIO_InitStructure); 
}

void Toggle_LED(GPIO_TypeDef * GPIO, uint16_t Pin){
	if (GPIO_ReadOutputDataBit(GPIO, Pin) == Bit_RESET)
		GPIO_SetBits(GPIO, Pin);
	else
		GPIO_ResetBits(GPIO, Pin);
}

void Send_impulse_GPIO(GPIO_TypeDef * GPIO, uint16_t Pin, uint16_t impulse_time_us){
	GPIO_ResetBits(GPIO, Pin);
	GPIO_SetBits(GPIO, Pin);
	Delay_us(impulse_time_us);
	GPIO_ResetBits(GPIO, Pin);
}
