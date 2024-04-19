
#include "SelectionMap.h"
#include "TextureLoader.h"
#include <vector>
#include <algorithm>
#include "Vector2D.h"
#include "Level.h"

/*
int Map::showMap(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    int windowWidth = 0, windowHeight = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    Uint32 time;
   // bool selected[NUMLEVELS] = { false, false, false, false };
   // SDL_Color color[2] = { {107, 142, 35}, {85, 107, 47} };
    SDL_Rect screen = { 0, 0, 960, 576 };
    SDL_Rect dstBoard = { 200, 150, 551, 300};
   // SDL_Texture* highScoreBoard = TextureLoader::loadTexture(renderer, "Board.bmp");
    SDL_Texture* textureMap = TextureLoader::loadTexture(renderer, "Selection Map.bmp");
    SDL_Texture* pointer = TextureLoader::loadTexture(renderer, "Arrow_Down.bmp");

//    SDL_Rect dstArrow = {};
//    SDL_RenderCopy()
    SDL_RenderCopy(renderer, textureMap, NULL, &screen);
    SDL_RenderPresent(renderer);

//    menus[0] = TTF_RenderText_Solid(font, labels[0], color[0]);
//    SDL_Texture* texturePlay = SDL_CreateTextureFromSurface(renderer, menus[0]);
  //  SDL_Rect level1 = { windowWidth / 2 - menus[0]->w / 2, windowHeight / 2 - menus[0]->h, menus[0]->w, menus[0]->h };
//
//    menus[1] = TTF_RenderText_Solid(font, labels[1], color[0]);
//    SDL_Texture* texture2P = SDL_CreateTextureFromSurface(renderer, menus[1]);
  //  SDL_Rect level2 = { windowWidth / 2 - menus[1]->w/2, windowHeight / 2 + 100 - menus[1]->h, menus[1]->w, menus[1]->h };
//
//    menus[2] = TTF_RenderText_Solid(font, labels[2], color[0]);
//    SDL_Texture* textureExit = SDL_CreateTextureFromSurface(renderer, menus[2]);
  //  SDL_Rect level3 = { windowWidth / 2 - menus[2]->w / 2, windowHeight / 2 + 200 - menus[2]->h, menus[2]->w, menus[2]->h };
//
//    menus[3] = TTF_RenderText_Solid(font, labels[3], color[0]);
//    SDL_Texture* textureRecord = SDL_CreateTextureFromSurface(renderer, menus[3]);
  //  SDL_Rect level4 = { windowWidth / 2 - menus[3]->w/2, windowHeight / 2 + 300 - menus[3]->h, menus[3]->w, menus[3]->h };

//
//    bool showHighScore = false;

    SDL_Event event;
    while (true) {
      //  time = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                return 1;

            case SDL_MOUSEBUTTONDOWN:
            mouseDownThisFrame = (mouseDownStatus == 0);
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseDownStatus = SDL_BUTTON_LEFT;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mouseDownStatus = SDL_BUTTON_RIGHT;
            break;
        case SDL_MOUSEBUTTONUP:
            mouseDownStatus = 0;
            break;


            int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    //Convert from the window's coordinate system to the game's coordinate system.
    Vector2D posMouse(mouseX, mouseY);


    if ((mouseX <= 10 && mouseX >= 0) && (mouseY <= 10 && mouseY >= 0) && mouseDownThisFrame == 1){
            levelChosen = level::level1;
    } else if ((mouseX <= 520 && mouseX >= 460) && (mouseY <= 540 && mouseY >= 490) && mouseDownThisFrame == 1){
            levelChosen = level::level2;
    } else if ((mouseX <= 590 && mouseX >= 550) && (mouseY <= 540 && mouseY >= 490) && mouseDownThisFrame == 1){
            levelChosen = level::level3;
    }

     if (mouseDownStatus > 0) {
        switch (mouseDownStatus) {
        case SDL_BUTTON_LEFT:
            switch (levelChosen) { ////////////////////////
            case level::level1:
                //Add wall at the mouse position.
                if (mouseDownThisFrame)
                SDL_DestroyTexture(textureMap);
                SDL_DestroyTexture(pointer);
                return 1;
                break;
            case level::level2:
                //Add the selected turret at the mouse position.
                if (mouseDownThisFrame)
                SDL_DestroyTexture(textureMap);
                SDL_DestroyTexture(pointer);
                return 2;
                break;
            case level::level3:
                //Add the selected debuffer at the mouse position.
                if (mouseDownThisFrame)
                SDL_DestroyTexture(textureMap);
                SDL_DestroyTexture(pointer);
                return 3;
                break;
            }
            break;




        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }
}
            }
        }
    }
}
*/

