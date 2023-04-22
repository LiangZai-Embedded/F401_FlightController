#ifndef _KALMAN_H
#define _KALMAN_H

#include "headfiles.h"


#define Q_ANGLE_ADDR  0x48
#define Q_GYRO_ADDR   0x4C
#define R_ANGLE_ADDR  0x50
#define M_ANGLE_ADDR  0x54


typedef struct
{
	float  P[2][2];
  float  angle;
  float  Q_bias;   //Q_bias为陀螺仪漂移
	float  Angle_err; 
}Kalman_t;
extern Kalman_t kalman_pitch,kalman_roll,kalman_yaw;



void kalman_filter_init(Kalman_t* klm);
void kalman_filter_apply(Axis6_t* axis6,Euler3_t* euler3_in,Euler3_t* euler3_out);

#endif
