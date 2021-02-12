#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <SDL2/SDL.h>

#define WIDTH 1600
#define HEIGHT 900

extern int (*gameState)(SDL_Window*, SDL_Renderer*, float);

int runGameStateMainMenu(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);
int runGameStateRunning(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);
int runGameStatePaused(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);
int runGameStateGameOver(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);

extern float gameOverTimer;

int gameLoop(SDL_Window* screen, SDL_Renderer* renderer);

#endif
