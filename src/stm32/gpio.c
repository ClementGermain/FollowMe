#include "gpio.h"

void Init_GPIO(GPIO_TypeDef * GPIO, int Pin, GPIOMode_TypeDef Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* GPIOA Configuration:TIMx Channel1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
  GPIO_Init(GPIO, &GPIO_InitStructure);
 
}


void Init_GPIO_Out(GPIO_TypeDef * GPIO, int Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* GPIOA Configuration:TIMx Channel1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
  GPIO_Init(GPIO, &GPIO_InitStructure);
 
}

void Init_GPIO_PWM(GPIO_TypeDef * GPIO, int Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* GPIOA Configuration:TIMx Channel1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
  GPIO_Init(GPIO, &GPIO_InitStructure);
 
}
