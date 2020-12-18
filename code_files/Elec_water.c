
#include "utiles.h"
#include "Timer_Interrupt.h"
#include "periph_Inti.h"
#include"SevenSeg.h"
#include "Set_Temp.h"
#include "Temp_sensor.h"
#include"eeprom.h"


 
 Heater_Mode heater_state = HEATER_OFF;     //heater state

 volatile u8 tempreture_set =60;

 
 u16 Water_Temp;            //water temperature
 u16 Water_Temp_Avg;        //water avrage temperature


    //TIMER0_INTERRUPT

volatile u16 cnt_5s_Flag;       // count the 5 s
volatile Temp_Setting_Mode Temp_Setting = Temp_Setting_OFF;        // flag declare is temp setting mode ON or  OFF
volatile u8 Display_delay_Flag=0;       // flash the 7 seg in the setting mode

volatile u8 TempSet_Flag=1;          //turn on the heater or cooler element
volatile u8 Temp_Read_Flag=1;        // flag to read water temp every 100ms
volatile u8 cnt_Temp_Read_Flag=0;   // counter flag to 100ms
u8 flag_heat_element_on;       // flash heater led

u8 flag_Heater_OFF=1;          // th first time on the heater state off


u16 HEAT_Read_Flag =0;       // count for the 1s for the led



  // Timer Interrupt routine
void interrupt() {
 TMR0 = 155;                          // every time enter the interrupt restart the 1 ms

 if(Temp_Setting==Temp_Setting_ON){     // if the setting mode is on start count for the 5 s
 cnt_5s_Flag++;

                }
                
 if( TempSet_Flag==1){      // if the heater start to work ..counter flag start counting
 cnt_Temp_Read_Flag++;
 
  }
  if( flag_heat_element_on==1)  // if heater element is on flag start to flash heater led
 {
HEAT_Read_Flag++;
}


 if((cnt_5s_Flag%1000)==0){     // ever 1000 ms = 1s the heater led is toggled or flash
  Display_delay_Flag^=1;
 }
 
if(cnt_5s_Flag==5000){        // check if the 5000ms=5 seconds are finished  turn off the setting mode and set flag to start heating or cooling
        Temp_Setting=Temp_Setting_OFF;
        TempSet_Flag=1;                       // flag mean that temperature is set
    }
    
if (cnt_Temp_Read_Flag==100){           // counter flag to read the water temperature every 100ms //every time after read in the main the flag cleared
      Temp_Read_Flag=1;                  //flag for read in the main
      ADC_Start_Conv();                 // start conversion the ADC to reduce the delay when reading the value of ADC register
    }

if ((HEAT_Read_Flag%1000)==0) {      //counter flag to flash the heater led every 1000ms=1s
 TOGGEL_Bit(PORTB,HEATER_LED );
 
 if(HEAT_Read_Flag==60000){        //clear the counter flag to avoid the overflow as its size is 16 bit
 HEAT_Read_Flag=0;}
}
    INTCON = 0x20;     // Bit T0IE is set, bit T0IF is cleared
}
 //////////////////////////////////////////

