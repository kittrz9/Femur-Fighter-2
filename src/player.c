#include "player.h"

#include <SDL2/SDL_image.h>
#include <stdlib.h>

#include "gameLoop.h" // Needed for WIDTH and HEIGHT for now
#include "types.h"
#include "gameStates.h"

#include "text.h"

#include <math.h>

unsigned int playerCounter = 0;

struct entity* createPlayer(SDL_Renderer* renderer, char* texturePath){
	struct entity* ent = malloc(sizeof(struct entity));
	ent->object = malloc(sizeof(struct playerStruct));
	struct playerStruct* player = (struct playerStruct*)ent->object;
	player->ent = ent;
	player->playerNumber = playerCounter;
	playerCounter++;
	
	SDL_Surface* surface = NULL;
	surface = IMG_Load(texturePath);
	ent->texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	
	pushToEntityList(ent);
	return ent;
}

void initializePlayer(struct entity* ent) {
	struct playerStruct* player = (struct playerStruct*)ent->object;
	player->ent = ent;
	ent->size.x = 150;
	ent->size.y = 200;
	ent->vel.x = 0;
	ent->vel.y = 0;
	player->health = 100;
	
	ent->draw = drawPlayer;
	ent->update = updatePlayerInAir;
	player->jumpCounter = 1;
	player->dashTimer = 0;
	player->dashCooldown = 0;
	if(player->playerNumber == 0){
		ent->pos.x = (WIDTH/4) - (ent->size.x / 2);
	} else {
		// Probably should fix this to like evenly spread each player across the screen
		ent->pos.x = ((WIDTH/4) - (ent->size.x / 2)) + (WIDTH/(2*(player->playerNumber)));
	}
	ent->pos.y = HEIGHT/2;
}
 
