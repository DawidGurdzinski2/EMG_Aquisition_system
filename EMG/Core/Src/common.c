#include "common.h"
#include "menu.h"

void (*key_next_func)(void) = &menu_next;
void (*key_prev_func)(void) = &menu_prev;
void (*key_enter_func)(void) = &menu_enter;




void key_next_press(void){
	if(key_next_func)(*key_next_func)();
}

void key_prev_press(void){
if(key_prev_func) (*key_prev_func)();
}



void key_enter_press(void){
	if(key_enter_func) (*key_enter_func)();
}


