#include "adc.h"

void ADC_Configuration(ADC_TypeDef * ADC)
{
  ADC_InitTypeDef  ADC_InitStructure;
  /* PCLK2 is the APB2 clock */
  /* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
  ADC_DeInit(ADC);

  /* ADC Configuration ------------------------------------------------------*/
  /* ADC1 and ADC2 operate independently */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  /* Disable the scan conversion so we do one at a time */
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  /* Don't do contimuous conversions - do them on demand */
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  /* Start conversin by software, not an external trigger */
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  /* Conversions are 12 bit - put them in the lower 12 bits of the result */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  /* Say how many channels would be used by the sequencer */
  ADC_InitStructure.ADC_NbrOfChannel = 1;

  /* Now do the setup */
  ADC_Init(ADC, &ADC_InitStructure);
  /* Enable ADC */
  ADC_Cmd(ADC, ENABLE);

  /* Enable ADC reset calibaration register */
  ADC_ResetCalibration(ADC);
  /* Check the end of ADC reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC));
  /* Start ADC calibaration */
  ADC_StartCalibration(ADC);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC));
}

u16 ADC_Read(ADC_TypeDef * ADC, int Channel)
{
  ADC_RegularChannelConfig(ADC, Channel, 1, ADC_SampleTime_28Cycles5);
  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC, ENABLE);
  // Wait until conversion completion
  while(ADC_GetFlagStatus(ADC, ADC_FLAG_EOC) == RESET);
  // Get the conversion value
  return ADC_GetConversionValue(ADC);
}
