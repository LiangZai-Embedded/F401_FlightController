#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H


#include "headfiles.h"

#define BLUETOOTH_STATE_PIN                  GPIO_Pin_13                 
#define BLUETOOTH_STATE_GPIO_PORT            GPIOC                           
#define BLUETOOTH_STATE_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define READ_STATE()               GPIO_ReadInputDataBit(BLUETOOTH_STATE_GPIO_PORT,BLUETOOTH_STATE_PIN)

void bluetooth_get_config(void);
void USART_Config_38400(void);
void USART_Config_Ui(void);
void bluetooth_init(void);

 

#endif
