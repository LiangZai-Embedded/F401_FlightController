
#ifndef _HEADFILES_H
#define _HEADFILES_H

typedef enum {FALSE = 0,TRUE = 1}bool;

#include "stm32f4xx.h"
#include "ff.h"

//types def
#include "imu_type.h"


//Intrinsic Libraries include
#include "math.h"
#include "arm_math.h"
#include "string.h"
#include "stdio.h"


//FreeRTOS include
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "list.h"
#include "event_groups.h"




//connections include
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "bsp_sdio_sd.h"


//periph include
#include "at24c02.h"
#include "hmc5883l.h"
#include "icm20602.h"
#include "led.h"
#include "cnt_timer.h"
#include "pwm.h"
#include "scope.h"
#include "bluetooth.h"
#include "internalFlash.h" 
#include "adc.h" 
#include "key.h" 


//maths include
#include "pid.h"
#include "filter.h"
#include "kalman.h"
#include "fastmath.h"




//user include
#include "remote.h"
#include "init.h"
#include "app_create_task.h"
#include "rc_task.h"
#include "imu_task.h"
#include "control_task.h"
#include "imu.h"
#include "upload_task.h"
#include "FatFs_test.h"
#include "batt_task.h"
#include "common.h"



#define __DEBUG

#ifdef __DEBUG
  #define DBG(fmt,arg...)          printf("<DEBUG>"fmt"\n",##arg)

#else
  #define DBG(fmt,arg...) 
	
#endif
 

#endif    
