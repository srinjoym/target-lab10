#include <stdint.h>
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FiFo.h"
#include "TargetInterrupts.h"
#include "ServoControl.h"
#include "Random.h"

void ServoAction(uint8_t num_player){
	
	//ServoDown(active_target[num_player]);
	ServoDown(active_target[num_player]);
	
	uint8_t rand = Random()%3;
	
	if(num_player==1)
		rand+=3;
	
	active_target[num_player] = rand;
	ServoUp(rand);

}



void ShotHandler(uint8_t p1_score,uint8_t p2_score,uint8_t active_target[2]){
	
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

void SpeedLoop(uint8_t game_status, uint8_t p1_score, uint8_t p2_score, uint8_t player_mode, uint8_t active_target[2]){
	
	if(game_status == 0){
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
			ShotHandler();
		else if(game_status ==2){
			ServoDown(active_target[0]);
			ServoDown(active_target[1]);
		}
		
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

