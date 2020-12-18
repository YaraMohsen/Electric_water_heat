 else {  

    if ( ((!Read_Bit(PORTB,UP_Button)) || (!Read_Bit(PORTB,DOWN_Button))) && (Temp_Setting==Temp_Setting_OFF) )
    {
         ///////////////// START COUNT THE 5 SECOUNDS FOR THE SETTING MODE///////////////////
         TMR0 = 155;
         cnt_5s_Flag=0;
          Enable_Timer0_Interrupt();
          
          
         Temp_Setting=Temp_Setting_ON;
         TempSet_Flag=0;
         cnt_Temp_Read_Flag=0;
         flag_heat_element_on=0;
         Cooler_Heater_OFF ();
              while((!Read_Bit(PORTB,UP_Button)) || (!Read_Bit(PORTB,DOWN_Button)));
      }
	  }