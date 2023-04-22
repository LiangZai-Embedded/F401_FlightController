
#include "pid.h"


PID_t pitch_inner, roll_inner, yaw_inner;
PID_t pitch_outer, roll_outer, yaw_outer;


/**
  * @brief  pid parameters initialization
  * @param  none
  * @retval none
	* @date   2021-1-17
  */
void pid_init(void)
{
	memset(&pitch_outer,0,sizeof(PID_t));
	memset(&roll_outer,0,sizeof(PID_t));
	memset(&yaw_outer,0,sizeof(PID_t));

	memset(&pitch_inner,0,sizeof(PID_t));
	memset(&roll_inner,0,sizeof(PID_t));
	memset(&yaw_inner,0,sizeof(PID_t));
  
	pitch_outer.p = AT24C02_param_get_one(PITCH_OUT_P_ADDR);
  pitch_outer.i = AT24C02_param_get_one(PITCH_OUT_I_ADDR);
  pitch_outer.d = AT24C02_param_get_one(PITCH_OUT_D_ADDR);
  roll_outer.p  = AT24C02_param_get_one(ROLL_OUT_P_ADDR);
  roll_outer.i  = AT24C02_param_get_one(ROLL_OUT_I_ADDR);
  roll_outer.d  = AT24C02_param_get_one(ROLL_OUT_D_ADDR);
  yaw_outer.p   = AT24C02_param_get_one(YAW_OUT_P_ADDR);
  yaw_outer.i   = AT24C02_param_get_one(YAW_OUT_I_ADDR);
  yaw_outer.d   = AT24C02_param_get_one(YAW_OUT_D_ADDR);
 
  pitch_inner.p = AT24C02_param_get_one(PITCH_IN_P_ADDR);
  pitch_inner.i = AT24C02_param_get_one(PITCH_IN_I_ADDR);
  pitch_inner.d = AT24C02_param_get_one(PITCH_IN_D_ADDR);
  roll_inner.p  = AT24C02_param_get_one(ROLL_IN_P_ADDR);
  roll_inner.i  = AT24C02_param_get_one(ROLL_IN_I_ADDR);
  roll_inner.d  = AT24C02_param_get_one(ROLL_IN_D_ADDR);
  yaw_inner.p   = AT24C02_param_get_one(YAW_IN_P_ADDR);
  yaw_inner.i   = AT24C02_param_get_one(YAW_IN_I_ADDR);
  yaw_inner.d   = AT24C02_param_get_one(YAW_IN_D_ADDR);
}

/**
  * @brief  pid controller
  *
  * @param    pid: pid structure ptr
  *           cur: current angle
  *        target: target  angle
  *          gyro: gyro data/dps
  *      throttle: to calculate sum_limit
  * 
  * @retval  output
  * @date    2021-02-10
  */
float pid_update(PID_t* pid,float cur,int8_t target,int16_t throttle)
{
  float err = target - cur;
	float ret;
//  float sum_limit = (float)throttle*0.05f;
//	
	pid->sum += err;
//  
//   if(pid->sum > sum_limit)
//     pid->sum = sum_limit;
//  
//   if(pid->sum < -sum_limit)
//     pid->sum = -sum_limit;
	
	 ret = pid->p * err + 
	       pid->i * pid->sum +
	       pid->d * (err - pid->err_last);


   pid->err_last = err;
	
	return ret;
}



