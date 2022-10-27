#include "gameStateRunning.h"
#include "gameStatePaused.h"

#include <errno.h>

#include "../controls.h"
#include "../gameStates.h"
#include "../entity.h"

const float netTimerInterval = 1000.0f/60.0f; // 60hz

int runGameStateRunning(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime){
	static float netTimer = 0;
	
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

	netTimer += deltaTime;

	if(netTimer >= netTimerInterval) {
		netTimer = 0.0f;
		// network stuff happens
		char request[] = "TEST/";
		printf("network stuff happens\n");
		if(send(connectedServer.socket, request, strlen(request), 0) != strlen(request)) {
			fprintf(stderr, "could not send request \"%s\": %i\n", request, errno);
		} else {
			printf("sent: %s\n",  request);
		}

	#define MAX_RESPONSE_LEN 4096
		char response[MAX_RESPONSE_LEN];
		if(recv(connectedServer.socket, response, MAX_RESPONSE_LEN, 0) < 0) {
			fprintf(stderr, "failed to recieve response: %i\n", errno);
		}
	}
	
	// Render everything to the screen
	SDL_RenderPresent(renderer);
	
	return 0;
} 
