#ifndef GAMESTATES_H
#define GAMESTATES_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "connection.h"

extern int (*gameState)(SDL_Window*, SDL_Renderer*, float);
extern bool running;

extern connection connectedServer; // probably should be in a different header but I can't think of where it should be right now

#include "gameStates/gameStateMainMenu.h"
#include "gameStates/gameStateRunning.h"
#include "gameStates/gameStatePaused.h"
#include "gameStates/gameStateGameOver.h"
#include "gameStates/gameStatePaused.h"

#endif
