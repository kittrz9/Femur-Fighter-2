#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
	struct entity* player1 = malloc(sizeof(struct entity));
	player1->playerNumber = 0;
	
	SDL_Surface* temp = NULL; // Temporary
	temp = IMG_Load("res/sansGriffin.png");
	player1->texture = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	
	pushToEntityList(player1);
	
	struct entity* player2 = malloc(sizeof(struct entity));
	player2->playerNumber = 1;
	
	temp = IMG_Load("./res/skeleman.bmp");
	player2->texture = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	
	pushToEntityList(player2);
	
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
	
		// Deltatime stuff
		lastTime = currentTime;
		currentTime = SDL_GetPerformanceCounter();
	
		// Deltatime is in milliseconds, not seconds
		deltaTime = (double)((currentTime - lastTime)*1000 / (double)SDL_GetPerformanceFrequency());
	}
	removeFromEntityList(player1);
	removeFromEntityList(player2);
	
	return 0;
}
