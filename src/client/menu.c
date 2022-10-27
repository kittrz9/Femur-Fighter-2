#include "menu.h"

#include <stddef.h> // Needed for size_t

#include "text.h"

// Needed because extern is weird
unsigned int menuIndex = 0;

void drawMenu(SDL_Renderer* renderer, struct menuItem* menu, size_t menuLength){
	for(int i = 0; i < menuLength; i++){
// 		sprintf(formatStr, "%s%s", (i == menuIndex ? ">" : ""), controlsMenu[i].str);
		drawTextCentered(renderer, menu[i].str, (i == menuIndex ? SDL_Color_Red : SDL_Color_White), menu[i].pos.x, menu[i].pos.y, menu[i].scaling);
	}
}
