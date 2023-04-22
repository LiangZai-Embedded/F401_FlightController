
  
#include "key.h"   

 /**
  * @brief  initialize KEY IO
  * @param  none
  * @date   2021-04-21
  * @retval none
  */
void KEY_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd ( KEY_GPIO_CLK,ENABLE); 

	GPIO_InitStructure.GPIO_Pin = KEY_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
}


		
	


/*********************************************END OF FILE**********************/