int Map::showMap(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    int windowWidth = 0, windowHeight = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    Uint32 time;
    // bool selected[NUMLEVELS] = { false, false, false, false };
    // SDL_Color color[2] = { {107, 142, 35}, {85, 107, 47} };
    SDL_Rect screen = { 0, 0, 960, 576 };
    SDL_Rect dstBoard = { 200, 150, 551, 300};
    // SDL_Texture* highScoreBoard = TextureLoader::loadTexture(renderer, "Board.bmp");
    SDL_Texture* textureMap = TextureLoader::loadTexture(renderer, "Selection Map.bmp");
    SDL_Texture* pointer = TextureLoader::loadTexture(renderer, "Arrow_Down.bmp");

    SDL_RenderCopy(renderer, textureMap, NULL, &screen);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return 1;

                case SDL_MOUSEBUTTONDOWN:
                    mouseDownThisFrame = (mouseDownStatus == 0);
                    if (event.button.button == SDL_BUTTON_LEFT)
                        mouseDownStatus = SDL_BUTTON_LEFT;
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                        mouseDownStatus = SDL_BUTTON_RIGHT;
                    break;

                case SDL_MOUSEBUTTONUP:
                    mouseDownStatus = 0;
                    break;
            }
        }

        int mouseX = 0, mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);
        // Convert from the window's coordinate system to the game's coordinate system.
        Vector2D posMouse(mouseX, mouseY);

        // Check for level selection based on mouse position and button clicks
        if ((mouseX <= 190 && mouseX >= 170) && (mouseY <= 280 && mouseY >= 230) && mouseDownThisFrame == 1) {
            levelChosen = level::level1;
            SDL_Texture* arrow = TextureLoader::loadTexture(renderer, "Arrow_Down.bmp");
            SDL_Rect rect = {165, 215, 30, 30};
            SDL_RenderCopy(renderer, arrow, NULL, &rect);
            SDL_RenderPresent(renderer);
            //SDL_Delay(1000);
        } else if ((mouseX <= 380 && mouseX >= 320) && (mouseY <= 320 && mouseY >= 270) && mouseDownThisFrame == 1) {
            levelChosen = level::level2;
            SDL_Texture* arrow = TextureLoader::loadTexture(renderer, "Arrow_Down.bmp");
            SDL_Rect rect = {350, 280, 30, 30};
            SDL_RenderCopy(renderer, arrow, NULL, &rect);
            SDL_RenderPresent(renderer);
            //SDL_Delay(1000);
        } else if ((mouseX <= 380 && mouseX >= 320) && (mouseY <= 500 && mouseY >= 400) && mouseDownThisFrame == 1) {
            levelChosen = level::level3;
            SDL_Texture* arrow = TextureLoader::loadTexture(renderer, "Arrow_Down.bmp");
            SDL_Rect rect = {350, 420, 30, 30};
            SDL_RenderCopy(renderer, arrow, NULL, &rect);
            SDL_RenderPresent(renderer);
            //SDL_Delay(1000);
        }

        if (mouseDownStatus > 0) {
            switch (mouseDownStatus) {
                case SDL_BUTTON_LEFT:
                    switch (levelChosen) {
                        case level::level1:
                            // Handle level 1 selection
                            Level::chooseLevel = 1;
                            return 1;
                            break;
                        case level::level2:
                            // Handle level 2 selection
                            Level::chooseLevel = 2;
                            return 2;
                            break;
                        case level::level3:
                            // Handle level 3 selection
                            Level::chooseLevel = 3;
                            return 3;
                            break;
                    }
                    break;
            }
        }

        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }
}

void Map::renderText(SDL_Renderer* renderer, float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, p_text, textColor);
    if (surface == nullptr) {
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = { static_cast<int>(p_x), static_cast<int>(p_y), surface->w, surface->h };

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    // Clean up resources
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
