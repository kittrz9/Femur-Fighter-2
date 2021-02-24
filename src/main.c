#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <stdio.h>

#include "gameLoop.h"
#include "types.h"
#include "text.h"
#include "config.h"

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	
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
	
	// Get the config file loaded
	getConfigFile();
	printf("config at %s\n", configPath);
	
	// Main loop
	gameLoop(screen, renderer);
	
	// Free resources and end
	// Quit SDL_ttf
	TTF_CloseFont(font);
	TTF_Quit();
	// Quit SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);
	SDL_Quit();
	return 0;
}
