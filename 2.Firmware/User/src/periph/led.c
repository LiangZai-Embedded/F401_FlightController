
  
#include "led.h"   

 /**
  * @brief  initialize LED IO
  * @param  none
  * @retval none
  */
void LED_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd ( LED_GREEN_GPIO_CLK,ENABLE); 
  RCC_AHB1PeriphClockCmd ( LED_RED_GPIO_CLK,ENABLE); 

	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN|LED_RED_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

	GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStructure);

  LED_GREEN_OFF();
  LED_RED_OFF();	
}

/**
  * @brief  LED blink
  * @param  led_buf[0]: blink times
  *         led_buf[1]: blink delay ms
  *            task_ms: LED_Blink() execute T
  * @retval none 
	* @date   2021-12-29
  */
void LED_Blink(uint16_t* led_buf,uint8_t task_ms)
{
	


}






		
	


