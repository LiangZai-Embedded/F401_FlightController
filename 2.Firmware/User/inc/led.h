#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

#define LED_GREEN_PIN                GPIO_Pin_5                 
#define LED_GREEN_GPIO_PORT          GPIOB                      
#define LED_GREEN_GPIO_CLK           RCC_AHB1Periph_GPIOB

#define LED_RED_PIN                  GPIO_Pin_6                 
#define LED_RED_GPIO_PORT            GPIOB                      
#define LED_RED_GPIO_CLK             RCC_AHB1Periph_GPIOB

#define	digitalHi(p,i)			 {p->BSRRL=i;}		
#define digitalLo(p,i)			 {p->BSRRH=i;}		
#define digitalToggle(p,i)	 {p->ODR ^=i;}	

#define LED_GREEN_TOGGLE()		digitalToggle(LED_GREEN_GPIO_PORT,LED_GREEN_PIN)
#define LED_GREEN_ON()			 digitalHi(LED_GREEN_GPIO_PORT,LED_GREEN_PIN)
#define LED_GREEN_OFF()				digitalLo(LED_GREEN_GPIO_PORT,LED_GREEN_PIN)

#define LED_RED_TOGGLE()		digitalToggle(LED_RED_GPIO_PORT,LED_RED_PIN)
#define LED_RED_ON()			 digitalHi(LED_RED_GPIO_PORT,LED_RED_PIN)
#define LED_RED_OFF()				digitalLo(LED_RED_GPIO_PORT,LED_RED_PIN)

void LED_GPIO_Config(void);
void LED_Blink(uint16_t* led_buf,uint8_t task_ms);


#endif 
