

#include "hmc5883l.h"


void hmc5883l_init(void)
{
    unsigned char buf[3]={0x78,0x20,0x00};
		
		Sensors_I2C_WriteRegister(HMC5883L_ADDR_7,HMC5883l_CONFIG_A,1,&buf[0]);//average 8,75Hz
		Sensors_I2C_WriteRegister(HMC5883L_ADDR_7,HMC5883l_CONFIG_B,1,&buf[1]);
		Sensors_I2C_WriteRegister(HMC5883L_ADDR_7,HMC5883l_MODECONFIG,1,&buf[2]);

		
}


/**
  * @brief  read 3-DOF magdata from qmc5883
  * @param  mag data buffer
  * @retval none  
	* @date   2020-09-13
  */
void hmc5883l_read_mxyz(float* mag)
{
	uint8_t buf[6];
  
  Sensors_I2C_ReadRegister(HMC5883L_ADDR_7,0x03,6,buf);  
  
	mag[0] = (float)((int16_t)(((uint16_t)buf[0]<<8 | buf[1])))/1090.0f;
	mag[1] = (float)((int16_t)(((uint16_t)buf[4]<<8 | buf[5])))/1090.0f;
	mag[2] = (float)((int16_t)(((uint16_t)buf[2]<<8 | buf[3])))/1090.0f;
}




float hmc5883l_read(void)
{	
  float mag_raw[3] = {0.0f};
	float mag_offset[3]={0.0f};
  
  static float mag_max[3] = {-2.0f,-2.0f,-2.0f};
	static float mag_min[3] = {2.0f,2.0f,2.0f};
	

  hmc5883l_read_mxyz(mag_raw);


 	for(uint8_t i=0;i<3;i++)
	{
		if(mag_raw[i]>mag_max[i])  
			mag_max[i] = mag_raw[i];
		
		if(mag_raw[i]<mag_min[i])  
			mag_min[i] = mag_raw[i];
	}
	
  mag_offset[0] = (mag_max[0]+mag_min[0])/2.0f;
  mag_offset[1] = (mag_max[1]+mag_min[1])/2.0f;
	mag_offset[2] = (mag_max[2]+mag_min[2])/2.0f;
  
	mag_raw[0] -= mag_offset[0];
  mag_raw[1] -= mag_offset[1];
  mag_raw[2] -= mag_offset[2];
	

	return RADTODEG(FastAtan2(mag_raw[1],mag_raw[0]));

}



void hmc5883l_get_data(Euler3_t* euler3) 
{    
  float mag[3] = {0.0f};
	float mag_offset[3]={0.0f};
  
  static float mag_max[3] = {-2.0f,-2.0f,-2.0f};
	static float mag_min[3] = {2.0f,2.0f,2.0f};
   
   float mx,my,mz,hy,hx;
   
   float roll = euler3->pitch;
   float pitch = -(euler3->roll);
   
   float cos_pitch,sin_pitch,cos_roll,sin_roll;
   



	 hmc5883l_read_mxyz(mag);
	 	for(uint8_t i=0;i<3;i++)
	{
		if(mag[i]>mag_max[i])  
			mag_max[i] = mag[i];
		
		if(mag[i]<mag_min[i])  
			mag_min[i] = mag[i];
	}
	
  mag_offset[0] = (mag_max[0]+mag_min[0])/2.0f;
  mag_offset[1] = (mag_max[1]+mag_min[1])/2.0f;
	mag_offset[2] = (mag_max[2]+mag_min[2])/2.0f;
  
	mag[0] -= mag_offset[0];
  mag[1] -= mag_offset[1];
  mag[2] -= mag_offset[2];
	 
   roll = DEGTORAD(roll);
   pitch = DEGTORAD(pitch);
   
	 FastSinCos(pitch,&sin_pitch,&cos_pitch);
   FastSinCos(roll,&sin_roll,&cos_roll);



	 mx = mag[0];
   my = mag[1];
   mz = mag[2];
   
	 hx = mx * cos_pitch + my * sin_roll * sin_pitch + mz * cos_roll * sin_pitch;
   hy = my * cos_roll - mz * sin_roll;
   
   euler3->yaw = RADTODEG(FastAtan2(-hy,hx)); 
}
 













//#include "QMC5883L.h"

