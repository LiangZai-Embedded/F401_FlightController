


#include "ICM20602.h"


static void icm_spi_w_reg_byte(uint8_t reg,uint8_t val)
{
	 CS_LOW();

	 SPI_WR_Byte(reg|ICM20602_SPI_W);
	
	 SPI_WR_Byte(val);
	
	 CS_HIGH();
}
	

	

//-------------------------------------------------------------------------------------------------------------------

void icm_spi_r_reg_byte(uint8_t reg, uint8_t* data)
{
	CS_LOW();
	
	SPI_WR_Byte(reg|ICM20602_SPI_R);
	
	*data = SPI_WR_Byte(DUMMY);
	
	CS_HIGH();
}
  



void icm_spi_r_reg_bytes(uint8_t* addr, uint8_t num)
{
	uint8_t i;
	
	CS_LOW();
	
	SPI_WR_Byte(*addr|ICM20602_SPI_R);
	
	addr++;
	
	for(i=0;i<num-1;i++)  
	  *addr++ = SPI_WR_Byte(DUMMY);
	
  CS_HIGH();
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602自检函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void icm20602_self3_check(void)
{
    uint8_t dat=0;
    while(0x12 != dat)   //读取ICM20602 ID
    {
        icm_spi_r_reg_byte(ICM20602_WHO_AM_I,&dat);
        cnt_timer_delay_ms(10);
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }
}
     
//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化ICM20602
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void icm20602_init(void)
{
  uint8_t val = 0x0;

	  SPI_Config();

	icm20602_self3_check();//检测
  
  icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,0x80);//复位设备
  cnt_timer_delay_ms(2);
  do
  {//等待复位成功
      icm_spi_r_reg_byte(ICM20602_PWR_MGMT_1,&val);
  }while(0x41 != val);
  
  icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,     0x01);//时钟设置
  icm_spi_w_reg_byte(ICM20602_PWR_MGMT_2,     0x00);//开启陀螺仪和加速度计
  icm_spi_w_reg_byte(ICM20602_CONFIG,         0x02);//gyro: 3dB_BW=92Hz   rate=1KHZ
  icm_spi_w_reg_byte(ICM20602_SMPLRT_DIV,     0x01);//1KHZ/(1+1)=500Hz
  icm_spi_w_reg_byte(ICM20602_GYRO_CONFIG,    0x18);//±2000 dps
  icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG,   0x10);//±8g
  icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x02);//acc:  3dB_BW=99.8HZ   
}



static void icm20602_get_acc(Axis6_t* axis6)
{
	struct
  {
     uint8_t reg;
     uint8_t dat[6];
  }buf;
	
	buf.reg = ICM20602_ACCEL_XOUT_H | ICM20602_SPI_R;
    
  icm_spi_r_reg_bytes(&buf.reg, 7);
  
	axis6->ax = (float)((int16_t)(((uint16_t)buf.dat[0]<<8 | buf.dat[1])))*0.002441f;
  axis6->ay = (float)((int16_t)(((uint16_t)buf.dat[2]<<8 | buf.dat[3])))*0.002441f;
  axis6->az = (float)((int16_t)(((uint16_t)buf.dat[4]<<8 | buf.dat[5])))*0.002441f;
}


static void icm20602_get_gyro(Axis6_t* axis6)
{
	struct
  {
     uint8_t reg;
     uint8_t dat[6];
  }buf;
	
	buf.reg = ICM20602_GYRO_XOUT_H | ICM20602_SPI_R;

	icm_spi_r_reg_bytes(&buf.reg, 7);
  
	axis6->gx = (float)((int16_t)(((uint16_t)buf.dat[0]<<8 | buf.dat[1])))*0.061037f;// 2000dps/32767
  axis6->gy = (float)((int16_t)(((uint16_t)buf.dat[2]<<8 | buf.dat[3])))*0.061037f;
  axis6->gz = (float)((int16_t)(((uint16_t)buf.dat[4]<<8 | buf.dat[5])))*0.061037f;
}



void icm20602_get_data(Axis6_t* axis6)
{	
  float gx_bias = -3.102641f,gy_bias = 13.074298f,gz_bias = -12.717360f;
	icm20602_get_acc(axis6);
	icm20602_get_gyro(axis6);

	axis6->gx = axis6->gx - gx_bias;
  axis6->gy = axis6->gy - gy_bias;
	axis6->gz = axis6->gz - gz_bias;
}


/**
  * @brief  icm20602 print data to console
  * @param  none
  * @retval none  
	* @date   2022-01-14
  */
void icm20602_print_data(void)
{
  Axis6_t axis6;
	float gx_sum = 0.0f,gy_sum = 0.0f,gz_sum = 0.0f;
	
  for(uint32_t cnt = 0;cnt<500;cnt++)
  {
	  icm20602_get_gyro(&axis6);
		
		gx_sum += axis6.gx/500.0f;
		gy_sum += axis6.gy/500.0f;
		gz_sum += axis6.gz/500.0f;
    
		printf("%f\t%f\t%f\t\r\n",axis6.gx,axis6.gy,axis6.gz);
  }
	
	
	
	printf("bias:%f\t%f\t%f\t\r\n",gx_sum,gy_sum,gz_sum);

	
}
	
















