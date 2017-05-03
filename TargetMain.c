#include <stdint.h>
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FiFo.h"
#include "TargetInterrupts.h"
#include "ServoControl.h"
#include "Random.h"
#include "Timer0.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
	
uint8_t ready_bit;
uint8_t active_target[2];
uint8_t player_mode;
uint8_t game_status;
uint8_t game_mode = -1;
int32_t p1_score;
int32_t p2_score;
uint32_t count_time = 20; 
uint32_t level = 1;
uint8_t count_over = 0;
uint8_t count_targets[6] = {1,0,0,0,0,0};


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
//initialize systick for uart transmission	
	  NVIC_ST_CTRL_R &= (~0x01);
    NVIC_ST_RELOAD_R = 100000 - 1;
    NVIC_ST_CURRENT_R = 0;
    NVIC_SYS_PRI3_R=(NVIC_SYS_PRI3_R&0X00FFFFFF)|0X80000000;//Priority 4
    NVIC_ST_CTRL_R |= 0x07;
}


void ServoAction(uint8_t num_player){
	
	//ServoDown(active_target[num_player]);
	ServoDown(active_target[num_player]);
	
	uint8_t rand = Random()%3;
	
	if(num_player==1)
		rand+=3;
	
	active_target[num_player] = rand;
	ServoUp(rand);

}


void SpeedShotHandler(void){
	
		if(hit_array[0]==1&&active_target[0]==0){
			p1_score++;
			ServoAction(0);
		}
		else if (hit_array[1]==1&&active_target[0]==1){
			p1_score++;
			ServoAction(0);
		}
		else if (hit_array[2]==1&&active_target[0]==2){
			p1_score++;
			ServoAction(0);
		}
		
		if(hit_array[3]==1&&active_target[1]==3){
		//if(1){
			p2_score++;
			ServoAction(1);
		}
		else if (hit_array[4]==1&&active_target[1]==4){
			p2_score++;
			ServoAction(1);
		}
		else if (hit_array[5]==1&&active_target[1]==5){
			p2_score++;
			ServoAction(1);
		}

}

void count_set_target(){
	for(int i = 0; i<6; i++){
				if(count_targets[i]==0){
					count_targets[i] = 1;
					ServoUp(i);
					break;
				}
			}
}
void CountShotHandler(void){
	
		if(hit_array[0]==1&&count_targets[0]==1){
			p1_score++;
			ServoDown(0);

		}
		if (hit_array[1]==1&&count_targets[1]==1){
			p1_score++;
			ServoDown(1);

		}
		if (hit_array[2]==1&&count_targets[2]==1){
			p1_score++;
			ServoDown(2);

		}
		
		if(hit_array[3]==1&&count_targets[3]==1){
		//if(1){
			p2_score++;
			ServoDown(3);

		}
		if (hit_array[4]==1&&count_targets[4]==1){
			p2_score++;
			ServoDown(4);

		}
		if (hit_array[5]==1&&count_targets[5]==1){
			p2_score++;
			ServoDown(5);

		}

}

void CountServosMove(){
	for(int i = 0; i< 6; i++){
			if (count_targets[i]==1)
				ServoUp(i);
			else
				ServoDown(i);
	}
}

void SpeedLoop(){
			if(game_status == 0){
				for(int i = 0; i<6; i++){
					ServoDown(i);
				}
				p1_score = 0;
				p2_score = 0;
			if(player_mode==1){
				active_target[0] = 0;
				ServoUp(active_target[0]);
			}
			else if(player_mode==2){
				active_target[0] = 0;
				ServoUp(active_target[0]);
				active_target[1] = 5;
				ServoUp(active_target[1]);
			}

		}
		else if(game_status ==1)
			SpeedShotHandler();
		else if(game_status ==2){
			ServoDown(active_target[0]);
			ServoDown(active_target[1]);
		}
}

void CountdownLoop(){
		if(game_status == 0){
			p1_score = 0;
			p2_score = 0;
			for(int i =0; i<6; i++){
				count_targets[i]=0;
			}
			count_targets[0] = 1;
			ServoUp(0);
			count_over = 0;
		}
		else if(game_status ==1){
			uint32_t old_level = level;
			level = p1_score/10;
			if(level>old_level)
				count_time-=2;
			
			if(count_time == 0)
				count_over = 1;
			
			uint8_t flag = 0;
			for(int i = 0; i<6; i++){
				if(count_targets[i]==0){
					flag = 1;
				}
			}
			if(flag==0)
				count_over = 1;
			
			if(count_over!=1){
				CountShotHandler();
				CountServosMove();
			}
		}
		else if(game_status ==2){
			ServoDown(active_target[0]);
		}
}

void Timer0Handler(void){
	TIMER0_TAILR_R = ((count_time)/0.0000000125)-1;
}


int main (void){ 
	
	DisableInterrupts();
  
  TExaS_Init();       // Bus clock is 80 MHz 
	PortE_Init();				//for servos
	FiFo_Init();				//uart
	UART_Init();				
  PortF_Init();				//led
	SysTick_Init();			//uart
	GPIOPortB_Init();		//port b edge trigger

	if(game_mode==1){
		Timer0_Init(&Timer0Handler, ((count_time)/0.0000000125));
	}
	EnableInterrupts();
	
	while(1){
		
		if(game_mode == 0)
			SpeedLoop();
		else if(game_mode ==1)
			CountdownLoop();
		
		
		
		while(ready_bit==1){ //wait until ready bit on transmission 
			ready_bit = 0;
		}
		
		char bit;
		
		while(bit != 0x02){ //while uart is not start bit keep looping
			FiFo_Get(&bit);
		}
		FiFo_Get(&bit);
		game_mode = bit;
		
		FiFo_Get(&bit);
		game_status = bit;
		
		FiFo_Get(&bit);
		player_mode = bit;

		FiFo_Get(&bit);	//skip end bits
		FiFo_Get(&bit);
		
	}
	
}

/* SysTick ISR
*/
void SysTick_Handler(void){
	//update hit array based on updates from edge triggered and output
	uint8_t array[6];
	GPIO_PORTF_DATA_R ^= 0x04;
	GPIO_PORTF_DATA_R ^= 0x04;
	array[0] = 0x02;
	array[1] = p1_score;
	array[2] = p2_score;
	array[3] = count_over;
	array[4] = 0x0D;
	array[5] = 0x03;
	
	for (int i =0; i<6; i++){
		UART_OutChar(array[i]);
	}

	ready_bit = 1;
	GPIO_PORTF_DATA_R ^= 0x04;
	
}


