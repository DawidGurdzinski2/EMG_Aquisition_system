#include <stdio.h>
#include <string.h>
#include "common.h"
#include "menu.h"
#include "menu_callbacks.h"
#include "LCD.h"



extern uint8_t BLstatus;
extern uint8_t SDstatus;
extern uint8_t RST;
extern uint8_t clearSDflag;
extern  menu_t *currentPointer;


void clearSD(void){
	clearSDflag=1;

}

void turn_ALL_OFF(){
	BLstatus=0;
	SDstatus=0;
}

 void turn_BL(){
	 if(BLstatus)
	 	BLstatus=0;
	 else
	 	BLstatus=1;
 }
extern void turn_SD(){
	if(SDstatus)
		SDstatus=0;
	else
		SDstatus=1;
}


void reset(){
	RST=1;

}
