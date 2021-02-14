#include "gameStateControlsMenu.h"

#include "gameStatePaused.h"

#include "menu.h"
#include "../controls.h"
#include "../gameStates.h"
#include "../text.h"
#include "../config.h"

// enum CONTROLS{UP, DOWN, LEFT, RIGHT, DASH_LEFT, DASH_RIGHT, UP2, DOWN2, LEFT2, RIGHT2, DASH_LEFT2, DASH_RIGHT2, EXIT, PAUSE, MENU_CONFIRM, CONTROLS_LENGTH};

void changeControl();
void controlsMenuBack(){
	gameState = runGameStateMainMenu;
	menuIndex = 0;
	saveConfigFile();
	return;
}

struct menuItem controlsMenu[] = {
	{"UP: ", changeControl, WIDTH/2, 1*HEIGHT/17, 200, 50},
	{"DOWN: ", changeControl, WIDTH/2, 2*HEIGHT/17, 300, 50},
	{"LEFT: ", changeControl, WIDTH/2, 3*HEIGHT/17, 300, 50},
	{"RIGHT: ", changeControl, WIDTH/2, 4*HEIGHT/17, 350, 50},
	{"DASH LEFT: ", changeControl, WIDTH/2, 5*HEIGHT/17, 450, 50},
	{"DASH RIGHT: ", changeControl, WIDTH/2, 6*HEIGHT/17, 450, 50},
	{"UP 2: ", changeControl, WIDTH/2, 7*HEIGHT/17, 300, 50},
	{"DOWN 2: ", changeControl, WIDTH/2, 8*HEIGHT/17, 350, 50},
	{"LEFT 2: ", changeControl, WIDTH/2, 9*HEIGHT/17, 350, 50},
	{"RIGHT 2: ", changeControl, WIDTH/2, 10*HEIGHT/17, 400, 50},
	{"DASH LEFT 2: ", changeControl, WIDTH/2, 11*HEIGHT/17, 500, 50},
	{"DASH RIGHT 2: ", changeControl, WIDTH/2, 12*HEIGHT/17, 500, 50},
	{"EXIT: ", changeControl, WIDTH/2, 13*HEIGHT/17, 300, 50},
	{"PAUSE: ", changeControl, WIDTH/2, 14*HEIGHT/17, 325, 50},
	{"MENU CONFIRM: ", changeControl, WIDTH/2, 15*HEIGHT/17, 500, 50},
	{"BACK", controlsMenuBack, WIDTH/2, 16*HEIGHT/17, 150, 50},
};

// There's probably a much better way of doing this
int runGameStateChangingControl(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
	// Clear the screen/renderer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	
	for(int i = 0; i < sizeof(controlsMenu)/sizeof(struct menuItem); i++){
		sprintf(formatStr, "%s%s", (i == menuIndex ? ">" : ""), controlsMenu[i].str);
		drawTextCentered(renderer, formatStr, SDL_Color_White, controlsMenu[i].pos.x, controlsMenu[i].pos.y, controlsMenu[i].size.x, controlsMenu[i].size.y);
	}
	
	SDL_Event event;
	// Event handling
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_KEYDOWN:
				printf("%i %s\n", menuIndex, SDL_GetKeyName(event.key.keysym.sym));
				keys[menuIndex].keycode = event.key.keysym.sym;
				gameState = runGameStateControlsMenu;
				/*for(int i = 0; i < CONTROLS_LENGTH; i++){
					if(event.key.keysym.sym == keys[i].keycode){
						keys[i].held = true;
						keys[i].pressedTimer = 0.1;
					}
				}*/
				break;

			default:break;
		}
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

void changeControl(){
	for(int i = 0; i < CONTROLS_LENGTH; i++){
		if(keys[i].pressedTimer > 0.0) {
			keys[i].pressedTimer = 0.0;
		}
	}
	gameState = runGameStateChangingControl;
}

int runGameStateControlsMenu(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
	// Clear the screen/renderer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	
	for(int i = 0; i < sizeof(controlsMenu)/sizeof(struct menuItem); i++){
		sprintf(formatStr, "%s%s%s", (i == menuIndex ? ">" : ""), controlsMenu[i].str, (strcmp(controlsMenu[i].str, "BACK") == 0 ? "" : SDL_GetKeyName(keys[i].keycode)));
		drawTextCentered(renderer, formatStr, SDL_Color_White, controlsMenu[i].pos.x, controlsMenu[i].pos.y, controlsMenu[i].size.x, controlsMenu[i].size.y);
	}
	
	if(keys[UP].pressedTimer > 0.1 && menuIndex > 0){
		menuIndex--;
	}
	if(keys[DOWN].pressedTimer > 0.1 && menuIndex < sizeof(controlsMenu)/sizeof(struct menuItem)-1){
		menuIndex++;
	}
	if(keys[MENU_CONFIRM].pressedTimer > 0.1){
		(*controlsMenu[menuIndex].func)();
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
