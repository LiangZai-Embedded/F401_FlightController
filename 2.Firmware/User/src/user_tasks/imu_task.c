

#include "imu_task.h"

#define IMU_QUEUE_LEN  12
#define IMU_QUEUE_SIZE 48

#define Euler_Queue_LEN  6
#define Euler_Queue_SIZE 24

#define LED_QUEUE_LEN  2
#define LED_QUEUE_SIZE 4  


#define PITCH_LIMIT   120.0f
#define ROLL_LIMIT    120.0f


QueueHandle_t IMU_Queue = NULL;
QueueHandle_t Euler_Queue = NULL;
QueueHandle_t LED_Queue = NULL;

TaskHandle_t IMU_Task_Handle = NULL;

static void IMU_Task(void* pvParameters);


/**
  * @brief  IMU Task Create
  * @param  none
  * @retval none 
	* @date   2020-10-22
  */
void IMU_TaskCreate(void)
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate((TaskFunction_t)IMU_Task,
	                      (const char*   )"IMU_Task",
                          (uint16_t      )2048,
						  (void*         )NULL,
						  (UBaseType_t   )5,
						  (TaskHandle_t  )&IMU_Task_Handle);

  if(pdPASS == xReturn)
    DBG("Create IMU_Task OK!\r\n");
	
	IMU_Queue = xQueueCreate((UBaseType_t)IMU_QUEUE_LEN,(UBaseType_t)IMU_QUEUE_SIZE);
	if(NULL!=IMU_Queue)
		DBG("IMU_Queue created!\r\n");
	
	Euler_Queue = xQueueCreate((UBaseType_t)Euler_Queue_LEN,(UBaseType_t)Euler_Queue_SIZE);
	if(NULL!=Euler_Queue)
		DBG("Euler_Queue created!\r\n");
  
  LED_Queue = xQueueCreate((UBaseType_t)LED_QUEUE_LEN,(UBaseType_t)LED_QUEUE_SIZE);
	if(NULL!=LED_Queue)
		DBG("LED_Queue created!\r\n");
}

/**
  * @brief  Start IMU Task
  * @param  none
  * @retval none 
  * @date   2020-06-28
  */
