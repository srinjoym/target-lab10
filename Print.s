; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
	PRESERVE8
    THUMB
	

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
castNum EQU 0
LCD_OutDec
    PUSH {R11}
    SUB SP, #4 ;allocate
    MOV R11, SP ;base pointer
    CMP R0, #10
    BLO print
    PUSH {R0,LR} ;push current val to stack
    LDR R1, =10  
    UDIV R0, R1  ;divide val by 10
    STR R0, [R11,#castNum] ;save divided value into currentNum
    BL LCD_OutDec
    POP {R2,LR}               ;get original val and link register
    LDR R0, [R11,#castNum]
    MUL R0, R1              ;multiply truncated value by  10
   
    SUBS R0, R2, R0         ;subtract cast value from original value to get next digit     
print
 
    ADD R0, R0, #48
    PUSH {LR,R1}
    BL ST7735_OutChar
    POP {LR,R1}
 
end ADD SP, #4
    POP {R11}
    BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
maxNum			EQU 0
tenNum 			EQU 4
hundredNum 		EQU 8
thousandNum 	EQU 12
firstNum 		EQU 16
secondNum 		EQU 20
thirdNum 		EQU 24
fourthNum 		EQU 28
asteriskChar	EQU 32
decimalChar 	EQU 36

	
	
	PUSH {R11, LR}
	SUBS SP, #40	;allocate
	MOV R1, #9999
	STR R1, [SP, #maxNum]	;maximum number for comparison - asterisks if greater than
	MOV R1, #10 
	STR R1, [SP, #tenNum]	;the number 10
	MOV R1, #100
	STR R1, [SP, #hundredNum] ;the number 100
	MOV R1, #1000
	STR R1, [SP, #thousandNum] ;the numer 1000
	MOV R1, #0x2A
	STR R1, [SP, #asteriskChar]	;ascii value for *
	MOV R1, #0x2E
	STR R1, [SP, #decimalChar]	;ascii value for .
	
	MOV R11, SP
	
	LDR R2, [R11, #maxNum]
	CMP R0, R2							;check if asterisks should be printed
	BHI printAsterisks
	LDR R3, [R11, #thousandNum]
	UDIV R2, R0, R3						;determine first digit
	STR R2, [R11, #firstNum]
	LDR R3, [R11, #tenNum]	
	LDR R2, [R11, #firstNum]
	MUL R2, R2, R3						;determine value of first digit (after dividing)
	LDR R1, [R11, #hundredNum]
	UDIV R3, R0, R1						;get 2nd digit in place of first digit (if this were decimal)
	SUBS R3, R3, R2						;subtract value of first digit to isolate 2nd digit
	STR R3, [R11, #secondNum]
	LDR R1, [R11, #firstNum]
	LDR R2, [R11, #hundredNum]			;determine value of first digit (after dividing)
	MUL R1, R1, R2
	LDR R2, [R11, #secondNum]			;determine value of second digit (after dividing)
	LDR R3, [R11, #tenNum]
	MUL R2, R2, R3
	ADD R1, R1, R2
	LDR R2, [R11, #tenNum]
	UDIV R3, R0, R2						;move 3rd digit into place of first digit (if this were decimal) 
	SUBS R3, R3, R1						;subtract values of first and second digits to isolate 3rd digit
	STR R3, [R11, #thirdNum]
	LDR R1, [R11, #firstNum]
	LDR R2, [R11, #thousandNum]
	MUL R1, R1, R2						;determine value of first digit
	LDR R2, [R11, #secondNum]
	LDR R3, [R11, #hundredNum]
	MUL R2, R2, R3						;determine value of second digit
	ADD R1, R1, R2
	LDR R2, [R11, #thirdNum]
	LDR R3, [R11, #tenNum]
	MUL R2, R2, R3						;determine value of third diit
	ADD R1, R1, R2
	SUBS R2, R0, R1						;subtract value of first second and third digits to isolate first digit
	STR R2, [R11, #fourthNum]
	LDR R1, [R11, #firstNum]
	ADD R1, R1, #0x30
	STR R1, [R11, #firstNum]			;adjust to ascii values
	LDR R1, [R11, #secondNum]
	ADD R1, R1, #0x30
	STR R1, [R11, #secondNum]
	LDR R1, [R11, #thirdNum]
	ADD R1, R1, #0x30
	STR R1, [R11, #thirdNum]
	LDR R1, [R11, #fourthNum]
	ADD R1, R1, #0x30
	STR R1, [R11, #fourthNum]
	LDR R0, [R11, #firstNum]
	BL ST7735_OutChar
	LDR R0, [R11, #decimalChar]
	BL ST7735_OutChar					;print
	LDR R0, [R11, #secondNum]
	BL ST7735_OutChar
	LDR R0, [R11, #thirdNum]
	BL 	ST7735_OutChar
	LDR R0, [R11, #fourthNum]
	BL ST7735_OutChar


back

	ADD SP, SP, #40
	POP {R11, LR}
	
	
     BX   LR
	 
printAsterisks
	PUSH {LR}
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2E
	BL ST7735_OutChar		;print asterisks
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2A
	BL ST7735_OutChar
	POP {LR}
	B back
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