//float mag_offset[3] = { (MAGX_MAX + MAGX_MIN) / 2, (MAGY_MAX + MAGY_MIN) / 2, (MAGZ_MAX + MAGZ_MIN) / 2 };
//double magGain[3];


///**
//  * @brief  write reg to qmc5883
//  * @param  reg: reg addr
//	          data: data to be sent
//  * @retval none  
//	* @date   2020-09-13
//  */
//static void qmc5883_write_reg(uint8_t reg,uint8_t data)
//{
//  int res = Sensors_I2C_WriteRegister(QMC_ADDR_7,reg,1,&data);
//	res=res;
//}


///**
//  * @brief  reg 3-DOF magdata from qmc5883
//  * @param  mag_buff[x,y,z]
//  * @retval none  
//	* @date   2020-09-13
//  */
//static void qmc5883_read_mxyz(int16_t* mag_buff)
//{
//  uint8_t buf[6];
//	
//	Sensors_I2C_ReadRegister(QMC_ADDR_7,0x00,6,buf);
//	
//	mag_buff[0] = ((int16_t)buf[1]<<8)|buf[0];
//	mag_buff[1] = ((int16_t)buf[3]<<8)|buf[2];
//  mag_buff[2] = ((int16_t)buf[5]<<8)|buf[4];
//}


///**
//  * @brief  qmc5883 init
//  * @param  none
//  * @retval none  
//	* @date   2020-09-13
//  */
//void qmc5883_init(void)
//{
//  qmc5883_write_reg(0X0B,0x01);
//  qmc5883_write_reg(CFG,0x1D);

//  //qmc5883_get_mag_offset();
//}


//// /**
////   * @brief  get compass data
////   * @param  none
////   * @retval mag_yaw  
//// 	* @date   2020-09-16
////   */
//// float qmc5883_get_raw_data(void)
//// {
//// 	int16_t mag[3];
//// 	float mag_yaw;
//	
//// 	qmc5883_read_mxyz(mag);
//	
//// 	mag_yaw = FastAtan2((float)mag[0],(float)mag[1])*(180.0f / PI)+180.0f;
//	
//// 	return mag_yaw;
//// }


//void qmc5883_get_data(Euler3_t* euler3) 
//{    
//   int16_t mag[3];
//   
//   float hy,hx;
//   
//   float pitch = euler3->pitch;
//   float roll = euler3->roll;
//   
//   float cos_pitch,sin_pitch,cos_roll,sin_roll;
//   
//   arm_sin_cos_f32(pitch,&sin_pitch,&cos_pitch);
//   arm_sin_cos_f32(roll,&sin_roll,&cos_roll);


//	qmc5883_read_mxyz(mag);
//	 
//    roll  *= DEG_TO_RAD;
//    pitch *= DEG_TO_RAD;  

//     
////   mag[0] *= magGain[0];
////   mag[1] *= magGain[1];
////   mag[2] *= magGain[2];
//     
//   mag[0] -= mag_offset[0];
//   mag[1] -= mag_offset[1];
//   mag[2] -= mag_offset[2];

//	
//   hy = mag[1] * cos_roll + mag[0] * sin_roll * sin_pitch - mag[2] * cos_pitch * sin_roll;

//   hx = mag[0] * cos_pitch + mag[2] * sin_pitch;
//   
//   euler3->yaw = FastAtan2(hy, hx) * RAD_TO_DEG;
//}



//void qmc5883_get_mag_offset(void)
//{
//	int16_t mag_min[3] = {32767,32767,32767};
//	int16_t mag_max[3] = {-32768,-32768,-32768};
//	int16_t mag_raw[3];
//	
//	while(1)
//	{
//		qmc5883_read_mxyz(mag_raw);
//		
//		for(int i=0;i<3;i++)
//		{
//			if(mag_raw[i]>mag_max[i])
//				mag_max[i] = mag_raw[i];
//			if(mag_raw[i]<mag_min[i])
//				mag_min[i] = mag_raw[i];
//			
//	  }
//		
//		cnt_timer_delay_ms(20);
//		
//		printf("max ");
//		for(int i=0;i<3;i++)
//		  printf("%d ",mag_max[i]);
//		printf("\r\n");
//		
//	    printf("min ");
//        for(int i=0;i<3;i++)
//		  printf("%d ",mag_min[i]);
//		printf("\r\n");
//		
//	
//	}
//}




