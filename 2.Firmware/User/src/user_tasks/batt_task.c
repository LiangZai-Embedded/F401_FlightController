

#include "batt_task.h"

#define BATT_QUEUE_LEN  2
#define BATT_QUEUE_SIZE 8  


static void Batt_Task(void* pvParameters);


TaskHandle_t  BATT_Task_Handle = NULL;
QueueHandle_t BATT_Queue = NULL;




/**
  * @brief  BATTARY task create
  * @param  none
  * @retval none 
  * @date   2021-04-21
  */
void Batt_TaskCreate(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate((TaskFunction_t)Batt_Task,
	                      (const char*   )"Batt_Task",
                        (uint16_t      )128,
						            (void*         )NULL,
						            (UBaseType_t   )1,
						            (TaskHandle_t  )&BATT_Task_Handle);

  if(pdPASS == xReturn)
    DBG("Create BATT_Blink_Task OK!\r\n");
	
  BATT_Queue = xQueueCreate((UBaseType_t)BATT_QUEUE_LEN,(UBaseType_t)BATT_QUEUE_SIZE);
  if(NULL!=BATT_Queue)
	DBG("BATT_Queue created!\r\n");
}




/**
  * @brief  Start BATTARY Task 
  * @param  none
  * @retval none 
  * @date   2021-04-21
  */
void Batt_Task(void* pvParameters)
{
  float v1,v2 = 1.0f;
  float v_buf[2] = {0};

	static portTickType PreviousWakeTime;
	const  portTickType TimeIncrement = pdMS_TO_TICKS(200);

	while(1)
	{ 
    vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);

	  adc_get_voltage(&v1,&v2);

		v_buf[0] = v1;
		v_buf[1] = v2;

    xQueueSend(BATT_Queue,v_buf,0);
  }

}













