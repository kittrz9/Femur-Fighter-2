#ifndef MENU_H
#define MENU_H

#include "types.h"

extern unsigned int menuIndex; // Index to what menu item is pointed to in either the main menu or pause menu

struct menuItem{
	char* str;
	void (*func)();
	vec2f pos, size;
};

#endif
