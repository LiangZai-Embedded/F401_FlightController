

#ifndef _BATT_TASK_H
#define _BATT_TASK_H

#include "headfiles.h"

extern SemaphoreHandle_t LED_Sem_Handle;
extern QueueHandle_t BATT_Queue;
void Batt_TaskCreate(void);

#endif


