#include "config.h"

#define MAX_CONFIG_PATH_LENGTH 256
#define MAX_CONFIG_LENGTH 2048

char* configPath;
char* config;

/*
 * Game crashes when trying to create the config file but it's still created and initialized
 * Still needs to be fixed
 */

void getConfigFile(){
	configPath = malloc(sizeof(char) * MAX_CONFIG_PATH_LENGTH);
	sprintf(configPath, "%sconfig.txt", SDL_GetPrefPath("kittrz","FemurFighter2"));
	
	SDL_RWops* rw = SDL_RWFromFile(configPath, "r");
	if(rw == NULL){
		printf("Couldn't open config file: %s\nWill try to create config file\n", SDL_GetError());
		initConfigFile();
		rw = SDL_RWFromFile(configPath, "r");
		if(rw == NULL){
			printf("%s\n", SDL_GetError());
		}
	}
	config = malloc(sizeof(char) * MAX_CONFIG_LENGTH);
	SDL_RWread(rw, config, MAX_CONFIG_LENGTH, 1);
	SDL_RWclose(rw);
}

void initConfigFile(){
	SDL_RWops* rw = SDL_RWFromFile(configPath, "w");
	if(rw == NULL){
		printf("Couldn't write to config file: %s\n", SDL_GetError());
	}
	const char* defaultConfig = "Left:A\nRight:D\nUp:W\nDown:S\nDashL:Q\nDashR:E\nLeft2:Keypad 4\nRight2:Keypad 6\nUp2:Keypad 8\nDown2:Keypad 5\nDashL2:Keypad 7\nDashR2:Keypad 9\nPause:Escape\nMenuConfirm:Return";
	if(SDL_RWwrite(rw, defaultConfig, 1, strlen(defaultConfig)) != strlen(defaultConfig)){
		printf("Couldn't write to config file: %s\n", SDL_GetError());
	}
	SDL_RWclose(rw);
}

void saveConfigFile(){
	SDL_RWops* rw = SDL_RWFromFile(configPath, "w");
	
	if(SDL_RWwrite(rw, config, 1, strlen(config)) != strlen(config)){
		printf("Couldn't write to config file: %s\n", SDL_GetError());
	}
	SDL_RWclose(rw);
}
