#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "CommonSprites.h"


uint8_t SelectCharacter(uint8_t player_num){
	uint8_t sel; 
	ST7735_FillScreen(0x0000);            // set screen to black
	
	ST7735_SetCursor(0, 0);
	if(player_num==1)
		ST7735_OutString("P1 choose character");
	else
		ST7735_OutString("P2 choose character");
	
	ST7735_DrawBitmap(15,75,chars_red[0],45,45);
	ST7735_DrawBitmap(70,75,chars[1],45,45);
	ST7735_DrawBitmap(15,130,chars[2],45,45);
	ST7735_DrawBitmap(70,130,chars[3],45,45);
	uint32_t delayCounter = 10000000;
			while(delayCounter!=0){
				 delayCounter--;
			 }
			 delayCounter = 10000000;
	while(1){
		 
		 
		 if((GPIO_PORTF_DATA_R&0x10) == 0){
			 return sel;
			
		 }
		 if((GPIO_PORTF_DATA_R&0x1) == 0){
			 
			 sel = (sel+1)%4;
			 if(sel == 0){
				 ST7735_DrawBitmap(15,75,chars_red[0],45,45);
				 ST7735_DrawBitmap(70,130,chars[3],45,45);
			 }
			 else if(sel ==1){
				 ST7735_DrawBitmap(70,75,chars_red[1],45,45);
				 ST7735_DrawBitmap(15,75,chars[0],45,45);
			 }
			 else if(sel==2){
				 ST7735_DrawBitmap(15,130,chars_red[2],45,45);
				 ST7735_DrawBitmap(70,75,chars[1],45,45);
			 }
			 else if(sel==3){
				 ST7735_DrawBitmap(70,130,chars_red[3],45,45);
				 ST7735_DrawBitmap(15,130,chars[2],45,45);
			 }
			 while(delayCounter!=0){
				 delayCounter--;
			 }
			 delayCounter = 10000000;
		 }
		 
	}

}

const unsigned short * getCharacter(uint8_t sel){
		return Cowboy4_jumpwithoutgun_0_red;
}