#include "controls.h" 

#include "config.h"

// I need this for some reason to have the extern thing work
key keys[CONTROLS_LENGTH]; 

void initControls() {
	// There's probably a better way to do this
	keys[LEFT].name = "Left";
	keys[RIGHT].name = "Right";
	keys[UP].name = "Up";
	keys[DOWN].name = "Down";
	keys[DASH_LEFT].name = "DashL";
	keys[DASH_RIGHT].name = "DashR";
	keys[LEFT2].name = "Left2";
	keys[RIGHT2].name = "Right2";
	keys[UP2].name = "Up2";
	keys[DOWN2].name = "Down2";
	keys[DASH_LEFT2].name = "DashL2";
	keys[DASH_RIGHT2].name = "DashR2";
	keys[EXIT].name = "Exit";
	keys[PAUSE].name = "Pause";
	keys[MENU_CONFIRM].name = "MenuConfirm";
	
	// Getting this to work was hell
	// Split config file by lines
	const char* delim = "\n\0";
	const char* delim2 = ":";
	char* token = strtok(config, delim);
	char* controlName = malloc(sizeof(char) * 30);
	char* keyName = malloc(sizeof(char) * 30);
	char* lines[50];
	
	int linesCount = 0;
	while(token != NULL){
		lines[linesCount] = token;
		
		token = strtok(NULL, delim);
		linesCount++;
	}
	
	SDL_Keycode* controlPointer;
	
	for(int i = 0; i < linesCount; i++){
		controlName = strtok(lines[i], delim2);
		keyName = strtok(NULL, delim2);
		for(int i = 0; i < CONTROLS_LENGTH; i++){
			if(strcmp(controlName, keys[i].name) == 0){
				controlPointer = &keys[i].keycode;
				break;
			}
		}
		if(controlPointer != NULL) {
			*controlPointer = SDL_GetKeyFromName(keyName);
		}
		controlPointer = NULL;
	}
	
	// Also I guess I need this because it isn't always just initialized to false
	for(int i = 0; i < CONTROLS_LENGTH; i++){
		keys[i].held = false;
		keys[i].pressedTimer = 0.0;
	}
}
