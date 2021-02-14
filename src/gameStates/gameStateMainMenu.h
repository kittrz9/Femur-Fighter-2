#ifndef GAMESTATEMAINMENU_H
#define GAMESTATEMAINMENU_H

#include <SDL2/SDL.h>

void mainMenuStart();
void mainMenuExit();
int runGameStateMainMenu(SDL_Window* screen, SDL_Renderer* renderer, float deltaTime);

#endif
