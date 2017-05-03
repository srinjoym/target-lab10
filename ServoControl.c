#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Random.h"
#include "TExaS.h"


uint32_t UP_COUNT = 19500;
uint32_t DOWN_COUNT = 31000;
uint32_t TOTAL_COUNT = 319770;
uint32_t CYCLE_COUNT = 10;


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void PortE_Init(){
	
	uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x0000010; 		
  delay = SYSCTL_RCGCGPIO_R;					
	GPIO_PORTE_DIR_R |= 0x07F;      
  GPIO_PORTE_DEN_R |= 0x07F;     
	GPIO_PORTE_AFSEL_R &= ~0xFF;

}


void ServoUp(uint8_t servo){
	if(servo == 5)
		UP_COUNT = 18000;
	else if(servo == 0)
		UP_COUNT = 18750;
	
		
	DisableInterrupts();
	uint32_t pulseCounter = UP_COUNT;
	uint32_t inverseCounter = (TOTAL_COUNT-pulseCounter);
	uint16_t cycleCount = CYCLE_COUNT;
	uint8_t mask;
	
	mask = 0x01 << (servo);
	
	while(cycleCount > 0){
		
		pulseCounter = UP_COUNT;
		inverseCounter = (TOTAL_COUNT-pulseCounter);
		
		GPIO_PORTE_DATA_R |= mask;
		while(pulseCounter > 0){
			pulseCounter --;
		}
		GPIO_PORTE_DATA_R ^= mask;
		while(inverseCounter > 0){
			inverseCounter --;
		}
		cycleCount --;
	}
	
	EnableInterrupts();
}

void ServoDown(uint8_t servo){
	
	if((servo == 1) ||( servo ==3))
		DOWN_COUNT = 32000;

	DisableInterrupts();
	uint32_t pulseCounter = DOWN_COUNT;
	uint32_t inverseCounter = (TOTAL_COUNT-pulseCounter);
	uint16_t cycleCount = CYCLE_COUNT;
	uint8_t mask;
	
	mask = 0x01 << (servo);
	
	while(cycleCount > 0){
		
		pulseCounter = DOWN_COUNT;
		inverseCounter = (TOTAL_COUNT-pulseCounter);
		
		GPIO_PORTE_DATA_R |= mask;
		while(pulseCounter > 0){
			pulseCounter --;
		}
		GPIO_PORTE_DATA_R ^= mask;
		while(inverseCounter > 0){
			inverseCounter --;
		}
		cycleCount --;
	}
	EnableInterrupts();
}





