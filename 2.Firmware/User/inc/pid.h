#ifndef _PID_H
#define _PID_H


#include "headfiles.h"

#define PITCH_OUT_P_ADDR  0x00
#define PITCH_OUT_I_ADDR  0x04
#define PITCH_OUT_D_ADDR  0x08
#define ROLL_OUT_P_ADDR   0x0C
#define ROLL_OUT_I_ADDR   0x10
#define ROLL_OUT_D_ADDR   0x14
#define YAW_OUT_P_ADDR    0x18
#define YAW_OUT_I_ADDR    0x1C
#define YAW_OUT_D_ADDR    0x20

#define PITCH_IN_P_ADDR   0x24
#define PITCH_IN_I_ADDR   0x28
#define PITCH_IN_D_ADDR   0x2C
#define ROLL_IN_P_ADDR    0x30
#define ROLL_IN_I_ADDR    0x34
#define ROLL_IN_D_ADDR    0x38
#define YAW_IN_P_ADDR     0x3C
#define YAW_IN_I_ADDR     0x40
#define YAW_IN_D_ADDR     0x44






typedef struct
{
	float p;
	float i;
	float d;
	float sum;
	float err_last;
	float i_limit;
}PID_t;

extern PID_t pitch_inner, roll_inner, yaw_inner;	
extern PID_t pitch_outer, roll_outer, yaw_outer;	


void pid_init(void);
float pid_update(PID_t* pid,float cur,int8_t target,int16_t throttle);
	
	
	
	

#endif
