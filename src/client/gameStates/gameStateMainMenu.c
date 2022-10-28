#include "gameStateMainMenu.h"

#include <errno.h>

#include "gameStateRunning.h"
#include "gameStateControlsMenu.h"

#include "../menu.h"
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
	{"START", mainMenuStart, WIDTH/2, 5*HEIGHT/8, 2.0f},
	{"CONTROLS", mainMenuToControlsMenu, WIDTH/2, 6*HEIGHT/8, 2.0f},
	{"EXIT", mainMenuExit, WIDTH/2, 7*HEIGHT/8, 2.0f}
};
int runGameStateMainMenu(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
	
	// Clear the screen/renderer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	
	drawTextCentered(renderer, "FEMUR FIGHTER 2", SDL_Color_White, WIDTH/2, HEIGHT/3, 4.0f);
	drawMenu(renderer, mainMenu, sizeof(mainMenu)/sizeof(struct menuItem));
	
	
	if(keys[UP].pressedTimer > 0.1){
		if(menuIndex <= 0) {
			menuIndex = sizeof(mainMenu)/sizeof(struct menuItem) - 1;
		} else {
			menuIndex--;
		}
	}
	if(keys[DOWN].pressedTimer > 0.1){
		menuIndex++;
		if(menuIndex > sizeof(mainMenu)/sizeof(struct menuItem) - 1){
			menuIndex = 0;
		}
	}
	if(keys[MENU_CONFIRM].pressedTimer > 0.1){
		(*mainMenu[menuIndex].func)();
	}
	
	// Render everything to the screen
	SDL_RenderPresent(renderer);
	return 0;
}
