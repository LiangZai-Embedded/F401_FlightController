#include "kalman.h"

float Q_angle,Q_gyro,R_angle,M_angle;
Kalman_t kalman_pitch,kalman_roll,kalman_yaw;


//Q+  acc+
//R+  gyro+

//Q_angle  0.005    //陀螺仪噪声的协方差
//Q_gyro   0.003    //陀螺仪漂移噪声的协方差
//R_angle  0.95     //加速度计的协方差
//M_angle  0.0015   //磁力计的协方差

/**
  * @brief  kalman structure initialization
  * @param  klm: point to the kalman structure
  * @retval none
	* @date   2020-10-03
  */
void kalman_filter_init(Kalman_t* klm)
{ 
	klm->P[0][0] = 1.0f;
	klm->P[0][1] = 0.0f;
	klm->P[1][0] = 0.0f;
	klm->P[1][1] = 1.0f;
	 
	klm->angle = 0.0f;
	klm->Q_bias = 0.0f;
	klm->Angle_err = 0.0f;
	
	Q_angle = AT24C02_param_get_one(Q_ANGLE_ADDR);
	Q_gyro  = AT24C02_param_get_one(Q_GYRO_ADDR);
	R_angle = AT24C02_param_get_one(R_ANGLE_ADDR);
	M_angle = AT24C02_param_get_one(M_ANGLE_ADDR);
}

/**
  * @brief  kf
  * @param  klm: point to the kalman structure
            gyro: gyro data
            acc_angle: angle calculated by accelarater using atan2
            dt: excecute time
  * @retval none
	* @date   2020-10-03
  */
float kalman_filter(Kalman_t *klm, float gyro, float acc_angle ,float dt)
{ 
	char   C_0 = 1;
  float  PCt_0=0, PCt_1=0, E=0;
  float  K_0=0, K_1=0, t_0=0, t_1=0;
  
  float  Pdot[4] = {0,0,0,0};
	
  klm->angle += (gyro-klm->Q_bias)*dt;
  
  Pdot[0] = Q_angle - klm->P[0][1] - klm->P[1][0]; 
  Pdot[1] = -klm->P[1][1];
  Pdot[2] = -klm->P[1][1];
  Pdot[3] = Q_gyro;

  klm->P[0][0] += Pdot[0] * dt;   
  klm->P[0][1] += Pdot[1] * dt;   
  klm->P[1][0] += Pdot[2] * dt;
  klm->P[1][1] += Pdot[3] * dt;
  
//  P[0][0] += -(P[0][1]+P[1][0])*dt + Q_angle;
//  P[0][1] += -P[1][1]*dt;
//  P[1][0] += -P[1][1]*dt;
//  P[1][1] += Q_bias;
  
  PCt_0 = C_0 * klm->P[0][0];//矩阵乘法的中间变量
  PCt_1 = C_0 * klm->P[1][0];//C_0=1
     
  E = R_angle + C_0 * PCt_0;//分母
    
  //增益值     
  K_0 = PCt_0 / E;
  K_1 = PCt_1 / E;
  
  klm->Angle_err = acc_angle - klm->angle; //Accel是加速度计算出来的角度的测量值。

  klm->angle += K_0 * klm->Angle_err; //对状态的卡尔曼估计。

  klm->Q_bias += K_1 * klm->Angle_err;

  t_0 = PCt_0; //矩阵计算中间变量
    
	t_1 = C_0 * klm->P[0][1];

  klm->P[0][0] -= K_0 * t_0;
  klm->P[0][1] -= K_0 * t_1;
  klm->P[1][0] -= K_1 * t_0;
  klm->P[1][1] -= K_1 * t_1;
	
	return klm->angle;

}


/**
  * @brief  kf application
  * @param  axis6: 6 axis data ptr
  *         euler3_in: raw 3 eula angles ptr
  *         euler3_out: final 3 eula angles ptr
  * @retval none
	* @date   2021-03-04
  */
void kalman_filter_apply(Axis6_t* axis6,Euler3_t* euler3_in,Euler3_t* euler3_out)
{ 
  euler3_out->roll =  kalman_filter(&kalman_roll, axis6->gy,euler3_in->roll,0.002);
  euler3_out->pitch = kalman_filter(&kalman_pitch,axis6->gx,euler3_in->pitch,0.002);
}





