#ifndef GAMESTATEMAINMENU_H
#define GAMESTATEMAINMENU_H

#include "menu.h"

#include <SDL2/SDL.h>

void mainMenuStart();
void mainMenuExit();
int runGameStateMainMenu(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);

#endif
