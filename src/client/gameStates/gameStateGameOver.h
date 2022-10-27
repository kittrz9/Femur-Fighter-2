#ifndef GAMESTATEGAMEOVER_H
#define GAMESTATEGAMEOVER_H

#include <SDL2/SDL.h>

extern float gameOverTimer;

int runGameStateGameOver(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);

#endif
