
_DIO_Inti:

;DIO_Inti.c,5 :: 		void DIO_Inti(void){
;DIO_Inti.c,7 :: 		TRISA=0xC7;
	MOVLW      199
	MOVWF      TRISA+0
;DIO_Inti.c,8 :: 		TRISB=0x07;
	MOVLW      7
	MOVWF      TRISB+0
;DIO_Inti.c,9 :: 		TRISC=0x01;
	MOVLW      1
	MOVWF      TRISC+0
;DIO_Inti.c,10 :: 		TRISD=0x00;
	CLRF       TRISD+0
;DIO_Inti.c,11 :: 		TRISE=0x00;
	CLRF       TRISE+0
;DIO_Inti.c,13 :: 		}
L_end_DIO_Inti:
	RETURN
; end of _DIO_Inti
