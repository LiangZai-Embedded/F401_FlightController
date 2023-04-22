
#ifndef _UPLOAD_TASK_H
#define _UPLOAD_TASK_H


#include "headfiles.h"

#define PARAM_UPLOAD_HEAD       0xFA
#define VBAT_UPLOAD_HEAD        0xFB
#define VBLUETOOTH_UPLOAD_HEAD  0xFC
#define IMU1_UP_HEAD            0xFD

void Upload_TaskCreate(void);

extern QueueHandle_t Param_Addr_Queue;


#endif
