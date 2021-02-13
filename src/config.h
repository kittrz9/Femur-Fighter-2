#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <SDL2/SDL.h>

extern char* configPath;
extern char* config;

void getConfigFile();
void initConfigFile();

#endif
