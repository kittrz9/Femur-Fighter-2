#ifndef GAMESTATEPAUSED_H
#define GAMESTATEPAUSED_H

#include <SDL2/SDL.h>

void pauseMenuToMainMenu();

int runGameStatePaused(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);

#endif
