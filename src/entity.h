#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
	float x, y;
} vec2f;

// Really need to find a way to have like specific entities/entity types have their own variables
// so not everything has it's own like jump counter lmao
// Doesnt really affect this thing since the only entities (so far) are just the players
struct entity {
	vec2f pos, size, vel;
	
	SDL_Texture* texture;
	int jumpCounter;
	bool facingLeft; // Lmao this is a stupid way to draw flippped characters
	int playerNumber;
	int health;
	float dashTimer;
	float dashCooldown;
	float knockbackTimer;
	
	void (*draw)(struct entity*, SDL_Renderer*);
	void (*update)(struct entity*, double);
};

bool checkEntityCollision(struct entity* ent1, struct entity* ent2);

// Linked list stuff
struct entListNode{
	struct entity* ent;
	struct entListNode* next;
	struct entListNode* prev;
};
extern struct entListNode* entListHead;
extern struct entListNode* entListCurrent;
extern struct entListNode* entListTail;

void pushToEntityList(struct entity* ent);
void removeFromEntityList(struct entity* ent);
void removeEntityList();

#endif
