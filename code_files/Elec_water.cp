#line 1 "C:/Users/yaram/Documents/elec_water/Elec_water.c"
#line 1 "c:/users/yaram/documents/elec_water/utiles.h"









typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
#line 1 "c:/users/yaram/documents/elec_water/timer_interrupt.h"
#line 1 "c:/users/yaram/documents/elec_water/utiles.h"
#line 1 "c:/users/yaram/documents/elec_water/timer_interrupt.h"
#line 10 "c:/users/yaram/documents/elec_water/timer_interrupt.h"
extern void Timer0_Init(void);
extern void Enable_Timer0_Interrupt(void);
extern void Disable_Timer0_Interrupt(void);
#line 1 "c:/users/yaram/documents/elec_water/periph_inti.h"
#line 1 "c:/users/yaram/documents/elec_water/utiles.h"
#line 48 "c:/users/yaram/documents/elec_water/periph_inti.h"
 typedef enum{
 Temp_Setting_OFF=0,
 Temp_Setting_ON=1,

 } Temp_Setting_Mode;

 typedef enum{
 HEATER_OFF=0,
 HEATER_ON=1,

 } Heater_Mode;




 void DIO_Inti(void);
 void HeaterON(void);
 void Cooler_ON (void);
 void Cooler_Heater_OFF (void);
#line 1 "c:/users/yaram/documents/elec_water/sevenseg.h"
#line 1 "c:/users/yaram/documents/elec_water/utiles.h"
#line 11 "c:/users/yaram/documents/elec_water/sevenseg.h"
 extern u8 display7s(u8 v) ;
 extern void SEGMENT_Display_2Digit(u8 num);
 extern void Display_OFF (void);
#line 1 "c:/users/yaram/documents/elec_water/set_temp.h"
#line 1 "c:/users/yaram/documents/elec_water/utiles.h"
#line 1 "c:/users/yaram/documents/elec_water/eeprom.h"







unsigned char E2PROM_Read(unsigned int addr);
void E2PROM_Write(unsigned int addr, unsigned char val);
#line 12 "c:/users/yaram/documents/elec_water/set_temp.h"
 extern u8 tempreture_set ;

 void Temp_Inc(void);
 void Temp_Dec(void);

 void Temp_push(u16 Current_water_temp );
 u16 Calculate_Temp_Avg(void);
#line 1 "c:/users/yaram/documents/elec_water/temp_sensor.h"
#line 1 "c:/users/yaram/documents/elec_water/utiles.h"
#line 11 "c:/users/yaram/documents/elec_water/temp_sensor.h"
 void ADC_Init(void) ;
 U16 ADC_Read(void);
 void ADC_Start_Conv(void);
 u16 Temp_calc(void);
#line 1 "c:/users/yaram/documents/elec_water/eeprom.h"
#line 12 "C:/Users/yaram/Documents/elec_water/Elec_water.c"
 Heater_Mode heater_state = HEATER_OFF;

 volatile u8 tempreture_set =60;


 u16 Water_Temp;
 u16 Water_Temp_Avg;




volatile u16 cnt_5s_Flag;
volatile Temp_Setting_Mode Temp_Setting = Temp_Setting_OFF;
volatile u8 Display_delay_Flag=0;

volatile u8 TempSet_Flag=1;
volatile u8 Temp_Read_Flag=1;
volatile u8 cnt_Temp_Read_Flag=0;
u8 flag_heat_element_on;

u8 flag_Heater_OFF=1;


u16 HEAT_Read_Flag =0;




