#ifndef MENU_H_
#define MENU_H_
#include <stdint.h>

typedef struct menu_struct menu_t;

#define MENU_LENGHT 5
struct menu_struct {
	//const uint8_t back;
	const char * name;
	menu_t * next;
	menu_t * prev;
	menu_t * child;
	menu_t * parent;
	void (*menu_function)(void);

};



void menu_refresh(void);
void menu_next(void);
void menu_prev(void);
void menu_enter(void);
void refresh_taskbar(void);
void create_structarray(void);



#endif /* MENU_H_ */
