#include <SDL2/SDL.h>
#include <stdbool.h>

#include "gameLoop.h"
#include "entity.h"
#include "controls.h"
#include "player.h"

#include "text.h"

// This has to be here because of weird extern stuff
int (*gameState)(SDL_Window*, SDL_Renderer*, float) = runGameStateRunning;

float gameOverTimer;

int gameLoop(SDL_Window* screen, SDL_Renderer* renderer) {
	bool running = true;
	SDL_Event event;
	unsigned int lastTime = 0, currentTime = SDL_GetPerformanceCounter();
	double deltaTime = 0;
    gameOverTimer = 0;

	initControls();
    
	// Create entities
	struct entity* player1 = malloc(sizeof(struct entity));
    initializePlayer(player1);
    player1->playerNumber = 0;
    
    SDL_Surface* temp = NULL; // Temporary
    temp = SDL_LoadBMP("./res/sansGriffin.bmp");
    player1->texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    
	pushToEntityList(player1);
    
    struct entity* player2 = malloc(sizeof(struct entity));
    initializePlayer(player2);
	player2->pos.x = (3*WIDTH/4) - (player2->size.x / 2);
    player2->playerNumber = 1;
    
    temp = SDL_LoadBMP("./res/skeleman.bmp");
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
		//if(runGameStateRunning(screen, renderer, deltaTime)) {running = false;}
        if((*gameState)(screen, renderer, deltaTime)) {running = false;}

		// Deltatime stuff
		lastTime = currentTime;
		currentTime = SDL_GetPerformanceCounter();

        // Deltatime is in milliseconds, not seconds
		deltaTime = (double)((currentTime - lastTime)*1000 / (double)SDL_GetPerformanceFrequency());
        
        printf("%f\n", deltaTime);
	}
	removeFromEntityList(player1);
    removeFromEntityList(player2);

	return 0;
}


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
    
    //SDL_Color SDL_Color_White = {255, 255, 255}; // This is dumb
    drawTextCentered(renderer, "bruh", SDL_Color_White, WIDTH/2, HEIGHT/2, 80, 40);
    
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

int runGameStateGameOver(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
    // Floor rectangle
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 4*HEIGHT/5;
    rect.w = WIDTH;
    rect.h = HEIGHT/5;
    
    if(keys[EXIT].held) {
        return 1;
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
        (*entListCurrent->ent->update)(entListCurrent->ent, deltaTime/4);
    }
    
    // Decrement the pressed timer for each key if they're being pressed
    for(int i = 0; i < CONTROLS_LENGTH; i++){
        if(keys[i].pressedTimer > 0.0) {
            keys[i].pressedTimer -= deltaTime;
        }
    }

    gameOverTimer -= deltaTime * 0.001;
    
    if(gameOverTimer <= 0){
        gameState = runGameStateRunning;
        for(entListCurrent = entListHead; entListCurrent != NULL; entListCurrent = entListCurrent->next){
            initializePlayer(entListCurrent->ent);
        }
    }
    
    // Render everything to the screen
    SDL_RenderPresent(renderer);
    
    return 0;
}
