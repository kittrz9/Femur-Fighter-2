#include "gameStateControlsMenu.h"

#include "gameStatePaused.h"

#include "../menu.h"
#include "../controls.h"
#include "../gameStates.h"
#include "../text.h"
#include "../config.h"

void changeControl();
void controlsMenuBack(){
	gameState = runGameStateMainMenu;
	menuIndex = 0;
	saveConfigFile();
	return;
}

struct menuItem controlsMenu[] = {
	{"UP: ", changeControl, WIDTH/2, 1*HEIGHT/17, 2.0f},
	{"DOWN: ", changeControl, WIDTH/2, 2*HEIGHT/17, 2.0f},
	{"LEFT: ", changeControl, WIDTH/2, 3*HEIGHT/17, 2.0f},
	{"RIGHT: ", changeControl, WIDTH/2, 4*HEIGHT/17, 2.0f},
	{"DASH LEFT: ", changeControl, WIDTH/2, 5*HEIGHT/17, 2.0f},
	{"DASH RIGHT: ", changeControl, WIDTH/2, 6*HEIGHT/17, 2.0f},
	{"UP 2: ", changeControl, WIDTH/2, 7*HEIGHT/17, 2.0f},
	{"DOWN 2: ", changeControl, WIDTH/2, 8*HEIGHT/17, 2.0f},
	{"LEFT 2: ", changeControl, WIDTH/2, 9*HEIGHT/17, 2.0f},
	{"RIGHT 2: ", changeControl, WIDTH/2, 10*HEIGHT/17, 2.0f},
	{"DASH LEFT 2: ", changeControl, WIDTH/2, 11*HEIGHT/17, 2.0f},
	{"DASH RIGHT 2: ", changeControl, WIDTH/2, 12*HEIGHT/17, 2.0f},
	{"EXIT: ", changeControl, WIDTH/2, 13*HEIGHT/17, 2.0f},
	{"PAUSE: ", changeControl, WIDTH/2, 14*HEIGHT/17, 2.0f},
	{"MENU CONFIRM: ", changeControl, WIDTH/2, 15*HEIGHT/17, 2.0f},
	{"BACK", controlsMenuBack, WIDTH/2, 16*HEIGHT/17, 2.0f},
};

// There's probably a much better way of doing this
int runGameStateChangingControl(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
	// Clear the screen/renderer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	
	//drawMenu(renderer, controlsMenu, sizeof(controlsMenu)/sizeof(struct menuItem));
	
	// Have to do this instead of using drawMenu to have the key at the end of the string
	// There's probably a better solution to this but I'm very stupid
	for(int i = 0; i < sizeof(controlsMenu)/sizeof(struct menuItem); i++){
		sprintf(formatStr, "%s%s", controlsMenu[i].str, (strcmp(controlsMenu[i].str, "BACK") == 0 || i == menuIndex ? "" : SDL_GetKeyName(keys[i].keycode)));
		drawTextCentered(renderer, formatStr, (i == menuIndex ? SDL_Color_Red : SDL_Color_White), controlsMenu[i].pos.x, controlsMenu[i].pos.y, controlsMenu[i].scaling);
	}
	
	SDL_Event event;
	// Event handling
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_KEYDOWN:
				printf("%i %s\n", menuIndex, SDL_GetKeyName(event.key.keysym.sym));
				keys[menuIndex].keycode = event.key.keysym.sym;
				gameState = runGameStateControlsMenu;
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
	
 	//drawMenu(renderer, controlsMenu, sizeof(controlsMenu)/sizeof(struct menuItem));
	// Have to do this instead of using drawMenu to have the key at the end of the string
	// There's probably a better solution to this but I'm very stupid
	for(int i = 0; i < sizeof(controlsMenu)/sizeof(struct menuItem); i++){
		sprintf(formatStr, "%s%s", controlsMenu[i].str, (strcmp(controlsMenu[i].str, "BACK") == 0 ? "" : SDL_GetKeyName(keys[i].keycode)));
		drawTextCentered(renderer, formatStr, (i == menuIndex ? SDL_Color_Red : SDL_Color_White), controlsMenu[i].pos.x, controlsMenu[i].pos.y, controlsMenu[i].scaling);
	}
	
	if(keys[UP].pressedTimer > 0.1){
		if(menuIndex <= 0) {
			menuIndex = sizeof(controlsMenu)/sizeof(struct menuItem) - 1;
		} else {
			menuIndex--;
		}
	}
	if(keys[DOWN].pressedTimer > 0.1){
		menuIndex++;
		if(menuIndex > sizeof(controlsMenu)/sizeof(struct menuItem) - 1){
			menuIndex = 0;
		}
	}
	if(keys[MENU_CONFIRM].pressedTimer > 0.1){
		(*controlsMenu[menuIndex].func)();
	}
	
	// Render everything to the screen
	SDL_RenderPresent(renderer);
	return 0;
}
