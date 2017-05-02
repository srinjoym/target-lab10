#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "ServoControl.h"



void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

uint8_t hit_array[] = {0, 0, 0, 0, 0, 0};

volatile uint8_t flag;
void GPIOPortB_Init(void){
	
	DisableInterrupts();
	SYSCTL_RCGCGPIO_R |= 0x02;
	flag = 0;
	GPIO_PORTB_AMSEL_R &= ~0x3F;
	GPIO_PORTB_PCTL_R &= ~0x000F0000;
	GPIO_PORTB_DIR_R &= ~0x3F;
	GPIO_PORTB_AFSEL_R &= ~0x3F;
	GPIO_PORTB_DEN_R |= 0x3F;
	GPIO_PORTB_IS_R &= ~0x3F;
	GPIO_PORTB_IBE_R &= ~0x3F;
	GPIO_PORTB_IEV_R |= 0x3F;
	GPIO_PORTB_ICR_R = 0x3F;
	GPIO_PORTB_IM_R |= 0x3F;
	NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF0FFF) | 0x00004000;
	NVIC_EN0_R = 0x02;
	EnableInterrupts();
}



void GPIOPortB_Handler(void){
	
	uint8_t indexArr = 0;
	uint8_t checkReg = 0;
	checkReg = GPIO_PORTB_RIS_R;
	GPIO_PORTB_ICR_R |= 0x3F;
	hit_array[0] = 0;
	hit_array[1] = 0;
	hit_array[2] = 0;
	hit_array[3] = 0;
	hit_array[4] = 0;
	hit_array[5] = 0;
	
	if(checkReg == 1){
		indexArr = 0;
	}
	else if(checkReg == 2){
		indexArr = 1;
	}
	else if(checkReg == 4){
		indexArr = 2;
	}
	else if(checkReg == 8){
		indexArr = 3;
	}
	else if(checkReg == 16){
		indexArr = 4;
	}
	else if(checkReg == 32){
		indexArr = 5;
	}
	
	
	hit_array[indexArr] =	0x01;

	
	
	
}
