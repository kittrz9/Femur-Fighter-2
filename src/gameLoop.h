#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <SDL2/SDL.h>

//enum gameStates {RUNNING, PAUSED};
//gameStates gameState;

extern int (*gameState)(SDL_Window*, SDL_Renderer*, float);

int runGameStateRunning(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);
int runGameStatePaused(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);

int gameLoop(SDL_Window* screen, SDL_Renderer* renderer);

#endif
