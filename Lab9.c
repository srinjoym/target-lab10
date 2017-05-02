// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 4/10/2017 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// UART1 on PC4-5
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w FiFo on the receiver end 
//    (we suggest implementing and testing this first)

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FiFo.h"
#include "TargetInterrupts.h"
#include "ServoControl.h"
//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia
int mailbox;
uint8_t flag3;
uint32_t TxCounter = 0;
uint8_t shot_trig=0;
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

uint8_t servo_array[] = {0,0,0,0,0,0};
uint8_t servo_state[] = {0,0,0,0,0,0};

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.001 cm


// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
// Intialize PortF for hearbeat
	SYSCTL_RCGCGPIO_R |= 0x21;
    volatile uint32_t delay = 1000;
	delay = 1001;
	delay = 1002;
    GPIO_PORTF_DIR_R |= 0x0E;
    GPIO_PORTF_DEN_R |= 0x0E;
    GPIO_PORTF_AFSEL_R &= ~0x0E;
    GPIO_PORTF_AMSEL_R &= ~0x0E;
    GPIO_PORTF_PUR_R |= 0x0E;
}


void SysTick_Init(void){
	
	  NVIC_ST_CTRL_R &= (~0x01);
    NVIC_ST_RELOAD_R = 100000 - 1;
    NVIC_ST_CURRENT_R = 0;
    NVIC_SYS_PRI3_R=(NVIC_SYS_PRI3_R&0X00FFFFFF)|0X80000000;//Priority 4
    NVIC_ST_CTRL_R |= 0x07;
}

void servo_action(){
	for(int i =0; i<6;i++){
		if(servo_array[i]==1 && servo_state[i]==0)
		{
			ServoUp(i);
		}
		else if(servo_array[i]==0&& servo_state[i]==1){
			ServoDown(i);
		}
	}
	
}
// final main program for bidirectional communication
// Sender sends using SysTick Interrupt
// Receiver receives using RX


int main
	(void){ 
  
  TExaS_Init();       // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB);
	PortE_Init();
	FiFo_Init();
	UART_Init();
  ADC_Init();    // initialize to sample ADC
  PortF_Init();
	SysTick_Init();
	GPIOPortB_Init();
  UART_Init();       // initialize UART
	//ST7735_SetCursor(0,0);
  //LCD_OutFix(0);
  //ST7735_OutString(" cm");
	
//Enable SysTick Interrupt by calling SysTick_Init()
  //EnableInterrupts();
	int position;
	char string[] = {'c','m'};
	ST7735_SetCursor(0,0);
	while(1){
		while(flag3==1){
			flag3 = 0;
		}
		servo_action();
		char bit;
		
		while(bit != 0x02){
			FiFo_Get(&bit);
		}
		for(int i = 0; i<6;i++){
			servo_state[i] = servo_array[i];
			//ST7735_OutChar(inti+48);
		}
		ST7735_SetCursor(0,0);
		for(int i = 0; i<6;i++){
			FiFo_Get(&bit);
			servo_array[i] = bit;
			//ST7735_OutChar(inti+48);
		}
		FiFo_Get(&bit);
		shot_trig = bit;
		FiFo_Get(&bit);
		FiFo_Get(&bit);
		
	
}
}

/* SysTick ISR
*/
void SysTick_Handler(void){ // every 25 ms
 //Similar to Lab9 except rather than grab sample and put in mailbox
 //        format message and transmit 
	uint8_t array[9];
	GPIO_PORTF_DATA_R ^= 0x04;
	GPIO_PORTF_DATA_R ^= 0x04;
	array[0] = 0x02;
	array[1] = hit_array[0];
	array[2] = hit_array[1];
	array[3] = hit_array[2];
	array[4] = hit_array[3];
	array[5] = hit_array[4];
	array[6] = hit_array[5];
	array[7] = 0x0D;
	array[8] = 0x03;
	
	for (int i =0; i<9; i++){
		UART_OutChar(array[i]);
	}
	hit_array[0]=0;
		hit_array[1]=0;
		hit_array[2]=0;
		hit_array[3]=0;
		hit_array[4]=0;
		hit_array[5]=0;
	flag3 = 1;
	GPIO_PORTF_DATA_R ^= 0x04;
	
}


