

#include "at24c02.h"


	
/**
  * @brief  save a float param to eeprom
  * @param  ee_addr: eeprom storage addr
  *            data: float data to be stored
  * @retval none  
	* @date   2020-08-08
  */
void AT24C02_param_save_one(uint8_t ee_addr, float data)
{
	uint8_t* start_addr = (uint8_t*)&data;
	
	for(uint8_t i=0;i<4;i++)
	  Sensors_I2C_WriteRegister(EEPROM_ADDRESS_7,ee_addr++,1,start_addr++);
		
}	
	
	
/**
  * @brief  save a float param to eeprom
  * @param  ee_addr: eeprom storage addr
  * @retval a float data  
	* @date   2020-08-08
  */
float AT24C02_param_get_one(uint8_t ee_addr)
{
	uint8_t buf[4];
	int res;
	for(uint8_t i=0;i<4;i++)
	{
			res = Sensors_I2C_ReadRegister(EEPROM_ADDRESS_7,ee_addr++,1,buf+i);
	    res = res;
	}
	return *(float*)buf;
		
}	 



/**
  * @brief  print parameter to console
  * @param  none
  * @retval none  
	* @date   2022-01-14
  */
void AT24C02_param_print(void)
{
	uint8_t buf[4] = {0};
	
	 
	for(uint8_t ee_addr = 0x00;ee_addr < 0x30;)
	{
		printf("0x%x\t",ee_addr);
		for(uint8_t i=0;i<4;i++)
	   Sensors_I2C_ReadRegister(EEPROM_ADDRESS_7,ee_addr++,1,buf+i);
		printf("%f\r\n",*(float*)buf);
	}
		
}	

