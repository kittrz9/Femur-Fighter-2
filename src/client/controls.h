#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
	bool held;
	float pressedTimer; // Timer for how long the key is considered "pressed" instead of "held"
	SDL_Keycode keycode;
	
	char* name;
} key;

enum CONTROLS{UP, DOWN, LEFT, RIGHT, DASH_LEFT, DASH_RIGHT, UP2, DOWN2, LEFT2, RIGHT2, DASH_LEFT2, DASH_RIGHT2, EXIT, PAUSE, MENU_CONFIRM, CONTROLS_LENGTH};
extern key keys[CONTROLS_LENGTH]; 

void initControls();

#endif
