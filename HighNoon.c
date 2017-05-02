// HighNoon.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 3/6/2015 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "common.h"
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "IO.h"
#include "BackgroundSprites.h"
#include "Lobby.h"
#include "Character.h"
#include "GameMode.h"
#include "SpeedMode.h"
#include "PracticeMode.h"
#include "CountdownMode.h"

uint8_t num_players;
uint8_t game_mode;



player players[2];

//void PortF_Init(void){
//// Intialize PortF for hearbeat
//		volatile uint8_t temp;
//		SYSCTL_RCGCGPIO_R |= 0x21;
//		temp =1;
//		temp = 2;
//    GPIO_PORTF_DIR_R |= 0x1F;
//    GPIO_PORTF_DEN_R |= 0x1F;
//    GPIO_PORTF_AFSEL_R &= ~0x0E;
//    GPIO_PORTF_AMSEL_R &= ~0x0E;
//    GPIO_PORTF_PUR_R |= 0x0E;
//}



// *************************** Capture image dimensions out of BMP**********




int main4(void){
  TExaS_Init();  // set system clock to 80 MHz
	ST7735_InitR(INITR_REDTAB);
	IO_Init();
	//PortF_Init();
  ST7735_FillScreen(0x0000);            // set screen to black

  ST7735_DrawBitmap(0,158,splash,128,160);
	IO_Touch();
	
  while(1){
		num_players = Lobby();
		
		players[0].sprite = SelectCharacter(1);
		if(num_players == 2)
			players[1].sprite= SelectCharacter(2);
		game_mode = GameMode();
		if(game_mode == 0)
			SpeedMode(num_players,&players[0]);
		//else if(game_mode==1)
			//PracticeMode();
		//else
			//CountdownMode();
  }

}





