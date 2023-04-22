#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "headfiles.h"

#define BATT_ADC_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define BATT_ADC_GPIO_PORT        GPIOB

#define BATT_ADC_GPIO_PIN_3V3     GPIO_Pin_0
#define BATT_ADC_GPIO_PIN_5V      GPIO_Pin_1

#define BATT_ADC                  ADC1
#define BATT_ADC_CLK              RCC_APB2Periph_ADC1
#define BATT_ADC_CHANNEL_3V3      ADC_Channel_8
#define BATT_ADC_CHANNEL_5V       ADC_Channel_9





void adc_init(void);
void adc_get_voltage(float* v1,float* v2);


#endif 



