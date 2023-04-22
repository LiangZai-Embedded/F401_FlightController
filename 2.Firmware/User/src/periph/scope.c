
#include "scope.h"



#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )


void ano_test_send(float f0,float f1,float f2,float f3)
{
  uint8_t testdatatosend[50];	//发送数据缓存
  uint8_t _cnt = 0;
  uint8_t i; 
  uint8_t sum = 0;
  BaseType_t xReturn = pdPASS;
	
  testdatatosend[_cnt++]=ANO_FC_HEAD;
  testdatatosend[_cnt++]=0xA1;//数据帧1  数据位是f0/f1/f2/f3
  testdatatosend[_cnt++]=0;

  testdatatosend[_cnt++] = BYTE3(f0);
  testdatatosend[_cnt++] = BYTE2(f0);
  testdatatosend[_cnt++] = BYTE1(f0);
  testdatatosend[_cnt++] = BYTE0(f0);
		
  testdatatosend[_cnt++] = BYTE3(f1);
  testdatatosend[_cnt++] = BYTE2(f1);
  testdatatosend[_cnt++] = BYTE1(f1);
  testdatatosend[_cnt++] = BYTE0(f1);
	
  testdatatosend[_cnt++] = BYTE3(f2);
  testdatatosend[_cnt++] = BYTE2(f2);
  testdatatosend[_cnt++] = BYTE1(f2);
  testdatatosend[_cnt++] = BYTE0(f2);  
  testdatatosend[_cnt++] = BYTE3(f3);
  testdatatosend[_cnt++] = BYTE2(f3);
  testdatatosend[_cnt++] = BYTE1(f3);
  testdatatosend[_cnt++] = BYTE0(f3);
  
  testdatatosend[2] = _cnt-3;
		
  for(i=0;i<_cnt;i++)
		sum += testdatatosend[i];
	
  testdatatosend[_cnt++]=sum;	//将sum校验数据放置最后一字节

  xReturn = xSemaphoreTake(USART_MuxSem_Handle, 0);
  
  if (pdPASS == xReturn)
  {
		//USART_SendBuff(BLUETOOTH_USART,(const uint8_t*)testdatatosend,_cnt);
	  //USARTx_DMA_tx((const char*)testdatatosend,_cnt);
    xSemaphoreGive(USART_MuxSem_Handle);
  }
	
}



/**  
  *  功能：发送数据给匿名上位机（V2.6）
  *  入口参数：fun,功能字，0xA0~0xAF
  *			       data，数据缓存区，最多28个字节
  *			       len，data数据长度
  *  返回值：无
  *  注：数据格式：0x88+FUN+LEN+DATA+SUM
  */
void ano_report(uint8_t fun,uint8_t *data,uint8_t len)
{
	uint8_t send_buf[32]={0};
	uint8_t i;
	
	if(len>28) 
		return;//超过28个字节，无效
	
	send_buf[len+3]=0;//校验位置零
	send_buf[0]=ANO_FC_HEAD;//帧头0x88
	send_buf[1]=fun;//命令帧FUN
	send_buf[2]=len;//数据长度帧LEN
	
	for(i=0;i<len;i++)
		send_buf[i+3]=data[i];
	for(i=0;i<len+3;i++)
		send_buf[len+3] += send_buf[i];//计算数据校验位SUM
	
	USART_SendBuff(BLUETOOTH_USART,(const uint8_t*)send_buf,len+4);
  
	//USARTx_DMA_tx(send_buf);
}	



/**
  * @brief  ano report imu data
  * @param  ...
  *
  * @retval none
	* @date   2022-03-22
  */
