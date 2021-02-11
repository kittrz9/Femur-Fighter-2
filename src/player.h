#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "entity.h"
#include "controls.h"

void initializePlayer(struct entity* ent);

void drawPlayer(struct entity* ent, SDL_Renderer* renderer);

void updatePlayerOnGround(struct entity* ent, double deltaTime);
void updatePlayerInAir(struct entity* ent, double deltaTime);
void updatePlayerDashing(struct entity* ent, double deltaTime);
void updatePlayerKnockback(struct entity* ent, double deltaTime);

#endif
