#include "gameStates.h"

// This has to be here because of weird extern stuff
int (*gameState)(SDL_Window*, SDL_Renderer*, float) = runGameStateMainMenu;

bool running = true;