void ano_report_imu(Axis6_t axis6_upload, Euler3_t euler3_upload, float* mag)
{
	uint8_t buf[28]={0};
	
	int16_t ax    = (int16_t)(axis6_upload.ax*100.0f);
	int16_t ay    = (int16_t)(axis6_upload.ay*100.0f);
	int16_t az    = (int16_t)(axis6_upload.az*100.0f);
	
	int16_t gx    = (int16_t)(axis6_upload.gx*100.0f);
	int16_t gy    = (int16_t)(axis6_upload.gy*100.0f);
	int16_t gz    = (int16_t)(axis6_upload.gz*100.0f);
	
	int16_t mx    = (int16_t)(mag[0]*100.0f);
	int16_t my    = (int16_t)(mag[1]*100.0f);
	int16_t mz    = (int16_t)(mag[2]*100.0f);

	
	int16_t roll  = (int16_t)(euler3_upload.roll*100.0f);
	int16_t pitch = (int16_t)(euler3_upload.pitch*100.0f);
	int16_t yaw   = (int16_t)(euler3_upload.yaw*10.0f);

	buf[0]=(ax>>8)&0xFF;
	buf[1]=ax&0xFF;
	buf[2]=(ay>>8)&0xFF;
	buf[3]=ay&0xFF;
	buf[4]=(az>>8)&0xFF;
	buf[5]=az&0xFF;
	
	buf[6]=(gx>>8)&0xFF;
	buf[7]=gx&0xFF;
	buf[8]=(gy>>8)&0xFF;
	buf[9]=gy&0xFF;
	buf[10]=(gz>>8)&0xFF;
	buf[11]=gz&0xFF;
	
	buf[12]=(mx>>8)&0xFF;
	buf[13]=mx&0xFF;
	buf[14]=(my>>8)&0xFF;
	buf[15]=my&0xFF;
	buf[16]=(mz>>8)&0xFF;
	buf[17]=mz&0xFF;    
	
	buf[18]=(roll>>8)&0xFF;
	buf[19]=roll&0xFF;
	buf[20]=(pitch>>8)&0xFF;
	buf[21]=pitch&0xFF;
	buf[22]=(yaw>>8)&0xFF;
	buf[23]=yaw&0xFF;
	
	//24-27为数据格式中的0x00
	
	ano_report(ANO_REPORT_IMU,buf,ANO_LEN);
}

/**
  * @brief  ano report pid data
  *         0x00 inner p
  *         0x04 inner i
  *         0x08 inner d
  *         0x0c outer p
  *         0x14 outer i
  *         0x18 outer d
  *
  * @param  none
  * @retval none
	* @date   2022-03-22
  */
void ano_report_pid(void)
{
	uint8_t addr = 0x00;
	uint8_t cnt = 1;
  uint8_t buf[28]={0};
	uint16_t temp = 0;
  float   pid_f[6] = {0.0f}; //3 outer, 3 inner
	
  buf[0] = ANO_REPOAT_PARAM_PID;

	
	if(pdPASS==xSemaphoreTake(I2C_MuxSem_Handle, 0))
  {
		for(uint8_t i=0;i<6;i++)
		{
			pid_f[i] = AT24C02_param_get_one(addr);
			addr += 4;
		}
		
		xSemaphoreGive(I2C_MuxSem_Handle);
  }
	
	for(uint8_t i=0;i<6;i++)
	{
		temp = (uint16_t)(pid_f[i]*100.0f);
		buf[cnt++] = temp>>8;
		buf[cnt++] = (temp<<8)>>8;
	}
  
	ano_report(ANO_REPORT_PARAM,buf,ANO_LEN);
}


/**
  * @brief  ano tune pid data
  *         0x00 inner p
  *         0x04 inner i
  *         0x08 inner d
  *         0x0c outer p
  *         0x14 outer i
  *         0x18 outer d
  *
  * @param  buf: raw data buffer ptr
  * @retval none
	* @date   2022-03-22
  */
