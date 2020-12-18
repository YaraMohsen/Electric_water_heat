#line 1 "C:/Users/yaram/Documents/elec_water/DIO_Inti.c"
#line 1 "c:/users/yaram/documents/elec_water/dio_inti.h"
#line 43 "c:/users/yaram/documents/elec_water/dio_inti.h"
 typedef enum{
 Temp_Setting_OFF=0,
 Temp_Setting_ON=1,

 } Temp_Setting_Mode;

 typedef enum{
 HEATER_OFF=0,
 HEATER_ON=1,

 } Heater_Mode;




 void DIO_Inti(void);
#line 5 "C:/Users/yaram/Documents/elec_water/DIO_Inti.c"
void DIO_Inti(void){

 TRISA=0xC7;
 TRISB=0x07;
 TRISC=0x01;
 TRISD=0x00;
 TRISE=0x00;

}
