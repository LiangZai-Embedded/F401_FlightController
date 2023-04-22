

#include "led_task.h"




static void LED_Blink_Task(void* pvParameters);


TaskHandle_t LED_Blink_Task_Handle = NULL;





/**
  * @brief  LED task create
  * @param  none
  * @retval none 
	* @date   2021-1-9
  */

void LED_Blink_TaskCreate(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate((TaskFunction_t)LED_Blink_Task,
	                      (const char*   )"LED_Blink_Task",
                          (uint16_t      )128,
						  (void*         )NULL,
						  (UBaseType_t   )1,
						  (TaskHandle_t  )&LED_Blink_Task_Handle);

  if(pdPASS == xReturn)
    DBG("Create LED_Blink_Task OK!\r\n");
	

}




/**
  * @brief  Start LED Task 
  *         led_buf[0]: how many times will the led blink
  *			led_buf[1]: led glows during each blink
  *
  * @param  none
  * @retval none 
  * @date   2021-1-9
  */