void ano_tune_pid(uint8_t* buf)
{
	uint8_t addr = 0x00;
	uint16_t temp_hi = 0, temp_lo = 0;
	uint8_t cnt = 4;  //data begin index

  float pid_f[6] = {0.0f}; //3 outer, 3 inner
	
	for(uint8_t i=0;i<6;i++)
	{
		temp_hi = buf[cnt++];
		temp_lo = buf[cnt++];

		pid_f[i] = ((float)((temp_hi<<8) | temp_lo))/100.0f;
	}
	
	if(pdPASS==xSemaphoreTake(I2C_MuxSem_Handle, 0))
  {
		for(uint8_t i=0;i<6;i++)
		{
			
			AT24C02_param_save_one(addr,pid_f[i]);
			addr += 4;
			vTaskDelay(5);
		}
		
		xSemaphoreGive(I2C_MuxSem_Handle);
  }
}

/**
  * @brief  lock or unlock the flight
  * @param  none
  * @retval none
	* @date   2022-04-02
  */
void ano_lock_unlock(void)
{
	static FlagStatus lock_flag = RESET;
	
	lock_flag = (FlagStatus)~lock_flag;
	
	
	if(lock_flag==RESET)
	{
		vTaskSuspend(Control_Task_Handle);
    motor_stop();
		LED_RED_OFF();
	}
	
	else
	{
		vTaskResume(Control_Task_Handle);
		LED_RED_ON();
  }
}

/**
  * @brief  ano data process
  * @param  rc_raw_buf: raw buffer ptr
  *
  * @retval RX_FALSE = 0,
  *         PROCESS_OK,
  *         PROCESS_FAILED
	*
  * @date   2022-03-22
  */
ANO_Status ano_data_process(uint8_t *rc_raw_buf)
{  
	uint8_t sum = 0;
	int16_t rc_buf[4] = {0};

	
	for (uint8_t i = 0; i < 31; i++)
	  sum += rc_raw_buf[i];


  if (sum != rc_raw_buf[31])
	{
		DBG("data check error!!!\r\n");
		
		return ANO_RX_FALSE;
	}
	
	
	//remote control
	if (rc_raw_buf[0] == ANO_UP_HEAD && rc_raw_buf[1] == ANO_UP_RC && rc_raw_buf[2] == ANO_LEN)
	{
		int16_t throttle = rc_raw_buf[3]<<8 | rc_raw_buf[4];
		int16_t  yaw      = rc_raw_buf[5]<<8 | rc_raw_buf[6]; 
		int16_t  roll     = rc_raw_buf[7]<<8 | rc_raw_buf[8]; 
		int16_t  pitch    = rc_raw_buf[9]<<8 | rc_raw_buf[10]; 
		
    		
		throttle -= 1000;  //ano[1000~2000]->FC[0~1000]
		
		roll  = (int16_t)((float)roll * 0.18f - 270.0f);//ano[1000~2000]->FC[-90~90]
		pitch = (int16_t)((float)pitch * 0.18f - 270.0f);
		yaw   = (int16_t)((float)yaw * 0.18f - 270.0f);

		
	  rc_buf[0] = throttle;
	  rc_buf[1] = pitch;   
	  rc_buf[2] = roll;    
	  rc_buf[3] = yaw;  
                        
		xQueueSend(RC_Queue,rc_buf,0);
	  					
		
		return ANO_PROCESS_OK;
	}
	
	
	//
	if (rc_raw_buf[0] == ANO_UP_HEAD || rc_raw_buf[1] == ANO_UP_PARAM || rc_raw_buf[2] == ANO_LEN)
	{
		switch(rc_raw_buf[3])
		{
			case ANO_INQUIRE_PARAM_PID:   ano_report_pid();
                                    break;
			
			case ANO_TUNE_PID:            ano_tune_pid(rc_raw_buf);
			                              break;
			
			case ANO_LOCK_UNLOCK:         ano_lock_unlock();
				                            LED_RED_TOGGLE();
			                              break;
			
			default:                      break;  			
		}
	}

	return ANO_PROCESS_OK;
}





