
_E2PROM_Read:

;E2PROM.c,4 :: 		unsigned char E2PROM_Read(unsigned int addr)
;E2PROM.c,10 :: 		ah=(addr&0x0100)>>8;
	MOVLW      0
	ANDWF      FARG_E2PROM_Read_addr+0, 0
	MOVWF      R3+0
	MOVF       FARG_E2PROM_Read_addr+1, 0
	ANDLW      1
	MOVWF      R3+1
	MOVF       R3+1, 0
	MOVWF      R0+0
	CLRF       R0+1
	MOVF       R0+0, 0
	MOVWF      E2PROM_Read_ah_L0+0
;E2PROM.c,11 :: 		al=addr&0x00FF;
	MOVLW      255
	ANDWF      FARG_E2PROM_Read_addr+0, 0
	MOVWF      E2PROM_Read_al_L0+0
;E2PROM.c,13 :: 		I2C1_Start();
	CALL       _I2C1_Start+0
;E2PROM.c,14 :: 		if(ah)
	MOVF       E2PROM_Read_ah_L0+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_E2PROM_Read0
;E2PROM.c,16 :: 		I2C1_Wr(0xA2);
	MOVLW      162
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,17 :: 		}
	GOTO       L_E2PROM_Read1
L_E2PROM_Read0:
;E2PROM.c,20 :: 		I2C1_Wr(0xA0);
	MOVLW      160
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,21 :: 		}
L_E2PROM_Read1:
;E2PROM.c,22 :: 		I2C1_Wr(al);
	MOVF       E2PROM_Read_al_L0+0, 0
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,24 :: 		I2C1_Start();
	CALL       _I2C1_Start+0
;E2PROM.c,25 :: 		if(ah)
	MOVF       E2PROM_Read_ah_L0+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_E2PROM_Read2
;E2PROM.c,27 :: 		I2C1_Wr(0xA3);
	MOVLW      163
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,28 :: 		}
	GOTO       L_E2PROM_Read3
L_E2PROM_Read2:
;E2PROM.c,31 :: 		I2C1_Wr(0xA1);
	MOVLW      161
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,32 :: 		}
L_E2PROM_Read3:
;E2PROM.c,33 :: 		ret=I2C1_Rd(1);
	MOVLW      1
	MOVWF      FARG_I2C1_Rd_ack+0
	CALL       _I2C1_Rd+0
	MOVF       R0+0, 0
	MOVWF      E2PROM_Read_ret_L0+0
;E2PROM.c,34 :: 		I2C1_Stop();
	CALL       _I2C1_Stop+0
;E2PROM.c,36 :: 		return ret;
	MOVF       E2PROM_Read_ret_L0+0, 0
	MOVWF      R0+0
;E2PROM.c,37 :: 		}
L_end_E2PROM_Read:
	RETURN
; end of _E2PROM_Read

_E2PROM_Write:

;E2PROM.c,40 :: 		void E2PROM_Write(unsigned int addr, unsigned char val)
;E2PROM.c,47 :: 		tmp=val;
	MOVF       FARG_E2PROM_Write_val+0, 0
	MOVWF      E2PROM_Write_tmp_L0+0
	CLRF       E2PROM_Write_tmp_L0+1
;E2PROM.c,48 :: 		ah=(addr&0x0100)>>8;
	MOVLW      0
	ANDWF      FARG_E2PROM_Write_addr+0, 0
	MOVWF      R3+0
	MOVF       FARG_E2PROM_Write_addr+1, 0
	ANDLW      1
	MOVWF      R3+1
	MOVF       R3+1, 0
	MOVWF      R0+0
	CLRF       R0+1
	MOVF       R0+0, 0
	MOVWF      E2PROM_Write_ah_L0+0
;E2PROM.c,49 :: 		al=addr&0x00FF;
	MOVLW      255
	ANDWF      FARG_E2PROM_Write_addr+0, 0
	MOVWF      E2PROM_Write_al_L0+0
;E2PROM.c,50 :: 		nt=0;
	CLRF       E2PROM_Write_nt_L0+0
;E2PROM.c,52 :: 		do
L_E2PROM_Write4:
;E2PROM.c,54 :: 		I2C1_Start();
	CALL       _I2C1_Start+0
;E2PROM.c,55 :: 		if(ah)
	MOVF       E2PROM_Write_ah_L0+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_E2PROM_Write7
;E2PROM.c,57 :: 		I2C1_Wr(0xA2);
	MOVLW      162
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,58 :: 		}
	GOTO       L_E2PROM_Write8
L_E2PROM_Write7:
;E2PROM.c,61 :: 		I2C1_Wr(0xA0);
	MOVLW      160
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,62 :: 		}
L_E2PROM_Write8:
;E2PROM.c,63 :: 		I2C1_Wr(al);
	MOVF       E2PROM_Write_al_L0+0, 0
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,64 :: 		I2C1_Wr(tmp);
	MOVF       E2PROM_Write_tmp_L0+0, 0
	MOVWF      FARG_I2C1_Wr_data_+0
	CALL       _I2C1_Wr+0
;E2PROM.c,65 :: 		I2C1_Stop();
	CALL       _I2C1_Stop+0
;E2PROM.c,67 :: 		nt++;
	INCF       E2PROM_Write_nt_L0+0, 1
;E2PROM.c,69 :: 		while((tmp != E2PROM_Read(addr))&&(nt < 10));
	MOVF       FARG_E2PROM_Write_addr+0, 0
	MOVWF      FARG_E2PROM_Read_addr+0
	MOVF       FARG_E2PROM_Write_addr+1, 0
	MOVWF      FARG_E2PROM_Read_addr+1
	CALL       _E2PROM_Read+0
	MOVLW      0
	XORWF      E2PROM_Write_tmp_L0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__E2PROM_Write14
	MOVF       R0+0, 0
	XORWF      E2PROM_Write_tmp_L0+0, 0
L__E2PROM_Write14:
	BTFSC      STATUS+0, 2
	GOTO       L__E2PROM_Write11
	MOVLW      10
	SUBWF      E2PROM_Write_nt_L0+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L__E2PROM_Write11
	GOTO       L_E2PROM_Write4
L__E2PROM_Write11:
;E2PROM.c,70 :: 		}
L_end_E2PROM_Write:
	RETURN
; end of _E2PROM_Write
