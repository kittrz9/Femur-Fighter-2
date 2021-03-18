#include <SDL2/SDL.h>
#include <stdbool.h>

#include "gameLoop.h"
#include "entity.h"
#include "controls.h"
#include "player.h"

#include "text.h"

#include "gameStates.h"

int gameLoop(SDL_Window* screen, SDL_Renderer* renderer) {
	SDL_Event event;
	unsigned int lastTime = 0, currentTime = SDL_GetPerformanceCounter();
	double deltaTime = 0;
	gameOverTimer = 0;

	initControls();
	
	// Create entities
	struct entity* player1 = createPlayer(renderer, "res/sansGriffin.png");
	struct entity* player2 = createPlayer(renderer, "res/skeleman.png");
	
	while(running){
		// Event handling
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					for(int i = 0; i < CONTROLS_LENGTH; i++){
						if(event.key.keysym.sym == keys[i].keycode){
							keys[i].held = true;
							keys[i].pressedTimer = 0.1;
						}
					}
					break;
	
				case SDL_KEYUP:
					for(int i = 0; i < CONTROLS_LENGTH; i++){
						if(event.key.keysym.sym == keys[i].keycode){
							keys[i].held = false;
							keys[i].pressedTimer = 0.0;
						}
					}
					break;
	
				case SDL_QUIT:
					running = false;
					break;
	
				default:break;
			}
		}
		// Do stuff
		if((*gameState)(screen, renderer, deltaTime)) {running = false;}
	
		// Decrement the pressed timer for each key if they're being pressed
		for(int i = 0; i < CONTROLS_LENGTH; i++){
			if(keys[i].pressedTimer > 0.0) {
				keys[i].pressedTimer -= deltaTime;
			}
		}
	
		// Deltatime stuff
		lastTime = currentTime;
		currentTime = SDL_GetPerformanceCounter();
	
		// Deltatime is in milliseconds, not seconds
		deltaTime = (double)((currentTime - lastTime)*1000 / (double)SDL_GetPerformanceFrequency());
		
		printf("delta time: %f\r", deltaTime);
	}
	removeEntityList();
	
	return 0;
}
