#include "Board_LED.h"
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"

int main()
{
	
	//LED_Initialize();
	RCC_DeInit();
	
	GPIO_InitTypeDef Init;
	Init.GPIO_Pin = GPIO_Pin_4;
	Init.GPIO_Speed = GPIO_Speed_50MHz;
	Init.GPIO_Mode = GPIO_Mode_Out_PP;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//GPIO_AFIODeInit();
	GPIO_Init(GPIOA, &Init);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
	
	while (1)
	{
		//LED_On(0);

		//sleep(1);
		for (int i=0 ; i<5000000 ; i++);
		
		
		//LED_Off(0);
		//sleep(1);
		for (int i=0 ; i<5000000 ; i++);
	}
	
	return 0;
}
