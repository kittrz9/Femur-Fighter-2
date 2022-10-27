#ifndef GAME_STATE_STRUCT_H
#define GAME_STATE_STRUCT_H

#include <stdint.h>

#include "types.h"

// probably dumb to have another whole struct for players just because I don't want to get rid of the weird function pointer shenanigans but still want to be able to send the state
typedef struct {
	vec2f pos;
	vec2f vel;
	vec2f size;
	float dashTimer;
	float dashCooldown;
	float knockbackTimer;
	uint8_t facingLeft;
	uint8_t health;
	uint8_t jumpCounter;
} netPlayer;

typedef struct {
	netPlayer players[2];
} gameStateStruct;

#endif
