#include "gameStateMainMenu.h"

#include "gameStateRunning.h"
#include "gameStateControlsMenu.h"

#include "menu.h"
#include "../controls.h"
#include "../gameStates.h"
#include "../entity.h"
#include "../player.h"
#include "../text.h"

void mainMenuStart(){
	gameState = runGameStateRunning;
	menuIndex = 0;
	for(entListCurrent = entListHead; entListCurrent != NULL; entListCurrent = entListCurrent->next){
		initializePlayer(entListCurrent->ent);
	}
	return;
}
// This is VERY stupid
void mainMenuExit(){
	running = false;
	return;
}

void mainMenuToControlsMenu(){
	gameState = runGameStateControlsMenu;
	menuIndex = 0;
	for(entListCurrent = entListHead; entListCurrent != NULL; entListCurrent = entListCurrent->next){
		initializePlayer(entListCurrent->ent);
	}
	return;
}

struct menuItem mainMenu[] = {
	{"START", mainMenuStart, WIDTH/2, 5*HEIGHT/8, 200, 100},
	{"CONTROLS", mainMenuToControlsMenu, WIDTH/2, 6*HEIGHT/8, 250, 100},
	{"EXIT", mainMenuExit, WIDTH/2, 7*HEIGHT/8, 175, 100}
};
int runGameStateMainMenu(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
	
	// Clear the screen/renderer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	
	drawTextCentered(renderer, "FEMUR FIGHTER 2", SDL_Color_White, WIDTH/2, HEIGHT/3, 400,150);
	for(int i = 0; i < sizeof(mainMenu)/sizeof(struct menuItem); i++){
		sprintf(formatStr, "%s%s", (i == menuIndex ? ">" : ""), mainMenu[i].str);
		drawTextCentered(renderer, formatStr, SDL_Color_White, mainMenu[i].pos.x, mainMenu[i].pos.y, mainMenu[i].size.x, mainMenu[i].size.y);
	}
	
	if(keys[UP].pressedTimer > 0.1 && menuIndex > 0){
		menuIndex--;
	}
	if(keys[DOWN].pressedTimer > 0.1 && menuIndex < sizeof(mainMenu)/sizeof(struct menuItem)-1){
		menuIndex++;
	}
	if(keys[MENU_CONFIRM].pressedTimer > 0.1){
		(*mainMenu[menuIndex].func)();
	}
	
	// Decrement the pressed timer for each key if they're being pressed
	for(int i = 0; i < CONTROLS_LENGTH; i++){
		if(keys[i].pressedTimer > 0.0) {
			keys[i].pressedTimer -= deltaTime;
		}
	}
	
	// Render everything to the screen
	SDL_RenderPresent(renderer);
	return 0;
}
