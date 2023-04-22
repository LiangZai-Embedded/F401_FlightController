#ifndef _IMU_TASK_H
#define _IMU_TASK_H

#include "headfiles.h"

extern QueueHandle_t IMU_Queue;
extern QueueHandle_t LED_Queue;
extern QueueHandle_t Euler_Queue;


extern TaskHandle_t IMU_Task_Handle;
void IMU_TaskCreate(void);

#endif
