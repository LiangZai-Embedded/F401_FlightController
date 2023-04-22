
#include "rc_task.h"


#define RC_QUEUE_LEN  4
#define RC_QUEUE_SIZE 8


TaskHandle_t RC_Task_Handle = NULL;
QueueHandle_t RC_Queue = NULL;


static void RC_Task(void* parameter);



/**
  * @brief  RC Task Create
  * @param  none
  * @retval none 
	* @date   2020-10-22
  */
void RC_TaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
	xReturn = xTaskCreate((TaskFunction_t )RC_Task, 
                        (const char*    )"RC_Task",
                        (uint16_t       )1024,  
                        (void*          )NULL,	 
                        (UBaseType_t    )3,	     
                        (TaskHandle_t*  )&RC_Task_Handle);
  if(pdPASS == xReturn)
    DBG("Create RC_Task  OK!\r\n");
	
	
	RC_Queue = xQueueCreate((UBaseType_t)RC_QUEUE_LEN,(UBaseType_t)RC_QUEUE_SIZE);
	if(NULL!=RC_Queue)
		DBG("RC_Queue created!\r\n");
	
}





/**
  * @brief  Start RC Task
  * @param  none
  * @retval none 
  * @date   2020-10-22
  */
static void RC_Task(void* parameter)
{
  BaseType_t xReturn = pdPASS;
  
  uint8_t      sta;
	uint8_t      rc_raw_buf[USART_RBUFF_SIZE];
  uint16_t     led_buf[2] = {0};
	FlagStatus   bluetooth_sta = RESET;
	FlagStatus   bluetooth_sta_last = RESET;

  
  
	
  static portTickType PreviousWakeTime;
  const  portTickType TimeIncrement = pdMS_TO_TICKS(50);
	

	
  while (1)
  {
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);

    
    if(READ_STATE()==SET)
			bluetooth_sta = SET;
		
		else
		  bluetooth_sta = RESET;
    
		if(bluetooth_sta != bluetooth_sta_last)
		{
			if(bluetooth_sta==SET)
			{
				vTaskResume(Control_Task_Handle);
			}
      else
			{
				vTaskSuspend(Control_Task_Handle);
        motor_stop();
			}
		}
		
		bluetooth_sta_last = bluetooth_sta; 
    
    xReturn = xSemaphoreTake(BluetoothReceiveSem_Handle,0); 
    if(pdTRUE==xReturn)
    {
    	memcpy(rc_raw_buf,bluetooth_rx_buf,USART_RBUFF_SIZE);
    	
    	sta = (uint8_t)ano_data_process(rc_raw_buf);
    	
      switch(sta)
    	{
    	   case PROCESS_OK:     led_buf[0] = 1;
    		                      led_buf[1] = 30;
    			                    xQueueSend(LED_Queue,led_buf,0);   
    		                          
                              break;
    		
    	  
        
        
        case PROCESS_FAILED:  led_buf[0] = 2;				
                              led_buf[1] = 600;
                              xQueueSend(LED_Queue,led_buf,0);
    						            
                              break;
    		
        
        
        
        case  RX_FALSE:       led_buf[0] = 1;
                              led_buf[1] = 600;
                              xQueueSend(LED_Queue,led_buf,0);
    						            
                              break;  

		    default:            break;      
    	} 
	  }
	}
}



