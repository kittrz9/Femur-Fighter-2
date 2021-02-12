#include "text.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
 
TTF_Font* font;

char* formatStr;

// https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
// This probably needs to be optimized since the deltatime went from like 0.01 to 0.2 when drawing text
void drawText(SDL_Renderer* renderer, char* str, SDL_Color col, int x, int y, int w, int h) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, str, col);
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;
    
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
