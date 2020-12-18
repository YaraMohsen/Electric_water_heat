#line 1 "C:/Users/yaram/Documents/elec_water/E2PROM.c"
#line 1 "c:/users/yaram/documents/elec_water/eeprom.h"







unsigned char E2PROM_Read(unsigned int addr);
void E2PROM_Write(unsigned int addr, unsigned char val);
#line 4 "C:/Users/yaram/Documents/elec_water/E2PROM.c"
unsigned char E2PROM_Read(unsigned int addr)
{
 unsigned char ret;
 unsigned char ah;
 unsigned char al;

 ah=(addr&0x0100)>>8;
 al=addr&0x00FF;

 I2C1_Start();
 if(ah)
 {
 I2C1_Wr(0xA2);
 }
 else
 {
 I2C1_Wr(0xA0);
 }
 I2C1_Wr(al);

 I2C1_Start();
 if(ah)
 {
 I2C1_Wr(0xA3);
 }
 else
 {
 I2C1_Wr(0xA1);
 }
 ret=I2C1_Rd(1);
 I2C1_Stop();

 return ret;
}


void E2PROM_Write(unsigned int addr, unsigned char val)
{
 unsigned int tmp;
 unsigned char ah;
 unsigned char al;
 unsigned char nt;

 tmp=val;
 ah=(addr&0x0100)>>8;
 al=addr&0x00FF;
 nt=0;

 do
 {
 I2C1_Start();
 if(ah)
 {
 I2C1_Wr(0xA2);
 }
 else
 {
 I2C1_Wr(0xA0);
 }
 I2C1_Wr(al);
 I2C1_Wr(tmp);
 I2C1_Stop();

 nt++;
 }
 while((tmp != E2PROM_Read(addr))&&(nt < 10));
}
