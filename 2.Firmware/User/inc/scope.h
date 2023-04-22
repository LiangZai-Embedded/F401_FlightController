#ifndef _SCOPE_H
#define _SCOPE_H

#include "headfiles.h"

#define ANO_LEN          0x1C

//PC->FC
//[0]
#define ANO_UP_HEAD 0x8A

//[1]
#define ANO_UP_RC    0x8A
#define ANO_UP_PARAM 0x8B

//[3]
#define ANO_LOCK_UNLOCK          0xAA
#define ANO_TUNE_PID             0xAE 
#define ANO_INQUIRE_PARAM_OFFSET 0xAC 
#define ANO_INQUIRE_PARAM_PID    0xAD

//FC->PC

//[0]
#define ANO_FC_HEAD 0x88

//[1]
#define ANO_REPORT_PARAM   0xAC
#define ANO_REPORT_IMU     0xAF 


//[3]
#define ANO_REPOAT_PARAM_OFFSET 0xAC 
#define ANO_REPOAT_PARAM_PID    0xAD



typedef enum {

ANO_RX_FALSE = 0,
ANO_PROCESS_OK,
ANO_PROCESS_FAILED

}ANO_Status;

void ano_test_send(float f0,float f1,float f2,float f3);
void ano_report_imu(Axis6_t axis6_upload, Euler3_t euler3_upload, float* mag);
ANO_Status ano_data_process(uint8_t *rc_raw_buf);


#endif