void drawPlayer(struct entity* ent, SDL_Renderer* renderer){
	struct playerStruct* player = (struct playerStruct*)ent->object;
	SDL_Rect rect; // Maybe like unoptimal to have this always being created every time the player needs to be drawn but idk maybe the compiler will optimize it? I also feel like it's better than when I just had the SDL_Rect in the entity's struct
	rect.x = ent->pos.x;
	rect.y = ent->pos.y;
	rect.w = ent->size.x;
	rect.h = ent->size.y;
	
	float rotation = ent->vel.x * 6;
	
	if(ent->update == updatePlayerDashing) {
		// Give player blue color while dashing
		SDL_SetTextureColorMod(ent->texture, 150, 150, 255);
		SDL_SetTextureAlphaMod(ent->texture, 200);
	} else if(ent->update == updatePlayerKnockback) {
		// Give player red color while in knockback
		SDL_SetTextureColorMod(ent->texture, 255, 150, 150);
		SDL_SetTextureAlphaMod(ent->texture, 255);
	} else if(ent->update == updatePlayerDead){
		// Give player gray color when dead
		SDL_SetTextureColorMod(ent->texture, 150, 150, 150);
		SDL_SetTextureAlphaMod(ent->texture, 255);
		
		rotation += 90;
	} else {
		// Have the player have normal colors if nothing else changes it's color (Changes it back after it's changed when in other states)
		SDL_SetTextureColorMod(ent->texture, 255, 255, 255);
		SDL_SetTextureAlphaMod(ent->texture, 255);
	}
	SDL_RenderCopyEx(renderer, ent->texture, NULL, &rect, rotation, NULL, (player->facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
	
	sprintf(formatStr, "%i%%", player->health);
	drawTextCentered(renderer, formatStr, SDL_Color_White, ent->pos.x + (ent->size.x / 2), ent->pos.y - 40, 2.0f);
}

bool playerBoundaryCheck(struct entity* ent){
	bool boundaryCollided = false;
	if(ent->pos.x < 0) {ent->pos.x = 0; boundaryCollided = true;}
	if(ent->pos.y < 0) {ent->pos.y = 0; boundaryCollided = true;}
	if(ent->pos.x > WIDTH - ent->size.x) {ent->pos.x = WIDTH - ent->size.x; boundaryCollided = true;}
	if(ent->pos.y > HEIGHT - ent->size.y) {ent->pos.y = HEIGHT - ent->size.y; boundaryCollided = true;}
	return boundaryCollided;
}

void updatePlayerOnGround(struct entity* ent, double deltaTime){
	struct playerStruct* player = (struct playerStruct*)ent->object;
	
	ent->pos.x += ent->vel.x * deltaTime;
	ent->pos.y += ent->vel.y * deltaTime;
	
	// The "+ (ent->playerNumber * UP2)" is to use the right controlls for each character since UP2 is in the middle of the controls thing
	if(keys[UP + (player->playerNumber * UP2)].pressedTimer > 0.1) {
		ent->vel.y = -2;
		ent->update = updatePlayerInAir;
	}
	
	if(keys[LEFT + (player->playerNumber * UP2)].held) {
		ent->vel.x = -1.5;
		player->facingLeft = true;
	} else if(keys[RIGHT + (player->playerNumber * UP2)].held) {
		ent->vel.x =  1.5;
		player->facingLeft = false;
	} else if(ent->vel.x > 0.1){
		ent->vel.x -= 0.01 * deltaTime;
	} else if(ent->vel.x < -0.1){
		ent->vel.x += 0.01 * deltaTime;
	} else {
		ent->vel.x = 0;
	}
	
	player->dashCooldown -= deltaTime * 0.001;
	
	if(player->dashCooldown < 0) {
		if(keys[DASH_LEFT + (player->playerNumber * UP2)].held) {
			ent->vel.x = -2.5;
			ent->update = updatePlayerDashing;
			player->dashTimer = 0.2;
		} else if(keys[DASH_RIGHT + (player->playerNumber * UP2)].held) {
			ent->vel.x =  2.5;
			ent->update = updatePlayerDashing;
			player->dashTimer = 0.2;
		}
	}
	
	playerBoundaryCheck(ent);
}

void updatePlayerInAir(struct entity* ent, double deltaTime){
	struct playerStruct* player = ent->object;
	ent->pos.x += ent->vel.x * deltaTime;
	ent->pos.y += ent->vel.y * deltaTime;
	
#define GRAVITY 0.01
	ent->vel.y += (GRAVITY * ((keys[DOWN + (player->playerNumber * UP2)].held * 2)+1)) * deltaTime;
	
	if(ent->pos.y > (4*HEIGHT/5 - ent->size.y)+10) {
		ent->pos.y = 4*HEIGHT/5 - ent->size.y;
		ent->vel.y = 0;
		player->jumpCounter = 1;
		ent->update = updatePlayerOnGround;
	}
	
	if(keys[UP + (player->playerNumber * UP2)].pressedTimer > 0.1 && player->jumpCounter > 0) {
		player->jumpCounter--;
		ent->vel.y = -2;
	}
	
	player->dashCooldown -= deltaTime * 0.001; // Multiplying by 0.001 because the units in these timers are in seconds and not milliseconds and I don't want to change them
	
	if(keys[LEFT + (player->playerNumber * UP2)].held) {
		ent->vel.x = -1.5;
		player->facingLeft = true;
	} else if(keys[RIGHT + (player->playerNumber * UP2)].held) {
		ent->vel.x =  1.5;
		player->facingLeft = false;
	} else if(ent->vel.x > 0.1){
		ent->vel.x -= 0.01 * deltaTime;
	} else if(ent->vel.x < -0.1){
		ent->vel.x += 0.01 * deltaTime;
	} else {
		ent->vel.x = 0;
	}
	
	player->dashCooldown -= deltaTime * 0.001;
	
	if(player->dashCooldown < 0) {
		if(keys[DASH_LEFT + (player->playerNumber * UP2)].held) {
			ent->vel.x = -2.5;
			ent->update = updatePlayerDashing;
			player->dashTimer = 0.2;
		} else if(keys[DASH_RIGHT + (player->playerNumber * UP2)].held) {
			ent->vel.x =  2.5;
			ent->update = updatePlayerDashing;
			player->dashTimer = 0.2;
		}
	}
	
	
	playerBoundaryCheck(ent);
}

void updatePlayerDashing(struct entity* ent, double deltaTime){
	struct playerStruct* player = ent->object;
	ent->pos.x += ent->vel.x * deltaTime;
	
	player->dashTimer -= deltaTime*0.001;
	if(player->dashTimer < 0){
		ent->vel.x /= 2;
		ent->update = updatePlayerInAir;
		player->dashCooldown = 0.6;
	}
	
	
	
	// Probably really REALLY inneficient to loop through the list like this but I don't want to like pass the other player into the update function just for this right now
	for(struct entListNode* current = entListHead; current != NULL; current = current->next){
		if(ent != current->ent && current->ent->update != updatePlayerDead && checkEntityCollision(ent, current->ent)){
			struct playerStruct* hitPlayer = (player->dashTimer < ((struct playerStruct*)current->ent->object)->dashTimer ? ent->object : current->ent->object);
			struct playerStruct* notHitPlayer = (player->dashTimer > ((struct playerStruct*)current->ent->object)->dashTimer ? ent->object : current->ent->object);
			givePlayerKnockback(notHitPlayer->ent, 0.5);
			hitPlayer->ent->vel.x = notHitPlayer->ent->vel.x;
			givePlayerKnockback(hitPlayer->ent, 0.5);
			hitPlayer->health -= 5;
			
			if(hitPlayer->health <= 0) {hitPlayer->ent->update = updatePlayerDead; gameState = runGameStateGameOver; gameOverTimer = 4.0;}
			if(notHitPlayer->health <= 0) {notHitPlayer->ent->update = updatePlayerDead; gameState = runGameStateGameOver; gameOverTimer = 4.0;}
		}
	}
	
	// dash cancel lmao
	if(keys[UP + (player->playerNumber * UP2)].pressedTimer > 0.1 && player->jumpCounter > 0) {
		player->jumpCounter--;
		ent->vel.y = -2;
		player->dashTimer = 0;
		player->dashCooldown = 0.3;
		ent->update = updatePlayerInAir;
	}
	
	if(playerBoundaryCheck(ent)) {
		givePlayerKnockback(ent, 0.5);
	}
}


void updatePlayerKnockback(struct entity* ent, double deltaTime){
	struct playerStruct* player = ent->object;
	ent->pos.x += ent->vel.x * deltaTime;
	ent->pos.y += ent->vel.y * deltaTime;
	
	ent->vel.y += (GRAVITY * ((keys[DOWN + (player->playerNumber * UP2)].held * 2)+1)) * deltaTime;
	
	if(player->knockbackTimer > 0) {
		player->knockbackTimer -= deltaTime * 0.001;
	} else {
		ent->update = updatePlayerInAir;
	}
	
	// I forgot to check for the floor but it's only noticable if you fastfall during knockback and I think it's funny so I'm leaving it like this lmao
	
	playerBoundaryCheck(ent);
}

void updatePlayerDead(struct entity* ent, double deltaTime) {
	ent->pos.x += ent->vel.x * deltaTime;
	ent->pos.y += ent->vel.y * deltaTime;
	
	#define GRAVITY 0.01
	ent->vel.y += GRAVITY * deltaTime;
	
	if(ent->pos.y > (4*HEIGHT/5 - ent->size.y)+10) {
		ent->pos.y = 4*HEIGHT/5 - ent->size.y;
		ent->vel.y = 0;
	}
	
	
	playerBoundaryCheck(ent);
}

void givePlayerKnockback(struct entity* ent, float force) {
	struct playerStruct* player = ent->object; 
	player->knockbackTimer = 0.2;
	ent->vel.x = -force * (ent->vel.x >= 0 ? 1 : -1);
	ent->vel.y = -(fabs(force)*4);
	ent->update = updatePlayerKnockback;
	player->dashTimer = 0;
	player->dashCooldown = 0.5;
}
