#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <stdio.h>

#include "gameLoop.h"

#include "text.h"

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	
	// Setting the font size to 200 is probably very dumb, but I don't think SDL_ttf has a way to change font size after it's been opened
	font = TTF_OpenFont("res/TerminusTTF-4.47.0.ttf", 24);
	
	formatStr = malloc(MAX_STR_LEN);
	
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
	
	// Window stuff
	SDL_Window* screen = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &screen, &renderer);
	if(!screen) {
		printf("Couldn't create window\n");
	}
	if(!renderer) {
		printf("Couldn't create renderer\n");
	}
	SDL_SetWindowTitle(screen, "bruh");
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	// Main loop
	gameLoop(screen, renderer);
	
	// End
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);
	SDL_Quit();
	return 0;
}
