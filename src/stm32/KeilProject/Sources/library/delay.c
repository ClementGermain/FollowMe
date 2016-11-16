#include "delay.h"

void Delay() {
  for (int n=0; n < 10000000; n++);
}
 

//FONCTION A TESTER
void Delay_ms(int time_us){
	// 1 FOR = 4 instructions
	//Fq Clock = 72MHz
	for (int n=0; n < (time_us*72/4); n++);
}

/*
extern uint32_t SystemCoreClock;
void DWT_Init(void) 
{
  if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) 
  {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  }
}
 
uint32_t DWT_Get(void)
{
  return DWT->CYCCNT;
}
 
__inline
uint8_t DWT_Compare(int32_t tp)
{
  return (((int32_t)DWT_Get() - tp) < 0);
}
 
void DWT_Delay(uint32_t us) // microseconds
{
  int32_t tp = DWT_Get() + us * (SystemCoreClock/1000000);
  while (DWT_Compare(tp));
}*/



