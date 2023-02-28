#include <string.h>



#include "menu.h"
#include "menu_callbacks.h"
#include <stdio.h>
#include "LCD.h"



menu_t menu1;
menu_t menu2;
menu_t menu3;
menu_t menu4;
menu_t menu5;


// definition of menu's components: (*name, *next, *prev, *child, *parent, (*menu_function))

menu_t menu1 = { "CLOSE ALL", &menu2, NULL, NULL, NULL, turn_ALL_OFF };
menu_t menu2 = { "SD CARD", &menu3, &menu1, NULL, NULL,turn_SD};
menu_t menu3 = { "BLUETOOTH",&menu4, &menu2, NULL, NULL,turn_BL };
menu_t menu4 = { "CLEAR SD", &menu5, &menu3, NULL, NULL,clearSD };
menu_t menu5 ={ "RESET", NULL, &menu4, NULL, NULL, reset};

 menu_t *currentPointer = &menu1;
 uint8_t CurrentPointerPosition=1;
 menu_t * structarray[MENU_LENGHT];
 uint8_t stringPosY[5]={20,40,60,80,100};
 uint8_t stringPosX[5]={5,5,5,5,5};
 extern uint8_t BLstatus;
 extern uint8_t SDstatus;
 extern uint8_t RSTstatus;





void  refresh_taskbar(){
	LCD_SetArealColor(0, 0, 160, 19, BLACK);
	//BLUETOOTH TASKBAR
	//LCD_DrawCircle(151, 9, 6, BLUE, DRAW_EMPTY,  DOT_PIXEL_DFT );
	LCD_DisplayString(120,2,"BL",&Font16,LCD_BACKGROUND,GRAY);

	//SD TASKBAR
	//LCD_DrawCircle(111, 9, 6, BLUE, DRAW_EMPTY,  DOT_PIXEL_DFT );
	LCD_DisplayString(80,2,"SD",&Font16,LCD_BACKGROUND,GRAY);

	//DEBUG TASKBAR
	//LCD_DrawCircle(71, 9, 6, BLUE, DRAW_EMPTY,  DOT_PIXEL_DFT );

	//BLUETOOTH STATUS TASKBAR
	if(BLstatus)
		LCD_DrawCircle(150, 9, 6, GREEN, DRAW_FULL, DOT_PIXEL_DFT);
	else
		LCD_DrawCircle(150, 9, 6, RED, DRAW_FULL, DOT_PIXEL_DFT);
	//SD STATUS TASKBAR
	if(SDstatus)
		LCD_DrawCircle(110, 9, 6, GREEN, DRAW_FULL, DOT_PIXEL_DFT);
	else
		LCD_DrawCircle(110, 9, 6, RED, DRAW_FULL, DOT_PIXEL_DFT);
	//DEBUG STATUS TASKBAR
	if(RSTstatus)
		LCD_DisplayString(1,2,"RESETSD",&Font16,LCD_BACKGROUND,0xFCE0);


}


void create_structarray(){
	menu_t * temp =currentPointer;
	while(temp->prev){
		temp=temp->prev;
	}
	structarray[0]=temp;
	for(uint8_t i=1;i<MENU_LENGHT;i++){
		if(temp->next){
			temp=temp->next;
			structarray[i]=temp;
		}
		else{
			structarray[i]=NULL;
		}
	}
}



void menu_refresh(){
	//LCD_Clear(GRAY);
	refresh_taskbar();
	uint16_t colors[5];
	for(uint8_t i =0;i<MENU_LENGHT;i++){
		if(i==CurrentPointerPosition-1){
			colors[i]=RED;
		}
		else{
			colors[i]=BLACK;
		}
	}
	for(uint8_t i=0;i<MENU_LENGHT;i++){
		if(structarray[i])
			LCD_DisplayString(stringPosX[i],stringPosY[i],structarray[i]->name,&Font24,LCD_BACKGROUND,colors[i]);
	}

}

void menu_next() {
	//create_structarray();
	if(currentPointer->next){

		currentPointer=currentPointer->next;
		CurrentPointerPosition++;
	}
	menu_refresh();

}

void menu_prev(void) {
	//create_structarray();
	if(currentPointer->prev){
		currentPointer=currentPointer->prev;
		CurrentPointerPosition--;
	}
	menu_refresh();
}

void menu_enter(void) {
	//create_structarray();
	if(!currentPointer->menu_function && currentPointer->child)
	{
		currentPointer=currentPointer->child;
		menu_refresh();

	}
	else if(currentPointer->menu_function)
	{
		currentPointer->menu_function();
	}
	refresh_taskbar();

}



