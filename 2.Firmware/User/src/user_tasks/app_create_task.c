#include "app_create_task.h"

static void AppTaskCreate(void);

TaskHandle_t AppTaskCreate_Handle = NULL;

SemaphoreHandle_t USART_MuxSem_Handle = NULL;
SemaphoreHandle_t I2C_MuxSem_Handle = NULL;
SemaphoreHandle_t BluetoothReceiveSem_Handle = NULL;


/**
  * @brief  Start FreeRTOS
  * @param  none
  * @retval Never return if success  
  * @date   2020-06-28
  */
bool OS_Start(void)
{
	BaseType_t xReturn = pdPASS;
	
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  
                        (const char*    )"AppTaskCreate",
                        (uint16_t       )512,  
                        (void*          )NULL,
                        (UBaseType_t    )7,
                        (TaskHandle_t*  )&AppTaskCreate_Handle);
           
  
	if(pdPASS == xReturn)
	{
	  DBG("Starting OS!\r\n");   

		vTaskStartScheduler();
	}
 
  return FALSE;
}  


/**
  * @brief  create app task 
  *         creat i2c mutex semphore
  *         creat usart mutex semphore
  *         creat bluetooth received semphore
  *         creat motor controll mutex semphore
  * @param  none
  * @retval none 
  * @date   2020-06-28
  */

static void AppTaskCreate(void)
{
	FileSystem_Init();

  taskENTER_CRITICAL();          
  
  RC_TaskCreate();
  IMU_TaskCreate();
  Control_TaskCreate();
  Upload_TaskCreate();
  Batt_TaskCreate();

   

  BluetoothReceiveSem_Handle = xSemaphoreCreateBinary();	
  if(NULL!=BluetoothReceiveSem_Handle)
  	DBG("BluetoothReceiveSemphore created!\r\n");

  
  USART_MuxSem_Handle = xSemaphoreCreateMutex();	
  if(NULL!=USART_MuxSem_Handle)
  	DBG("USART_MuxSem_Handle created!\r\n");
    
  
  I2C_MuxSem_Handle = xSemaphoreCreateMutex();	
  if(NULL!=I2C_MuxSem_Handle)
  	DBG("I2C_MuxSem_Handle created!\r\n");
	
  vTaskSuspend(Control_Task_Handle);
  motor_stop();
	
	LED_RED_OFF();	

  vTaskDelete(AppTaskCreate_Handle); 
  	

  taskEXIT_CRITICAL(); 

}













