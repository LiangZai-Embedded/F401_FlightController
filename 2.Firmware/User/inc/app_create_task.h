

#ifndef _APP_CREATE_TASK_H
#define _APP_CREATE_TASK_H

#include "headfiles.h"

extern SemaphoreHandle_t I2C_MuxSem_Handle;
extern SemaphoreHandle_t BluetoothReceiveSem_Handle;
extern SemaphoreHandle_t USART_MuxSem_Handle;

bool OS_Start(void);

#endif


