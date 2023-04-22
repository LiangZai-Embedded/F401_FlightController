
#ifndef _REMOTE_H
#define _REMOTE_H


#include "headfiles.h"


#define STORAGE_CMD      0x00
#define RC_CMD           0x01
#define INQUIRY_CMD      0x02
#define URGENT_STOP_CMD  0x03
#define UNLOCK_CMD       0x04
#define RAW_UPLOAD_CMD   0x05



typedef enum {

RX_FALSE = 0,
PROCESS_OK,
PROCESS_FAILED

}RC_Status;

RC_Status rc_data_process(uint8_t *raw_buf);


#endif
