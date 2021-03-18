#ifndef MENU_H
#define MENU_H

#include "types.h"

#include <SDL2/SDL.h>

extern unsigned int menuIndex; // Index to what menu item is pointed to in either the main menu or pause menu

struct menuItem{
	char* str;
	void (*func)();
// 	vec2f pos, size;
	vec2f pos;
	float scaling;
};

void drawMenu(SDL_Renderer* renderer, struct menuItem* menu, size_t menuLength);

#endif
