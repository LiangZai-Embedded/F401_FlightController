#ifndef _IMU_TYPE_H
#define _IMU_TYPE_H


#include "headfiles.h"

typedef struct
{
  float pitch;
  float roll;
  float yaw;
}Euler3_t;


typedef struct
{
  float ax;
  float ay;
  float az;
  
  float gx;
  float gy;
  float gz;
}Axis6_t;

#endif
