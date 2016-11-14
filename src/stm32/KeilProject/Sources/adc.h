#ifndef __ADC__
#define __ADC__

#include "include.h"

/** @brief Init an ADC
	* @param ADC: ADC to get init (ADC1 or ADC2)
	*	@retval None
*/
void ADC_Configuration(ADC_TypeDef * ADC);
	
/** @brief Read an ADC value
	* @param ADC: ADC to read (ADC1 or ADC2)
	* @param Channel: Channel of the ADC to read (ex ADC_Channel_6);
	*	@retval u16: integer value of the ADC value. Value goes 0 to 4096
*/
u16 Read_ADC(ADC_TypeDef * ADC, int Channel);

#endif

