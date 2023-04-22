#ifndef __AT24C02_H
#define	__AT24C02_H

#include "headfiles.h"

#define  EEPROM_ADDRESS_7     0x57


float AT24C02_param_get_one(uint8_t ee_addr);
void AT24C02_param_save_one(uint8_t ee_addr, float data);
void AT24C02_param_print(void);

#endif /* __I2C_EE_H */
