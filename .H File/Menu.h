#pragma once
#include"SDL.h"
#include <SDL_ttf.h>
#include "TextureLoader.h"


class Menu{
    public:
    //Menu(SDL_Surface* screen, SDL_Renderer* renderer, TTF_Font* font);
    TTF_Font* font = NULL;
    TTF_Font* fontSmaller = TTF_OpenFont("C:/Stuff/Pixelated.ttf", 60);
    int showMenu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
    void renderText(SDL_Renderer* renderer, float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
    const char* getHighScoreText();
};
