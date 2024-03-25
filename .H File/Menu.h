#pragma once
#include"SDL.h"
#include <SDL_ttf.h>
#include "TextureLoader.h"


class Menu{
    public:
    //Menu(SDL_Surface* screen, SDL_Renderer* renderer, TTF_Font* font);
    TTF_Font* font = NULL;
   int showMenu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);

};
