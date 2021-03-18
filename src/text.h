#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MAX_STR_LEN 256

static const SDL_Color SDL_Color_White = {255, 255, 255}; // This is dumb
static const SDL_Color SDL_Color_Red = {255, 0, 0};

extern char* formatStr; // This is very dumb but I dont want to rewrite the drawText function to have formatted text right now

extern TTF_Font* font;

void drawText(SDL_Renderer* renderer, char* str, SDL_Color col, int x, int y, float scaling);
void drawTextCentered(SDL_Renderer* renderer, char* str, SDL_Color col, int x, int y, float scaling);

#endif