void main() {
I2C1_Init(100000);

   tempreture_set =  E2PROM_Read(Temp_Address);    // load data from EEPROM address 2
   
//TIMER0_prescaller
Timer0_Init();

DIO_Inti();
ADC_Init() ;


 while(1)
              {


//********* Heater is OFF and polling for the ON/OFF button


/////////////////////////////////////     HEATER   OFF   STATE   ////////////////////////////////////////////
    
    
    while( heater_state == HEATER_OFF)                {

        if( !Read_Bit(PORTB,ON_OFF_BUTTON )){               // check if the ON/OFF button is pressed
             heater_state = HEATER_ON;                      //flag to change the heater state
             while( !Read_Bit(PORTB,ON_OFF_BUTTON ));
                                                     }

        if(flag_Heater_OFF==1){          // turn off all the seven segment ,heater,cooler ,led
                                          // the flag " flag_Heater_OFF " to just turn off all one time >> for the first time on the while loop only
           Display_OFF();                 // as we didn't have to turn them many times
           Cooler_Heater_OFF ();
           flag_Heater_OFF=0;     }
           
                                                         }
                                                         
                                                         
                                                         
   /////////////////////////////////////////////    HEATER    ON   STATE   //////////////////////////////////////

 while(  heater_state == HEATER_ON ){

///// inti value water temp

 if (flag_Heater_OFF==0){    // when it turns from the OFF state to the ON state i take a shot from the water temperature

   ADC_Start_Conv();
  
   Water_Temp =Temp_calc();
  
   flag_Heater_OFF=1;           // set the "flag_Heater_OFF" to turn off all when go to the OFF state
                           }



//////////////////////////////////////  BUTTON ON/OFF   /////////////////////////////////////////////////////////////////

                                                   // at first ,check on the on/off button
if( !Read_Bit(PORTB,ON_OFF_BUTTON ))         {

     heater_state = HEATER_OFF;
     
     while( !Read_Bit(PORTB,ON_OFF_BUTTON ));

                                              }
                                    
                                    

            // else , i check for the up , down button
    else {  

    if ( ((!Read_Bit(PORTB,UP_Button)) || (!Read_Bit(PORTB,DOWN_Button))) && (Temp_Setting==Temp_Setting_OFF) )   // press up , down for the first time , it just open the setting mode
    {
         ///////////////// START COUNT THE 5 SECOUNDS FOR THE SETTING MODE///////////////////
         TMR0 = 155;
         cnt_5s_Flag=0;
          Enable_Timer0_Interrupt();

          
         Temp_Setting=Temp_Setting_ON;            // open the setting mode and enable the counter flag on the interrupt to count
         
         TempSet_Flag=0;                      // clear all the other flags
         cnt_Temp_Read_Flag=0;
         flag_heat_element_on=0;
         
         Cooler_Heater_OFF ();                 // turn off cooler , heater  and the led
         
              while((!Read_Bit(PORTB,UP_Button)) || (!Read_Bit(PORTB,DOWN_Button)));
      }


          
///////////////////////////////////////////////////// SETTING TEMPETRURE MODE ///////////////////////////////////

         while((Temp_Setting==Temp_Setting_ON)&& Read_Bit(PORTB,ON_OFF_BUTTON )){        // if flag is set and the on/off button don't be pressed

           if(!Read_Bit(PORTB,UP_Button)) {      //////////////  up button pressed    /////
               TMR0 = 155;                       // if the up button ,start to count the 5 s from the first
              cnt_5s_Flag=0;                     // counter flag count the 5000ms in the interrupt

           Temp_Inc();                      // temp +=5;
           
           while(!Read_Bit(PORTB,UP_Button)); 

           }

            else if((! Read_Bit(PORTB,DOWN_Button)) ){      ////////DOWN  button pressed    ///
                       TMR0 = 155;                         // if the down button ,start to count the 5 s from the first
                      cnt_5s_Flag=0;

                        Temp_Dec();                       //  temp -=5;
                        
                         while(!Read_Bit(PORTB,DOWN_Button));
                   }


                 if(Display_delay_Flag==1)     //the flag toggled every 1000ms so it flash the seven segment

                { SEGMENT_Display_2Digit(tempreture_set) ; }
                
                else {Display_OFF();}

                                                                                 }

       //the interrupt set this flag " TempSet_flag " after 5s
       //while any button pressed , turn on the heater and cooler
   while((TempSet_Flag==1)&& Read_Bit(PORTB,ON_OFF_BUTTON )&& Read_Bit(PORTB,UP_Button) && Read_Bit(PORTB,DOWN_Button)) {
    ////////////clear all other flags
       Temp_Setting=Temp_Setting_OFF;
       cnt_5s_Flag=0;
       Enable_Timer0_Interrupt();    //enable interrupt
    ////////////////
    
      if (Temp_Read_Flag==1){        // if flag " Temp_Read_Flag" set by the interrupt every 100ms read the water temp
         TMR0 = 155;                 //restart count the 100ms
        cnt_Temp_Read_Flag=0;
        
        Temp_Read_Flag=0;            // clear the flag to just waiting the by the interrupt
        
        Water_Temp = Temp_calc();    // read the ADC register
        
        Temp_push( Water_Temp);      // push the value to an array to calculate the avrage
        }
        
        Water_Temp_Avg=(u8)Calculate_Temp_Avg();               //calculate the avg of 10 reads //the array start with 0 initial values

      if( ((Water_Temp_Avg)+5) < tempreture_set){
      flag_heat_element_on=1;                      // set the flag  to flash the led
           HeaterON();                              //open heater
          }

      if( ((Water_Temp_Avg)-5) >  tempreture_set ) {
       flag_heat_element_on=0;
           Cooler_ON();
           SET_BIT(PORTB,HEATER_LED);                     //open the led
            }



                SEGMENT_Display_2Digit(Water_Temp/10) ;

   }

                                                                                                                              }


                                                                                                                                 }

                                                                                                                                 }
                                                                           



                                                                                                                                 }