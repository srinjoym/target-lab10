#include <stdint.h>
#include "common.h"
#include "ST7735.h"
#include "Character.h"
#include "IO.h"
#include "Timer0.h"
#include "CommonSprites.h"
#include "BackgroundSprites.h"
#include "Random.h"
#include "ServoControl.h"
#include "TargetInterrupts.h"
uint8_t flag2 = 0;
uint8_t num_seconds = 5;
//uint8_t hit_array[]={0,0,0,0,0,0};
uint8_t active_target[2];
int32_t last_score = -1;
int32_t last_score1 = -1;
int32_t last_score2 = -1;
uint8_t num_p;

void PrintScore(int32_t score){
	if (last_score != score){
		if(score>10)
			ST7735_SetCursor(9, 12);
		else
			ST7735_SetCursor(10, 12);
		ST7735_OutUDec(score,2);
		last_score = score;
	}
	
}

void PrintScore1(int32_t score){
	if (last_score1 != score){
		if(score>10)
			ST7735_SetCursor(3, 10);
		else
			ST7735_SetCursor(4, 10);
		ST7735_OutUDec(score,2);
		last_score1 = score;
	}
	
}
void PrintScore2(int32_t score){
	if (last_score2 != score){
		if(score>10)
			ST7735_SetCursor(15, 10);
		else
			ST7735_SetCursor(16, 10);
		ST7735_OutUDec(score,2);
		last_score2 = score;
	}
	
}
void ServoAction(uint8_t sprite,uint8_t num_player){
	for(int i = 0; i<6; i++){
		hit_array[i] = 0;
	}
	ServoDown(active_target[num_player]);
	
	for(int i =0; i<4; i++){
		if(num_p==1){
			ST7735_DrawBitmap(75,100,shots[sprite][i],45,45);
		}
		else{
			if(num_player==0){
				
				ST7735_DrawBitmap(20,75,shots[sprite][i],45,45);
			}
			else{
				ST7735_DrawBitmap(80,75,shots[sprite][i],45,45);
			}
		}
		uint32_t count = 2000000;
		while(count>0){
			count--;
		}
		count = 2000000;
	}
	uint8_t rand = Random()%3;
	if(num_player==1)
		rand+=3;
	active_target[num_player] = rand;
	ServoUp(rand);
}
void ShotHandler(player *player1,player *player2,uint8_t player_num){
	
		//if(hit_array[0]==1&&active_target[0]==0){
		if(1){
			(*player1).score++;
			ServoAction((*player1).sprite,0);
		}
		else if (hit_array[1]==1&&active_target[0]==1){
			(*player1).score++;
			ServoAction((*player1).sprite,0);
		}
		else if (hit_array[2]==1&&active_target[0]==2){
			(*player1).score++;
			ServoAction((*player1).sprite,0);
		}
		
		//if(hit_array[3]==1&&active_target[1]==3){
		if(1){
			(*player2).score++;
			ServoAction((*player2).sprite,1);
		}
		else if (hit_array[4]==1&&active_target[1]==4){
			(*player2).score++;
			ServoAction((*player2).sprite,1);
		}
		else if (hit_array[5]==1&&active_target[1]==5){
			(*player2).score++;
			ServoAction((*player2).sprite,1);
		}

}

void TimerHandler(void){
	
	if(num_seconds>0)
	{
		if(num_p==1){
		if(num_seconds>10)
			ST7735_SetCursor(3, 6);
		else
			ST7735_SetCursor(4, 6);
		ST7735_OutUDec(num_seconds,3);
		num_seconds--;
		}
		
		else
		{
			if(num_seconds>10)
			ST7735_SetCursor(9, 14);
		else
			ST7735_SetCursor(10, 14);
		
		ST7735_OutUDec(num_seconds,2);
		num_seconds--;
		}
		
		//ST7735_SetCursor(3, 6);
		
		//ShotHandler();
	}
	else{
		flag2 = 2;
		Timer0_Disable();
	}
}

void PlaySpeed1(player *player1){
	flag2 = 0;
	num_seconds = 30;
	active_target[0] = 0;
	
	(*player1).score = 0;
	ST7735_FillScreen(0x0000);            // set screen to black
	
	
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Speed Mode!");
	ST7735_OutString("\nPress Button 1 \nwhen ready to play");
	IO_Touch();
	PortE_Init();
	GPIOPortB_Init();
	ServoUp(active_target[0]);
	ST7735_FillScreen(0x0000);
	ST7735_DrawBitmap(15,17,SpeedMode_Text,98,13);
	ST7735_DrawBitmap(20,45,time_text,25,8);
	ST7735_DrawBitmap(48,115,score_text,30,8);
	ST7735_DrawBitmap(93,45,p1_text,9,8);
	//ST7735_DrawBitmap(0,158,SpeedModeBg,128,160);
	
	ST7735_DrawBitmap(74,97,chars[(*player1).sprite],45,45);
	Timer0_Init(&TimerHandler,80000000); // 1 second on device
	//Timer0_Init(&TimerHandler,8000000); // 1 second on sim
	while(flag2<2){
		ShotHandler(player1,player1,0);
		PrintScore((*player1).score);
		
		
		if (flag2 == 1)
		{
			PlaySpeed1(player1);
		}
	}
	ST7735_SetCursor(0, 2);
	
	ST7735_OutString("Time Over!");
	ST7735_OutString("\nB1 to play again");
	IO_Touch();
	

}

void PlaySpeed2(player *player1,player *player2){
	flag2 = 0;
	num_seconds = 30;
	active_target[0] = 0;
	active_target[1] = 5;
	(*player1).score = 0;
	ST7735_FillScreen(0x0000);            // set screen to black
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Speed Mode!");
	ST7735_OutString("\nPress Button 1 \nwhen ready to play");
	IO_Touch();
	ST7735_FillScreen(0x0000);            // set screen to black
	ST7735_DrawBitmap(15,17,SpeedMode_Text,98,13);
	ST7735_DrawBitmap(50,127,time_text,25,8);
	ST7735_DrawBitmap(15,90,score_text,30,8);
	ST7735_DrawBitmap(85,90,score_text,30,8);
	ST7735_DrawBitmap(27,30,p1_text,9,8);
	ST7735_DrawBitmap(90,30,p2_text,11,8);
	PortE_Init();
	GPIOPortB_Init();
	ServoUp(active_target[0]);
	ServoUp(active_target[1]);
	//ST7735_DrawBitmap(0,158,SpeedModeBg,128,160);
	//ST7735_DrawBitmap(74,97,chars[(*player1).sprite],45,45);
	Timer0_Init(&TimerHandler,80000000); // 1 second on device
	//Timer0_Init(&TimerHandler,8000000); // 1 second on sim
	while(flag2<2){
		ShotHandler(player1,player2,0);
		PrintScore1((*player1).score);
		PrintScore2((*player2).score);
		
		if (flag2 == 1)
		{
			PlaySpeed1(player1);
		}
	}
	ST7735_SetCursor(0, 2);
	
	ST7735_OutString("Time Over!");
	ST7735_OutString("\nB1 to play again");
	IO_Touch();
	

}



void SpeedMode(uint8_t num_players, player* player){
	
	num_p = num_players;
	if(num_players == 1)
		PlaySpeed1(player);
	else
		PlaySpeed2(&player[0],&player[1]);
	
}