void interrupt() {
 TMR0 = 155;

 if(Temp_Setting==Temp_Setting_ON){
 cnt_5s_Flag++;

 }

 if( TempSet_Flag==1){
 cnt_Temp_Read_Flag++;

 }
 if( flag_heat_element_on==1)
 {
HEAT_Read_Flag++;
}


 if((cnt_5s_Flag%1000)==0){
 Display_delay_Flag^=1;
 }

if(cnt_5s_Flag==5000){
 Temp_Setting=Temp_Setting_OFF;
 TempSet_Flag=1;
 }

if (cnt_Temp_Read_Flag==100){
 Temp_Read_Flag=1;
 ADC_Start_Conv();
 }

if ((HEAT_Read_Flag%1000)==0) {
  (PORTB=PORTB^(1<< 4 )) ;

 if(HEAT_Read_Flag==60000){
 HEAT_Read_Flag=0;}
}
 INTCON = 0x20;
}


void main() {
I2C1_Init(100000);

 tempreture_set = E2PROM_Read( 2 );


Timer0_Init();

DIO_Inti();
ADC_Init() ;


 while(1)
 {








 while( heater_state == HEATER_OFF) {

 if( ! ((PORTB>> 2 )&1) ){
 heater_state = HEATER_ON;
 while( ! ((PORTB>> 2 )&1) );
 }

 if(flag_Heater_OFF==1){

 Display_OFF();
 Cooler_Heater_OFF ();
 flag_Heater_OFF=0; }

 }





 while( heater_state == HEATER_ON ){



 if (flag_Heater_OFF==0){

 ADC_Start_Conv();

 Water_Temp =Temp_calc();

 flag_Heater_OFF=1;
 }






if( ! ((PORTB>> 2 )&1) ) {

 heater_state = HEATER_OFF;

 while( ! ((PORTB>> 2 )&1) );

 }




 else {

 if ( ((! ((PORTB>> 1 )&1) ) || (! ((PORTB>> 0 )&1) )) && (Temp_Setting==Temp_Setting_OFF) )
 {

 TMR0 = 155;
 cnt_5s_Flag=0;
 Enable_Timer0_Interrupt();


 Temp_Setting=Temp_Setting_ON;

 TempSet_Flag=0;
 cnt_Temp_Read_Flag=0;
 flag_heat_element_on=0;

 Cooler_Heater_OFF ();

 while((! ((PORTB>> 1 )&1) ) || (! ((PORTB>> 0 )&1) ));
 }





 while((Temp_Setting==Temp_Setting_ON)&&  ((PORTB>> 2 )&1) ){

 if(! ((PORTB>> 1 )&1) ) {
 TMR0 = 155;
 cnt_5s_Flag=0;

 Temp_Inc();

 while(! ((PORTB>> 1 )&1) );

 }

 else if((!  ((PORTB>> 0 )&1) ) ){
 TMR0 = 155;
 cnt_5s_Flag=0;

 Temp_Dec();

 while(! ((PORTB>> 0 )&1) );
 }


 if(Display_delay_Flag==1)

 { SEGMENT_Display_2Digit(tempreture_set) ; }

 else {Display_OFF();}

 }



 while((TempSet_Flag==1)&&  ((PORTB>> 2 )&1) &&  ((PORTB>> 1 )&1)  &&  ((PORTB>> 0 )&1) ) {

 Temp_Setting=Temp_Setting_OFF;
 cnt_5s_Flag=0;
 Enable_Timer0_Interrupt();


 if (Temp_Read_Flag==1){
 TMR0 = 155;
 cnt_Temp_Read_Flag=0;

 Temp_Read_Flag=0;

 Water_Temp = Temp_calc();

 Temp_push( Water_Temp);
 }

 Water_Temp_Avg=(u8)Calculate_Temp_Avg();

 if( ((Water_Temp_Avg)+5) < tempreture_set){
 flag_heat_element_on=1;
 HeaterON();
 }

 if( ((Water_Temp_Avg)-5) > tempreture_set ) {
 flag_heat_element_on=0;
 Cooler_ON();
  (PORTB=PORTB|(1<< 4 )) ;
 }



 SEGMENT_Display_2Digit(Water_Temp/10) ;

 }

 }


 }

 }




 }
