#include "player.h"

#include <stdlib.h>

#include "gameLoop.h" // Needed for WIDTH and HEIGHT for now

#include "text.h"

void initializePlayer(struct entity* ent) {
    ent->size.x = 150;
	ent->size.y = 200;
    ent->vel.x = 0;
    ent->vel.y = 0;
    ent->health = 100;
    
	ent->draw = drawPlayer;
	ent->update = updatePlayerInAir;
    ent->jumpCounter = 1;
    ent->dashTimer = 0;
    ent->dashCooldown = 0;
    if(ent->playerNumber){
        ent->pos.x = (3*WIDTH/4) - (ent->size.x / 2);
    } else {
        ent->pos.x = (WIDTH/4) - (ent->size.x / 2);
	ent->pos.y = HEIGHT/2;
    }
}
 
void drawPlayer(struct entity* ent, SDL_Renderer* renderer){
	SDL_Rect rect; // Maybe like unoptimal to have this always being created every time the player needs to be drawn but idk maybe the compiler will optimize it? I also feel like it's better than when I just had the SDL_Rect in the entity's struct
	rect.x = ent->pos.x;
	rect.y = ent->pos.y;
	rect.w = ent->size.x;
	rect.h = ent->size.y;

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
    } else {
        // Have the player have normal colors if nothing else changes it's color (Changes it back after it's changed when in other states)
        SDL_SetTextureColorMod(ent->texture, 255, 255, 255);
        SDL_SetTextureAlphaMod(ent->texture, 255);
    }
    SDL_RenderCopyEx(renderer, ent->texture, NULL, &rect, ent->vel.x * 6, NULL, (ent->facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
    
    sprintf(formatStr, "%i%%", ent->health);
    drawTextCentered(renderer, formatStr, SDL_Color_White, ent->pos.x + (ent->size.x / 2), ent->pos.y - 40, ent->size.x / 2, 30);
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
	ent->pos.x += ent->vel.x * deltaTime;
	ent->pos.y += ent->vel.y * deltaTime;

    // The "+ (ent->playerNumber * UP2)" is to use the right controlls for each character since UP2 is in the middle of the controls thing
    if(keys[UP + (ent->playerNumber * UP2)].pressedTimer > 0.1) {
        ent->vel.y = -2;
        ent->update = updatePlayerInAir;
    }
	
	if(keys[LEFT + (ent->playerNumber * UP2)].held) {
        ent->vel.x = -1.5;
        ent->facingLeft = true;
    } else if(keys[RIGHT + (ent->playerNumber * UP2)].held) {
        ent->vel.x =  1.5;
        ent->facingLeft = false;
    } else if(ent->vel.x > 0.1){
        ent->vel.x -= 0.01 * deltaTime;
    } else if(ent->vel.x < -0.1){
        ent->vel.x += 0.01 * deltaTime;
    } else {
        ent->vel.x = 0;
    }
	
	ent->dashCooldown -= deltaTime * 0.001;
	
	if(ent->dashCooldown < 0) {
        if(keys[DASH_LEFT + (ent->playerNumber * UP2)].held) {
            ent->vel.x = -2.5;
            ent->update = updatePlayerDashing;
            ent->dashTimer = 0.2;
        } else if(keys[DASH_RIGHT + (ent->playerNumber * UP2)].held) {
            ent->vel.x =  2.5;
            ent->update = updatePlayerDashing;
            ent->dashTimer = 0.2;
        }
    }
	
	playerBoundaryCheck(ent);
}

void updatePlayerInAir(struct entity* ent, double deltaTime){
	ent->pos.x += ent->vel.x * deltaTime;
	ent->pos.y += ent->vel.y * deltaTime;

#define GRAVITY 0.01
    ent->vel.y += (GRAVITY * ((keys[DOWN + (ent->playerNumber * UP2)].held * 2)+1)) * deltaTime;
    
    if(ent->pos.y > (4*HEIGHT/5 - ent->size.y)+10) {
        ent->pos.y = 4*HEIGHT/5 - ent->size.y;
        ent->vel.y = 0;
        ent->jumpCounter = 1;
        ent->update = updatePlayerOnGround;
    }
    
    if(keys[UP + (ent->playerNumber * UP2)].pressedTimer > 0.1 && ent->jumpCounter > 0) {
        ent->jumpCounter--;
        ent->vel.y = -2;
    }
	
	ent->dashCooldown -= deltaTime * 0.001; // Multiplying by 0.001 because the units in these timers are in seconds and not milliseconds and I don't want to change them
	
    if(keys[LEFT + (ent->playerNumber * UP2)].held) {
        ent->vel.x = -1.5;
        ent->facingLeft = true;
    } else if(keys[RIGHT + (ent->playerNumber * UP2)].held) {
        ent->vel.x =  1.5;
        ent->facingLeft = false;
    } else if(ent->vel.x > 0.1){
        ent->vel.x -= 0.01 * deltaTime;
    } else if(ent->vel.x < -0.1){
        ent->vel.x += 0.01 * deltaTime;
    } else {
        ent->vel.x = 0;
    }
	
	ent->dashCooldown -= deltaTime * 0.001;
	
	if(ent->dashCooldown < 0) {
        if(keys[DASH_LEFT + (ent->playerNumber * UP2)].held) {
            ent->vel.x = -2.5;
            ent->update = updatePlayerDashing;
            ent->dashTimer = 0.2;
        } else if(keys[DASH_RIGHT + (ent->playerNumber * UP2)].held) {
            ent->vel.x =  2.5;
            ent->update = updatePlayerDashing;
            ent->dashTimer = 0.2;
        }
    }
	
	
	playerBoundaryCheck(ent);
}

void updatePlayerDashing(struct entity* ent, double deltaTime){
    ent->pos.x += ent->vel.x * deltaTime;
    
    ent->dashTimer -= deltaTime*0.001;
    if(ent->dashTimer < 0){
        ent->vel.x /= 2;
        ent->update = updatePlayerInAir;
        ent->dashCooldown = 0.6;
    }
    
    
    
    // Probably really REALLY inneficient to loop through the list like this but I don't want to like pass the other player into the update function just for this right now
    for(struct entListNode* current = entListHead; current != NULL; current = current->next){
        if(ent != current->ent && current->ent->update != updatePlayerDead && checkEntityCollision(ent, current->ent)){
            struct entity* hitPlayer = (ent->dashTimer < current->ent->dashTimer ? ent : current->ent);
            struct entity* notHitPlayer = (ent->dashTimer > current->ent->dashTimer ? ent : current->ent);
            givePlayerKnockback(notHitPlayer, 0.5);
            hitPlayer->vel.x = notHitPlayer->vel.x;
            givePlayerKnockback(hitPlayer, 0.5);
            hitPlayer->health -= 5;
            
            if(hitPlayer->health <= 0) {hitPlayer->update = updatePlayerDead; gameState = runGameStateGameOver; gameOverTimer = 4.0;}
            if(notHitPlayer->health <= 0) {notHitPlayer->update = updatePlayerDead; gameState = runGameStateGameOver; gameOverTimer = 4.0;}
        }
    }
    
    // dash cancel lmao
    if(keys[UP + (ent->playerNumber * UP2)].pressedTimer > 0.1 && ent->jumpCounter > 0) {
        ent->jumpCounter--;
        ent->vel.y = -2;
        ent->dashTimer = 0;
        ent->dashCooldown = 0.3;
        ent->update = updatePlayerInAir;
    }
    
    if(playerBoundaryCheck(ent)) {
        givePlayerKnockback(ent, 0.5);
    }
}


void updatePlayerKnockback(struct entity* ent, double deltaTime){
    ent->pos.x += ent->vel.x * deltaTime;
	ent->pos.y += ent->vel.y * deltaTime;
    
    ent->vel.y += (GRAVITY * ((keys[DOWN + (ent->playerNumber * UP2)].held * 2)+1)) * deltaTime;
    
    if(ent->knockbackTimer > 0) {
        ent->knockbackTimer -= deltaTime * 0.001;
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
    ent->vel.y += (GRAVITY * ((keys[DOWN + (ent->playerNumber * UP2)].held * 2)+1)) * deltaTime;
    
    if(ent->pos.y > (4*HEIGHT/5 - ent->size.y)+10) {
        ent->pos.y = 4*HEIGHT/5 - ent->size.y;
        ent->vel.y = 0;
    }
    
    
    playerBoundaryCheck(ent);
}

void givePlayerKnockback(struct entity* ent, float force) {
    ent->knockbackTimer = 0.2;
    ent->vel.x = -force * (ent->vel.x >= 0 ? 1 : -1);
    ent->vel.y = -(fabs(force)*4);
    ent->update = updatePlayerKnockback;
    ent->dashTimer = 0;
    ent->dashCooldown = 0.5;
}
