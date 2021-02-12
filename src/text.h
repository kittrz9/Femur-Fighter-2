#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern TTF_Font* font;

void drawText(SDL_Renderer* renderer, char* str, SDL_Color col, int x, int y, int w, int h);

#define drawTextCentered(renderer, str, col, x, y, w, h) drawText(renderer, str, col, x - w/2, y - h/2, w, h)

#endif
