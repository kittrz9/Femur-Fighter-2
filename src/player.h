#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "entity.h"
#include "controls.h"

struct playerStruct{
	struct entity* ent; // For the entity related stuff like position
	
	int jumpCounter;
	bool facingLeft; // Lmao this is a stupid way to draw flippped characters
	int playerNumber;
	int health;
	float dashTimer;
	float dashCooldown;
	float knockbackTimer;
};

extern struct entity* player1;
extern struct entity* player2;

struct entity* createPlayer(SDL_Renderer* renderer, char* texturePath);
void initializePlayer(struct entity* ent);

void drawPlayer(struct entity* ent, SDL_Renderer* renderer);

void updatePlayerOnGround(struct entity* ent, double deltaTime);
void updatePlayerInAir(struct entity* ent, double deltaTime);
void updatePlayerDashing(struct entity* ent, double deltaTime);
void updatePlayerKnockback(struct entity* ent, double deltaTime);
void updatePlayerDead(struct entity* ent, double deltaTime);

void givePlayerKnockback(struct entity* ent, float force);

#endif
