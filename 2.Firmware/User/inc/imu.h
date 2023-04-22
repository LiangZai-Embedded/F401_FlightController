#ifndef _IMU_H
#define _IMU_H

#include "headfiles.h"

void imu_update(Axis6_t axis6, Euler3_t* euler3);
void imu_get_eula(Axis6_t axis6,Euler3_t* euler3);







#endif
