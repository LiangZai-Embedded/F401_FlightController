
#include "adc.h"


static void BATT_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(BATT_ADC_GPIO_CLK, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = BATT_ADC_GPIO_PIN_3V3|BATT_ADC_GPIO_PIN_5V;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	
  GPIO_Init(BATT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

static void BATT_ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  
	RCC_APB2PeriphClockCmd(BATT_ADC_CLK , ENABLE);

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                    
  ADC_Init(BATT_ADC, &ADC_InitStructure);
	
	
  ADC_ClearFlag(BATT_ADC, ADC_FLAG_EOC);

  ADC_Cmd(BATT_ADC, DISABLE);  
  
}


void adc_init(void)
{
	BATT_ADC_GPIO_Config();
	BATT_ADC_Mode_Config();
}

/**
  * @brief  read 2 voltages    3.3v  5v normally
  * @param  v1,v2: data ptr
  * @retval none  
	* @date   2021-04-20
  */
void adc_get_voltage(float* v1,float* v2)
{
  uint16_t raw;

  ADC_Cmd(BATT_ADC, ENABLE);  
  
  ADC_RegularChannelConfig(BATT_ADC, BATT_ADC_CHANNEL_3V3, 1, ADC_SampleTime_56Cycles);
  ADC_SoftwareStartConv(BATT_ADC);
  while(ADC_GetFlagStatus(BATT_ADC,ADC_FLAG_EOC)!=SET){}
  ADC_ClearFlag(BATT_ADC, ADC_FLAG_EOC);

  raw = ADC_GetConversionValue(BATT_ADC);
  *v1 = (float)raw / 4096.0f * 3.3f;



  ADC_RegularChannelConfig(BATT_ADC, BATT_ADC_CHANNEL_5V, 1, ADC_SampleTime_56Cycles);
  ADC_SoftwareStartConv(BATT_ADC);
  while(ADC_GetFlagStatus(BATT_ADC,ADC_FLAG_EOC)!=SET){}
  ADC_ClearFlag(BATT_ADC, ADC_FLAG_EOC);

  raw = ADC_GetConversionValue(BATT_ADC);
  *v2 = (float)raw / 4096.0f * 3.3f;
		
	ADC_Cmd(BATT_ADC,DISABLE);

}



