
#include "remote.h"
 
/**
  * @brief  data process
  *	          
  *	raw_buf[0]: 0xA5 
  *                   
  *	raw_buf[1]: STORAGE:     0x00
  *                  RM:     0x01
  *             INQUARY:     0x02
            URGENT STOP:     0x03
  *             UNLOCK:      0x04
  *         RAW_UPLOAD:      0x05
  *	
  *    STORAGE or INQUARY: raw_buf[2]: PITCH_P_ADDR: 0x00
  *	  		                           PITCH_I_ADDR: 0x04
  *	  		                           PITCH_D_ADDR: 0x08
  *	  		                           ROLL_P_ADDR:  0x0C
  *	  		                           ROLL_I_ADDR:  0x10
  *	  	                               ROLL_D_ADDR:  0x14
  *	  	                               YAW_P_ADDR:   0x18
  *	  	                               YAW_I_ADDR:   0x1C
  *	  	                               YAW_D_ADDR:   0x20
  *	  		                           Q_ANGLE_ADDR: 0x24
  *	  		                           Q_GYRO_ADDR:  0x28
  *	  		                           R_ANGLE_ADDR: 0x2C
  *	  		                           M_ANGLE_ADDR: 0x30
  *	        
  *                     raw_buf[3-6]: 4bit float data (INQUARY 0)
  * 
  *    RM:  raw_buf[2-3]: throttle  
  *         raw_buf[4-6]: pitch,roll,yaw
  *      
  *	  
  *         raw_buf[7]:    check bit
  *         raw_buf[8]:    0x5A
  *						         
  *	@param  raw_buf: remote raw data buffer	
  * 
  * @retval RX_FALSE = 0,
  *         PROCESS_OK,
  *         PROCESS_FAILED
  * 
  * @date   2020-10-22
  */
RC_Status rc_data_process(uint8_t *raw_buf)
{
	BaseType_t xReturn = pdPASS;
    float data_write;
	float data_read;
	uint8_t check = 0;
	uint8_t cmd, addr;
	
	uint8_t temp_buf[4] = {0};
	int16_t rc_buf[4] = {0};

	if (raw_buf[0] != 0xA5 || raw_buf[8] != 0x5A)
	{
		DBG("data loss!!!\r\n");
		return RX_FALSE;
	}

	
	for (uint8_t i = 1; i < 7; i++)
		check += raw_buf[i];
	check = ~check + 1;

    if (check != raw_buf[7])
	{
		DBG("data check error!!!\r\n");
		return RX_FALSE;
	}

	cmd = raw_buf[1];

    switch (cmd)
	{
	  case STORAGE_CMD:  xReturn = xSemaphoreTake(I2C_MuxSem_Handle, 0);
	  	                 if (pdPASS == xReturn)
	  	                 {
	  					   addr = raw_buf[2];
	                       memcpy(temp_buf, &raw_buf[3], 4);
	                       data_write = *(float *)temp_buf;
	  	                   
	  					   AT24C02_param_save_one(addr, data_write);
	  	                   data_read = AT24C02_param_get_one(addr);
                 
	  	                   if(data_write == data_read)
	  	                   {
	  						  Parameters_Init();
	  						  xSemaphoreGive(I2C_MuxSem_Handle);

	  						  
	  						  return PROCESS_OK;
	  					   }
	  					   else
	  					   {
                               xSemaphoreGive(I2C_MuxSem_Handle);
	  	                	  return PROCESS_FAILED;

	  					   }
	  	                }
	  	                break;
	  					
	  
	  
	 case RC_CMD:        memcpy(temp_buf,&raw_buf[2],2);
	                     
	  					 rc_buf[0] = *(uint16_t *)temp_buf; //throttle
	                     rc_buf[1] =  (int16_t)raw_buf[4];  //pitch
	                     rc_buf[2] =  (int16_t)raw_buf[5];  //roll
	  				     rc_buf[3] =  (int16_t)raw_buf[6];  //yaw
                         xQueueSend(RC_Queue,rc_buf,0);
	  					
	  					 return PROCESS_OK;


	  									
	 case INQUIRY_CMD:   addr = raw_buf[2];
	                     xQueueSend(Param_Addr_Queue,&addr,0);

						 return PROCESS_OK;

	 
	 
	 
	 case URGENT_STOP_CMD: 	vTaskSuspend(Control_Task_Handle);
                            motor_stop();
							return PROCESS_OK;
	
	 
	 case UNLOCK_CMD:       vTaskResume(Control_Task_Handle);
	                        return PROCESS_OK;

	 case RAW_UPLOAD_CMD:   return PROCESS_OK;	
	 
	 default:           break;
	}


	return PROCESS_FAILED;
}





 
