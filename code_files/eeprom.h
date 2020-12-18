#ifndef _H_EEPROM
#define _H_EEPROM

#define Temp_Address 2
#define Compare1_Address 2
#define Compare2_Address 3
 
unsigned char E2PROM_Read(unsigned int addr);
void E2PROM_Write(unsigned int addr, unsigned char val);

#endif