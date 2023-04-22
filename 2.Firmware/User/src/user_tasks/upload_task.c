
#include "upload_task.h"

#define PARAM_ADDR_QUEUE_LEN  1
#define PARAM_ADDR_QUEUE_SIZE 1  

TaskHandle_t  Upload_Task_Handle = NULL;
QueueHandle_t Param_Addr_Queue = NULL;



static void Upload_Task(void* pvParameters);


/**
  * @brief  ANO UPLOAD Task Create
  * @param  none
  * @retval none 
  * @date   2021-03-01
  */
void Upload_TaskCreate(void)
{	
   BaseType_t xReturn = pdPASS;


   xReturn = xTaskCreate((TaskFunction_t )Upload_Task, 
                        (const char*    )"Upload_Task",
                        (uint16_t       )512,  
                        (void*          )NULL,	 
                        (UBaseType_t    )2,	     
                        (TaskHandle_t*  )&Upload_Task_Handle);
  if(pdPASS == xReturn)
    DBG("Create Upload_Task  OK!\r\n");

  Param_Addr_Queue = xQueueCreate((UBaseType_t)PARAM_ADDR_QUEUE_LEN,(UBaseType_t)PARAM_ADDR_QUEUE_SIZE);
  if(NULL!=Param_Addr_Queue)
	DBG("Param_Addr_Queue created!\r\n");

}



#define USING_ANO 1
/**
  * @brief  upload Task
  *
  *    HEAD    +   DATA     + CHECK  + '\n'
  *  (1Byte)     (4Bytes)    (1Byte)   (1Byte) 
  *
  * @param  none
  * @retval none 
  * @date   2021-02-26
  */
static void Upload_Task(void* pvParameters)
{
  uint8_t addr = 0;
	uint8_t mode = 0;
	uint8_t send_flag = 0;
	
	float param_read = 0.0f;

  
	Axis6_t  axis6_upload;
	Euler3_t euler3_upload;
	float    mag_upload[3] = {0.0f};
	float    imu_upload_buf[12] = {0.0f};
	
	
	
  float v_buf[2] = {0};
  char temp_buf[7] = {0,0,0,0,0,0,0};
	char send_buf[21] = {0};            //copy 3 times


  static portTickType PreviousWakeTime;
  const  portTickType TimeIncrement = pdMS_TO_TICKS(5);


	while(1)
  {
    vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);
		
		
    #if(USING_ANO==1)
    {
			if(pdPASS==xQueueReceive(IMU_Queue,imu_upload_buf,0))
			{
				axis6_upload.ax     =  imu_upload_buf[0];
				axis6_upload.ay     =  imu_upload_buf[1];
				axis6_upload.az     =  imu_upload_buf[2];
				axis6_upload.gx     =  imu_upload_buf[3];
				axis6_upload.gy     =  imu_upload_buf[4];
				axis6_upload.gz     =  imu_upload_buf[5];
				mag_upload[0]       =  imu_upload_buf[6];
			  mag_upload[1]       =  imu_upload_buf[7];
				mag_upload[2]       =  imu_upload_buf[8];

				
				euler3_upload.roll  =  imu_upload_buf[9];
				euler3_upload.pitch =  imu_upload_buf[10];
				euler3_upload.yaw   =  imu_upload_buf[11];

        ano_report_imu(axis6_upload,euler3_upload,mag_upload);
				
        //ano_test_send(imu_upload_buf[9],imu_upload_buf[10],imu_upload_buf[11],0);

			}
		}
		
		
		
		
		
		
		
		
		
		
		
    #else
    {
			switch(mode)
			{	
				case 0:       if(pdPASS==xQueueReceive(BATT_Queue,v_buf,0))
			                {
			                	temp_buf[0] = VBAT_UPLOAD_HEAD;
			                	memcpy(&temp_buf[1],(uint8_t*)&v_buf[0],4);
												
			                	send_flag = 1;
			                }
											
											break;
			
        
				case 1:       if(pdPASS==xQueueReceive(IMU_Queue,imu_upload_buf,0))
			                {
			                		temp_buf[0] = IMU1_UP_HEAD;
			                		memcpy(&temp_buf[1],(uint8_t*)&imu_upload_buf[1],4);
			                
										      send_flag = 1;
											}
											
											break;

				
				case 2:       if(pdPASS==xQueueReceive(Param_Addr_Queue,&addr,0))
                      {
                         if(pdPASS==xSemaphoreTake(I2C_MuxSem_Handle, 0))
                         {
		                        param_read = AT24C02_param_get_one(addr);
		                        xSemaphoreGive(I2C_MuxSem_Handle);
                         
		  	                 	  temp_buf[0] = PARAM_UPLOAD_HEAD;
					                 	memcpy(&temp_buf[1],(uint8_t*)&param_read,4);
													 
											  		send_flag = 1;
												 }
											}
											
			  default:      break;
			 }
			
			 if(send_flag)
			 {
         send_flag = 0;
       	 
				 temp_buf[6] = '\n';
				 temp_buf[5] = (temp_buf[1]+temp_buf[2]+temp_buf[3]+temp_buf[4])^0xFF;

				 myreplace0(temp_buf,7);
				 myarrcpy(send_buf,(const char*)temp_buf,7,3);
            
				 USART_SendBuff(BLUETOOTH_USART,(const uint8_t*)send_buf,strlen(send_buf));
		  	    
				 memset(send_buf,0,sizeof(send_buf));
				 memset(temp_buf,0,sizeof(temp_buf));				 
			 }
		 }
				#endif
		if(++mode==3)
			mode = 0; 
  }
}
			


