#include "controls.h" 

#include "config.h"

// I need this for some reason to have the extern thing work
key keys[CONTROLS_LENGTH]; 

void initControls() {
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
	
	for(int i = 0; /*0strcmp(lines[i], "")*/i < linesCount; i++){
		//printf("%s\n", lines[i]);
		controlName = strtok(lines[i], delim2);
		keyName = strtok(NULL, delim2);
		printf("%s   %s\n", controlName, keyName);
		// This is probably a very VERY stupid way of making this work but switch statements with strings dont work in C
		if(strcmp(controlName, "Left") == 0) {
			controlPointer = &keys[LEFT].keycode;
		} else if (strcmp(controlName, "Right") == 0) {
			controlPointer = &keys[RIGHT].keycode;
		} else if (strcmp(controlName, "Up") == 0) {
			controlPointer = &keys[UP].keycode;
		} else if (strcmp(controlName, "Down") == 0) {
			controlPointer = &keys[DOWN].keycode;
		} else if (strcmp(controlName, "DashL") == 0) {
			controlPointer = &keys[DASH_LEFT].keycode;
		} else if (strcmp(controlName, "DashR") == 0) {
			controlPointer = &keys[DASH_RIGHT].keycode;
		} else if(strcmp(controlName, "Left2") == 0) {
			controlPointer = &keys[LEFT2].keycode;
		} else if (strcmp(controlName, "Right2") == 0) {
			controlPointer = &keys[RIGHT2].keycode;
		} else if (strcmp(controlName, "Up2") == 0) {
			controlPointer = &keys[UP2].keycode;
		} else if (strcmp(controlName, "Down2") == 0) {
			controlPointer = &keys[DOWN2].keycode;
		} else if (strcmp(controlName, "DashL2") == 0) {
			controlPointer = &keys[DASH_LEFT2].keycode;
		} else if (strcmp(controlName, "DashR2") == 0) {
			controlPointer = &keys[DASH_RIGHT2].keycode;
		} else if (strcmp(controlName, "Pause") == 0) {
			controlPointer = &keys[PAUSE].keycode;
		} else if (strcmp(controlName, "MenuConfirm") == 0) {
			controlPointer = &keys[MENU_CONFIRM].keycode;
		} else if (strcmp(controlName, "Exit") == 0) {
			controlPointer = &keys[EXIT].keycode;
		} else {
			printf("Invalid control name in config file: %s\n", controlName);
			break;
		}
		printf("%i\n", SDL_GetKeyFromName(keyName));
		*controlPointer = SDL_GetKeyFromName(keyName);
		printf("%i\n", *controlPointer);
	}
	
	printf("%i\n", keys[MENU_CONFIRM].keycode);
	
	// Also I guess I need this because it isn't always just initialized to false
	for(int i = 0; i < CONTROLS_LENGTH; i++){
		keys[i].held = false;
		keys[i].pressedTimer = 0.0;
	}
}
