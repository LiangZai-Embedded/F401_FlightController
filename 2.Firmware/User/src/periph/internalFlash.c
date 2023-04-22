
  
#include "internalFlash.h"   




#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_4 /* 要擦除内部FLASH的起始地址 */  
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_5 /* 要擦除内部FLASH的结束地址 */
 

 
/**
  * @brief  flash initialization
  * @param  none
  * @retval none
  *         none 
	* @date   2021-03-01
  */
 void flash_init(void)
 {
  FLASH_Unlock();

  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
 }

/**
  * @brief  erase flash sector
  * @param  none
  * @retval none
	* @date   2021-02-28
  */
void flash_erase(void)
{
  while(FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
  {
    vTaskDelay(10);
  }

   while(FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3) != FLASH_COMPLETE)
  {
    vTaskDelay(10);
  }
}


/**
  * @brief  save 6 axis flight data to internal flash
  * @param  dat_buf: 6 axis data buffer ptr
  * @retval FALSE: no full
  *         TRUE:  full    
	* @date   2021-02-28
  */
bool flash_storage(float* dat_buf)
{
	static uint32_t addr = FLASH_USER_START_ADDR;
  static uint16_t cnt;

  if(addr<FLASH_USER_START_ADDR || (cnt>4096))
    return TRUE;
  
	for(uint8_t i=0;i<6;i++)
	{
		while (FLASH_ProgramWord(addr, *(uint32_t*)&dat_buf[i]) != FLASH_COMPLETE);
		
		addr += 4;
	}
	
  cnt++;

  return FALSE;
}

