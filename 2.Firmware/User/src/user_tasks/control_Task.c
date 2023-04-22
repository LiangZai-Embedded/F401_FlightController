
#include "control_task.h"


static void Control_Task(void* pvParameters);
TaskHandle_t Control_Task_Handle = NULL;


/**
  * @brief  Control Task Create
  * @param  none
  * @retval none 
  * @date   2020-10-22
  */
void Control_TaskCreate(void)
{	
   BaseType_t xReturn = pdPASS;


   xReturn = xTaskCreate((TaskFunction_t )Control_Task, 
                        (const char*    )"Control_Task",
                        (uint16_t       )1024,  
                        (void*          )NULL,	 
                        (UBaseType_t    )4,	     
                        (TaskHandle_t*  )&Control_Task_Handle);
  if(pdPASS == xReturn)
    DBG("Create Control_Task  OK!\r\n");

}


/**
  * @brief  Controll Task
  * @param  none
  * @retval none 
  * @date   2021-1-8
  * @note   add vTaskDelayUntil func
  */
static void Control_Task(void* pvParameters)
{
  BaseType_t xReturn = pdPASS;
  
  int16_t    ctrl_buf[4] = {0};
  float      imu_final_buf[6] =  {0.0f};
  
  float      pitch_cur = 0.0f,roll_cur = 0.0f,yaw_cur = 0.0f;
  float      gx_cur = 0.0f, gy_cur = 0.0f, gz_cur = 0.0f;
  
  float     pid_out_pitch=0.0f, pid_out_roll=0.0f,pid_in_pitch=0.0f,pid_in_roll=0.0f,pid_in_yaw=0.0f;

  int16_t    throttle = 0,pitch_target = 0,roll_target = 0,yaw_target = 0;  
  
  static portTickType PreviousWakeTime;
  const  portTickType TimeIncrement = pdMS_TO_TICKS(5);
	


	
  while(1)
  {
    vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);
    
    
    
    xReturn = xQueueReceive(RC_Queue,ctrl_buf,0);
    if(xReturn==pdPASS)
    {
      throttle      = ctrl_buf[0];
    	pitch_target  = ctrl_buf[1];
    	roll_target   = ctrl_buf[2];
    	yaw_target    = ctrl_buf[3];
    }
    
    
    
    xReturn = xQueueReceive(Euler_Queue,imu_final_buf,0);
    if(xReturn==pdPASS)
    {
      pitch_cur = imu_final_buf[0];
      roll_cur  = imu_final_buf[1];
      yaw_cur   = imu_final_buf[2];
      gx_cur    = imu_final_buf[3];
      gy_cur    = imu_final_buf[4];
      gz_cur    = imu_final_buf[5];
    }
    
    pid_out_roll = pid_update(&roll_outer,roll_cur,roll_target,throttle);
    //pid_out_roll = roll_target;
		pid_in_roll = pid_update(&roll_inner,gy_cur,pid_out_roll,throttle);

   // pitch_pwm = pid_ctrl(&pitch_outer,pitch_cur,pitch_target,throttle);
    
    motor_ctrl(throttle,pid_in_pitch,pid_in_roll,pid_in_yaw);
    }
}




