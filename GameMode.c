#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "BackgroundSprites.h"

uint8_t GameMode(){
	ST7735_FillScreen(0x0000);            // set screen to black
	ST7735_DrawBitmap(0,158,GameModeBg,128,160);
	ST7735_SetCursor(7, 2);
	//ST7735_OutString("1 Player");
	ST7735_SetCursor(7, 3);
	//ST7735_OutString("2 Player");
	uint8_t cs = 0;
	
	uint32_t delayCounter = 10000000;
			while(delayCounter!=0){
				 delayCounter--;
			 }
			 delayCounter = 10000000;
	ST7735_DrawFastHLine(25,(cs+1)*19+47,80,0x00);
	while(1){
			
		 	
		 //ST7735_DrawFastHLine(30,(cs+1)*11+5,70,0x001f);
		 if((GPIO_PORTF_DATA_R&0x10) == 0){
			 //select
			return cs;
		 }
		 if((GPIO_PORTF_DATA_R&0x1) == 0){
			 //ST7735_DrawFastHLine(30,(cs+3)*15+5,70,0x0);
			 ST7735_DrawBitmap(0,158,GameModeBg,128,160);
			 cs = (cs+1)%3;
			 ST7735_DrawFastHLine(25,(cs+1)*19+47,80,0x000);
			 while(delayCounter!=0){
				 delayCounter--;
			 }
			 delayCounter = 10000000;
		 }
		 
	}
}
