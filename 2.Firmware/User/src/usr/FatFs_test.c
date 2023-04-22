/**
  ******************************************************************************
  *                              头文件
  ******************************************************************************
  */
#include "FatFs_test.h"

/*
*************************************************************************
*                               变量
*************************************************************************
*/
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */



/**
  ******************************************************************
  * @brief   文件系统初始化
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */ 
void FileSystem_Init(void)
{
  res_sd = f_mount(&fs,"0:",1);
  
  if(res_sd == FR_NO_FILESYSTEM)
	{
		res_sd=f_mkfs("0:",0,0);							
		 
		if(res_sd == FR_OK)
		{
			res_sd = f_mount(NULL,"0:",1);			
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			while(1);
		}
	}
  else if(res_sd!=FR_OK)
  {
		while(1);
  }
  
  else
	{
		res_sd = f_open(&fnew, "RAW.XLS",FA_CREATE_ALWAYS | FA_WRITE );
		if(res_sd!=FR_OK)
			while(1);
	}
}





