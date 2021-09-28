SW0	EQU	P3.2
SW1	EQU	P3.3
SW2	EQU	P3.4
SW3	EQU	P3.5
	
;		Pin para controlar el IC	
D_1	EQU	P0.0		;	Display 0
D_2	EQU	P0.1		;	Display	1
D_3	EQU	P0.2		;	Display 2
D_4	EQU	P0.3		;	Display 3

S_H	EQU	P2.0		;	Pin 11	Clock para ingresar datos
S_T	EQU	P2.1		;	Pin	12	Trigger para ingresar datos al IC
D_S	EQU	P2.2		;	Pin 14	Input de datos seriales

R_DIS0	EQU	30H 	;	Storage	Almacenar datos en RAM 30h del Display 0	
R_DIS1	EQU	31H		;	Storage Almacenar datos en RAM 31h del Display 1
R_DIS2	EQU	32H		;	Storage Almacenar datos en RAM 32h del Display 2 
R_DIS3	EQU	33H		;	Storage Almacenar datos en RAM 33h del Display 3

R_N		EQU	34H		;	Contador para entrar 4 veces y desplegar datos uno a uno
R_P		EQU	35H		;	Almacena 00FEH para mandarlo al puerto y encender los Display's
;R_D		EQU 36H		;	

R_ADDR	EQU 36H		;	Guardar posicion del valor del DISPLAY0
BUFFER	EQU	37H		;	Almacenar datos del puerto serie
P_DATA	EQU	38H		;	Posicion de datos para mostrar en display
B_SWTCH EQU 7FH
;-------		Reset		-------;
ORG 0000H	
	LJMP	SETUP
;-----------------------------------	
;-------	External INT0	-------;
ORG	0003H
	LJMP	INT_0
;-----------------------------------	
;-------	Timer 0 [TF0]	-------;	
ORG 000BH	
	CLR		TR0	
	LJMP	TIMER_0
;-----------------------------------	
;-------	External INT1	-------;
ORG 0013H		
	LJMP	INT_1
;-----------------------------------	
;-------	Timer 1 [TF1]	-------;
ORG 001BH
	LJMP	TIMER_1
;-----------------------------------
;---	Serial COM [RI & TI]	---;
ORG	0023H		
	LJMP	SERIAL
;-----------------------------------	
;-------		Setup		-------;
ORG	0030H
	SETUP:
	MOV		DPTR,#BCD_A		; Cargar DPTR con la posicion de los datos de la FLASH
	;	CONFIGURE PORT's
	SETB	SW0
	SETB	SW1
	SETB	SW2
	SETB	SW3
	MOV		P0,#000H
	MOV		P1,#000H
	MOV		P2,#000H
	MOV		P2,#0FFH		
	;	CONFIGURE TIMER's
	MOV		TH0,#0EDH		
	MOV		TL0,#0FFH		
	MOV		TH1,#0FDH		; 9600 baud rate
	MOV		TMOD,#021H		; T1->[Mode 2,8 bit autoreload] T0->[Mode 1, 16 bit no autoreload]
	MOV		SCON,#050H		; 8-BIT, 1 STOP
	SETB	TR0				; Iniciar Timer0
	SETB	TR1				; Iniciar Timer1
	;	CONFIGURE INTERRUPT's
	MOV		IE,#10010111B	; | EA  |  -  | ET2 | ES  | ET1 | EX1 | ET0 | EX0 |	
	;TCON	| TF1 | TR1 | TF0 | TR0 | IE1 | IT1 | IE0 | IT0 |
	SETB	TCON.1			; Activar interrupcion por H->L del INT0 
	SETB	TCON.4			; Activar interrupcion por H->L del INT1
	;	CONFIGURE SERIAL
			
	;	INICIAR VARIABLES
	MOV		R_ADDR,#R_DIS0	; CARGAR POSICION 0030H DE RAM EN R_ADDR
	MOV		R_DIS0,#00		; Valor a cargar en DIS 0 
	MOV		R_DIS1,#00		; Valor a cargar en DIS 1
	MOV		R_DIS2,#00		; Valor a cargar en DIS 2
	MOV		R_DIS3,#00		; Valor a cargar en DIS 3
	MOV		R_N,#00			; CONTADOR DE VECES
	MOV		R_P,#11111110B	; SELECTOR DE PUERTO
	MOV		BUFFER,#00
	MOV		P_DATA,#00
	CLR		D_S
	CLR		S_H
	CLR 	S_T 
	CLR		B_SWTCH			; CLR bit de RAM para filtrar dato del buffer 
	;-------------------------------------
	LJMP	MAIN
;-----------------------------------------	
;------------------- MAIN ----------------	
ORG 0100H
	MAIN:
			LCALL	SWITCH_DIS
						
			
			LJMP	MAIN