int t = 0;
void IMU_Task(void* pvParameters)
{
  Axis6_t  axis6_raw,axis6_lpf;
  Euler3_t  euler3_mahony,euler3_raw,euler3_lpf;

  float imu_upload_buf[12] = {0.0f};
  float imu_control_buf[6] = {0.0f};
  char sd_buf[10] = {0}; 
	float mag[3] = {0.0f};
  
 

	uint8_t led_flag = 0;
	uint32_t tick = 0;
	uint32_t toggle = 0;
	uint16_t led_buf[2] = {0};
  
	int cnt = 0;
	int sd_flag = 0;
	
	static portTickType PreviousWakeTime;
  const  portTickType TimeIncrement = pdMS_TO_TICKS(2);

	
	//motor_pwm_out(600,600,600,600);
	while(1)
  { 
  	vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);  
    
	  cnt_timer_start();


    //calculate
    icm20602_get_data(&axis6_raw);
		axis6_raw.gx = DEGTORAD(axis6_raw.gx);
		axis6_raw.gy = DEGTORAD(axis6_raw.gy);
		axis6_raw.gz = DEGTORAD(axis6_raw.gz);
    
		biquad_filter_apply(&axis6_raw,&axis6_lpf);
    
		imu_get_eula(axis6_lpf,&euler3_lpf);
    imu_update(axis6_lpf,&euler3_mahony); 

		

		
		
     if(xSemaphoreTake(I2C_MuxSem_Handle,portMAX_DELAY)==pdPASS)
     {
       hmc5883l_get_data(&euler3_mahony); 
			 //hmc5883l_read_mxyz(mag);
			 //euler3_mahony.yaw = hmc5883l_read();
       xSemaphoreGive(I2C_MuxSem_Handle);
     }  		
    	
		
    imu_upload_buf[0] = axis6_lpf.ax;   //data upload
    imu_upload_buf[1] = axis6_lpf.ay; 
    imu_upload_buf[2] = axis6_lpf.az; 
    
		imu_upload_buf[3] = axis6_lpf.gx; 
    imu_upload_buf[4] = axis6_lpf.gy; 
    imu_upload_buf[5] = axis6_lpf.gz; 
    
		imu_upload_buf[6] = mag[0];
    imu_upload_buf[7] = mag[1];
    imu_upload_buf[8] = mag[2];
		 
		 

    
		imu_upload_buf[9] = euler3_mahony.roll;
    imu_upload_buf[10] = euler3_mahony.pitch;
    imu_upload_buf[11] = euler3_mahony.yaw;
    
		xQueueSend(IMU_Queue,imu_upload_buf,0);  

		 
		
		//send to control task
    imu_control_buf[0] = euler3_mahony.pitch;
    imu_control_buf[1] = euler3_mahony.roll;
    imu_control_buf[2] = euler3_mahony.yaw;
    imu_control_buf[3] = axis6_lpf.gx;
    imu_control_buf[4] = axis6_lpf.gy;
    imu_control_buf[5] = axis6_lpf.gz;
		
	  if(imu_control_buf[0]>PITCH_LIMIT || 
       imu_control_buf[0]<-PITCH_LIMIT||
       imu_control_buf[1]>ROLL_LIMIT  || 
       imu_control_buf[1]<-ROLL_LIMIT)
    {
      vTaskSuspend(Control_Task_Handle);
      motor_stop();
    }  
    xQueueSend(Euler_Queue,imu_control_buf,0);  

    
	  
		
    
		    
//		if(sd_flag==0)	  
//		{
//			float2str(sd_buf,axis6_raw.ax,3);
//			sd_buf[9] = '\t';
//		  f_write(&fnew,sd_buf,sizeof(sd_buf),&fnum);
//			
//			float2str(sd_buf,axis6_raw.ay,3);
//			sd_buf[9] = '\t';
//		  f_write(&fnew,sd_buf,sizeof(sd_buf),&fnum);
//			
//			float2str(sd_buf,axis6_raw.az,3);
//			sd_buf[9] = '\t';
//		  f_write(&fnew,sd_buf,sizeof(sd_buf),&fnum);
//			
//			float2str(sd_buf,axis6_raw.gx,3);
//			sd_buf[9] = '\t';
//		  f_write(&fnew,sd_buf,sizeof(sd_buf),&fnum);
//			
//			float2str(sd_buf,axis6_raw.gy,3);
//			sd_buf[9] = '\t';
//		  f_write(&fnew,sd_buf,sizeof(sd_buf),&fnum);
//			
//			float2str(sd_buf,axis6_raw.gz,3);
//			sd_buf[9] = '\t';
//		  f_write(&fnew,sd_buf,sizeof(sd_buf),&fnum);
//			
//			float2str(sd_buf,euler3_lpf.roll,3);
//			sd_buf[9] = '\t';
//		  f_write(&fnew,sd_buf,sizeof(sd_buf),&fnum);
//			
//			float2str(sd_buf,euler3_mahony.roll,3);
//			sd_buf[9] = '\n';
//		  f_write(&fnew,sd_buf,sizeof(sd_buf),&fnum);
//		}
//    

//		
//		if(++cnt==4096)
//		{
//		  sd_flag = 1;
//			f_close(&fnew);
//			LED_GREEN_ON();
//			motor_pwm_out(0,0,0,0);
//		}


		
		
		
		//control led to show any exception
		
		if(xQueueReceive(LED_Queue,(uint16_t*)led_buf,0))
		{
			led_flag = 1;
		}
		
		if(led_flag==1)
		{
			if(++tick==led_buf[1]/2)
		  {
			  tick = 0;
				LED_GREEN_TOGGLE();
			  if(++toggle==led_buf[0]*2)
			  {
				  toggle = 0;
			    
				  led_flag = 0;
			  }
			}
		}
		
		  t = cnt_timer_get_us();
      __nop();

		
	}
}

		
		
		
		
		

