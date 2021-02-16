#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "types.h"

struct entity {
	vec2f pos, size, vel;
	
	// This is probably a dumb way to have entities have their own variables but whatever
	// Probably not really even worth it to have it be like this because the only entities are the players
	// But idk maybe I'll add like projectiles or particles later
	// Also might add this to the SDL Project Base or whatever so I can do this stuff in later projects
	void* object; // Pointer to the struct that is using this entity with their own variables
	
	SDL_Texture* texture;
	
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
