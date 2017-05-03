//// SysTick.c
//// Implements two busy-wait based delay routines
//#include <stdint.h>
//#include "tm4c123gh6pm.h"
//// Initialize SysTick with busy wait running at bus clock.
//#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
//#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
//#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))
//	
//void (*PeriodicTask3)(void);   // user function

//void SysTick_Init(void(*task)(void)){
//		PeriodicTask3 = task;          // user function
//	  NVIC_ST_CTRL_R &= (~0x01);
//    NVIC_ST_RELOAD_R = 1000000 - 1;
//    NVIC_ST_CURRENT_R = 0;
//    NVIC_SYS_PRI3_R=(NVIC_SYS_PRI3_R&0X00FFFFFF)|0X80000000;//Priority 4
//    NVIC_ST_CTRL_R |= 0x07;
//}

//void SysTick_Handler(void){ // every 25 ms
// //Similar to Lab9 except rather than grab sample and put in mailbox
// //        format message and transmit 
//	(*PeriodicTask3)();                // execute user task
//}