;------------------- END MAIN ------------------			
;-----------------------------------------	
;------------------- FUNCTION ----------------	
SWITCH_DIS:	
			JB		B_SWTCH,JUMP0
			RET
			
	JUMP0:	CLR	IE.7
			CLR		B_SWTCH
			MOV		R7,BUFFER
			CJNE	R7,#'0',CASE0
			MOV		BUFFER,#0
			JMP		BREAK
	CASE0:	CJNE	R7,#'1',CASE1
			MOV		BUFFER,#1
			JMP		BREAK
	CASE1:	CJNE	R7,#'2',CASE2
			MOV		BUFFER,#2
			JMP		BREAK
	CASE2:	CJNE	R7,#'3',CASE3
			MOV		BUFFER,#3
			JMP		BREAK
	CASE3:	CJNE	R7,#'4',CASE4
			MOV		BUFFER,#4
			JMP		BREAK	
	CASE4:	CJNE	R7,#'5',CASE5
			MOV		BUFFER,#5
			JMP		BREAK
	CASE5:	CJNE	R7,#'6',CASE6
			MOV		BUFFER,#6
			JMP		BREAK
	CASE6:	CJNE	R7,#'7',CASE7
			MOV		BUFFER,#7
			JMP		BREAK
	CASE7:	CJNE	R7,#'8',CASE8
			MOV		BUFFER,#8
			JMP		BREAK
	CASE8:	CJNE	R7,#'9',CASE9
			MOV		BUFFER,#9
			JMP		BREAK
	CASE9:	CJNE	R7,#13,CASE10
			MOV		R_DIS3,#00			
			MOV		R_DIS2,#00
			MOV		R_DIS1,#00
			MOV		R_DIS0,#00
			MOV		BUFFER,#00
			JMP		BREAK		
	CASE10:	MOV		BUFFER,#0
	BREAK:	
			MOV		R7,P_DATA
			CJNE	R7,#01,CASE_1
			MOV		R_DIS3,R_DIS2			
			MOV		R_DIS2,R_DIS1
			MOV		R_DIS1,R_DIS0
			MOV		R_DIS0,BUFFER
			JMP		EXIT
	CASE_1:	CJNE	R7,#02,CASE_2
			MOV		R_DIS3,R_DIS2			
			MOV		R_DIS2,R_DIS1
			MOV		R_DIS1,R_DIS0
			MOV		R_DIS0,BUFFER
			JMP		EXIT
	CASE_2:	CJNE	R7,#03,CASE_3
			MOV		R_DIS3,R_DIS2			
			MOV		R_DIS2,R_DIS1
			MOV		R_DIS1,R_DIS0
			MOV		R_DIS0,BUFFER
			JMP		EXIT
	CASE_3:	CJNE	R7,#04,EXIT
			MOV		R_DIS3,R_DIS2
			MOV		R_DIS2,R_DIS1
			MOV		R_DIS1,R_DIS0
			MOV		R_DIS0,BUFFER
	EXIT:	
	SETB	IE.7
	
	RET	
;----------------- END FUNCTION ----------------	
;-----------------------------------------------
;------------------ ISR TIMER 0 ---------------
ORG	0400H
TIMER_0:
			;SEND DATA TO BCD
			MOV		P0,#0FFH
			MOV		R0,#8		;DATA TO SEND
			MOV		R1,R_ADDR	;LOAD DATA
			CLR		C 		;OBTENER VALOR DE LA POSICION R1
			MOV		A,@R1
			MOVC	A,@A+DPTR
SEND_B:		CLR		S_H
			RLC		A
			MOV		D_S,C
			SETB	S_H
			DJNZ	R0,SEND_B
			MOV		P0,R_P
			SETB	S_T
			CLR		D_S
			CLR		S_H
			CLR 	S_T
			MOV		R0,#R_N
			CJNE	@R0,#04,CONT_T
			MOV		R_N,#00
			MOV		R_P,#11111110B
			MOV		R_ADDR,#R_DIS0
			SJMP	RELOAD_T0
CONT_T:		INC		R_N
			MOV		A,R_P
			RL		A
			MOV		R_P,A
			INC		R_ADDR
RELOAD_T0:	;RELOAD REGISTER TIMER_0
			MOV		TH0,#0EDH
			MOV		TL0,#0FFH
			SETB		TR0
			RETI
;--------------- END ISR TIMER 0 ---------------------
;-----------------------------------------------------
;---------------   ISR TIMER 1   ---------------------
ORG	0450H
TIMER_1:	
			RETI
;--------------- END ISR TIMER 1 ---------------------
;-----------------------------------------------------
;---------------    ISR INT 0    -----------------------
ORG	0500H
INT_0:		CLR		P2.7
			RETI
;--------------- END ISR INT 0 ---------------------
;-----------------------------------------------------
;--------------    ISR INT 1    ---------------------
ORG	0550H
INT_1:		SETB	P2.7
			RETI
;--------------- END ISR INT 1 ---------------------
;-----------------------------------------------------
;---------------  ISR SERIAL   ---------------------
ORG 0600H
SERIAL:		JB		TI,TRANS
			MOV		BUFFER,SBUF	; Storage en BUFFER
			CLR		IE.7
			MOV 	SBUF,BUFFER	; Devolver datos por Tx
			CLR 	RI 			; clear RI 
			SETB	B_SWTCH		; Entrar a switch	
			MOV		R0,P_DATA
			CJNE	R0,#04,NOT_E
			MOV		P_DATA,#00
NOT_E:		INC		P_DATA		; Inrementar cada recepciom
			CLR		T1		
			SETB	IE.7 		; Enable serial INT
			RETI 				; Come back
TRANS:		CLR		TI
			RETI
;--------------- END ISR SERIAL ---------------------

ORG	0650H			
BCD_A:DB	03FH,006H,05BH,04FH,066H,06DH,07DH,007H,07FH,06FH	
BCD_C:DB	0C0H,0F9H,0A4H,0B0H,099H,092H,082H,0F8H,080H,090H
END
; Terminado
