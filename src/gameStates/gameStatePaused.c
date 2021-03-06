#include "gameStatePaused.h"

#include "../gameStates.h"
#include "../menu.h"
#include "../text.h"
#include "../controls.h"
#include "../entity.h"

void pauseMenuResume(){
	gameState = runGameStateRunning;
	return;
}
void pauseMenuToMainMenu(){
	gameState = runGameStateMainMenu;
	menuIndex = 0;
	return;
}
struct menuItem pauseMenu[] = {
	{"RESUME", pauseMenuResume, WIDTH/2, 6*HEIGHT/10, 200, 50},
	{"MAIN MENU", pauseMenuToMainMenu, WIDTH/2, 7*HEIGHT/10, 300, 50},
	{"EXIT", mainMenuExit, WIDTH/2, 8*HEIGHT/10, 175, 50}
};

int runGameStatePaused(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
	SDL_Rect rect;
	
	if(keys[EXIT].held) {
		return 1;
	}
	
	if(keys[PAUSE].pressedTimer > 0.1) {
		gameState = runGameStateRunning;
	}
	
	// Clear the screen/renderer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	
	if(keys[UP].pressedTimer > 0.1 && menuIndex > 0){
		menuIndex--;
	}
	if(keys[DOWN].pressedTimer > 0.1 && menuIndex < sizeof(pauseMenu)/sizeof(struct menuItem)-1){
		menuIndex++;
	}
	if(keys[MENU_CONFIRM].pressedTimer > 0.1){
		(*pauseMenu[menuIndex].func)();
	}
	
	// Draw the floor
	rect.x = 0;
	rect.y = 4*HEIGHT/5;
	rect.w = WIDTH;
	rect.h = HEIGHT/5;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
	
	// Entity stuff
	for(entListCurrent = entListHead; entListCurrent != NULL; entListCurrent = entListCurrent->next){
		// Call the entity's draw function
		(*entListCurrent->ent->draw)(entListCurrent->ent, renderer);
	}
	
	// Draw the pause screen
	rect.x = 0;
	rect.y = 0;
	rect.w = WIDTH;
	rect.h = HEIGHT;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
	SDL_RenderFillRect(renderer, &rect);
	for(int i = 0; i < sizeof(pauseMenu)/sizeof(struct menuItem); i++){
		sprintf(formatStr, "%s%s", (i == menuIndex ? ">" : ""), pauseMenu[i].str);
		drawTextCentered(renderer, formatStr, SDL_Color_White, pauseMenu[i].pos.x, pauseMenu[i].pos.y, pauseMenu[i].size.x, pauseMenu[i].size.y);
	}
	
	//SDL_Color SDL_Color_White = {255, 255, 255}; // This is dumb
	drawTextCentered(renderer, "bruh", SDL_Color_White, WIDTH/2, HEIGHT/2, 80, 40);
	
	// Render everything to the screen
	SDL_RenderPresent(renderer);
	
	return 0;
} 
