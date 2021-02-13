#include "gameStateRunning.h"
#include "gameStatePaused.h"

#include "../controls.h"
#include "../gameStates.h"
#include "../entity.h"

int runGameStateRunning(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
	// Floor rectangle
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 4*HEIGHT/5;
	rect.w = WIDTH;
	rect.h = HEIGHT/5;
	
	if(keys[EXIT].held) {
		return 1;
	}
	
	if(keys[PAUSE].pressedTimer > 0.1) {
		gameState = runGameStatePaused;
	}
	
	// Clear the screen/renderer
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	
	// Draw the floor
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
	
	// Entity stuff
	for(entListCurrent = entListHead; entListCurrent != NULL; entListCurrent = entListCurrent->next){
		// Call the entity's draw function
		(*entListCurrent->ent->draw)(entListCurrent->ent, renderer);
		// Call the entity's update function
		(*entListCurrent->ent->update)(entListCurrent->ent, deltaTime);
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
