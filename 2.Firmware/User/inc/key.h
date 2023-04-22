#ifndef __KEY_H
#define	__KEY_H

#include "headfiles.h"



#define KEY_PIN                  GPIO_Pin_14                 
#define KEY_GPIO_PORT            GPIOC                           
#define KEY_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define READ_KEY()               GPIO_ReadInputDataBit(KEY_GPIO_PORT,KEY_PIN)
#define ON                       0

void KEY_GPIO_Config(void);

#endif 
