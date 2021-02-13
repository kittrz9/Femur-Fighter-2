#include "gameStates.h"

int (*gameState)(SDL_Window*, SDL_Renderer*, float) = runGameStateMainMenu;

bool running = true;
