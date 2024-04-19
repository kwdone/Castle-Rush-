#pragma once
#include"SDL.h"
#include <SDL_ttf.h>
#include "TextureLoader.h"


class Map{
    public:
    //Menu(SDL_Surface* screen, SDL_Renderer* renderer, TTF_Font* font);
    TTF_Font* font = NULL;
    TTF_Font* fontSmaller = TTF_OpenFont("C:/Stuff/Pixelated.ttf", 60);
    int showMap(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
    void renderText(SDL_Renderer* renderer, float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
    int mouseDownStatus = 0;
    int mouseDownThisFrame = 0;
    enum class level {
	    level1,
		level2,
		level3,
		level4
	} levelChosen;
};

