#include "Menu.h"
#include "TextureLoader.h"


int Menu::showMenu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    int windowWidth = 0, windowHeight = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    Uint32 time;
    int x, y;
    const int NUMMENU = 2;
    const char* labels[NUMMENU] = { "PLAY", "EXIT" };
    SDL_Surface* menus[NUMMENU];
    bool selected[NUMMENU] = { false, false };
    SDL_Color color[2] = { {107, 142, 35}, {85, 107, 47} };
    //(85,107,47)
    //(107,142,35)
    SDL_Rect screen = { 0, 0, 960, 576 };
    SDL_Texture* textureGameName = TextureLoader::loadTexture(renderer, "orig.bmp");
    SDL_RenderCopy(renderer, textureGameName, NULL, &screen);
    SDL_RenderPresent(renderer);

    menus[0] = TTF_RenderText_Solid(font, labels[0], color[0]);
    SDL_Texture* texturePlay = SDL_CreateTextureFromSurface(renderer, menus[0]);
    SDL_Rect dstRectPlay = { windowWidth / 2 - menus[0]->w / 2, windowHeight / 2 - menus[0]->h, menus[0]->w, menus[0]->h };

    menus[1] = TTF_RenderText_Solid(font, labels[1], color[0]);
    SDL_Texture* textureExit = SDL_CreateTextureFromSurface(renderer, menus[1]);
    SDL_Rect dstRectExit = { windowWidth / 2 - menus[1]->w / 2, windowHeight / 2 + 100 - menus[1]->h, menus[1]->w, menus[1]->h };

    SDL_Event event;
    while (true) {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                for (int i = 0; i < NUMMENU; i++)
                    SDL_FreeSurface(menus[i]);
                SDL_DestroyTexture(texturePlay);
                SDL_DestroyTexture(textureExit);
                SDL_DestroyTexture(textureGameName);
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < NUMMENU; i++) {
                    if (x >= dstRectPlay.x && x <= dstRectPlay.x + dstRectPlay.w &&
                        y >= dstRectPlay.y && y <= dstRectPlay.y + dstRectPlay.h) {
                        if (!selected[i]) {
                            selected[i] = true;
                            SDL_FreeSurface(menus[i]);
                            menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
                            if (i == 0) {
                                SDL_DestroyTexture(texturePlay);
                                texturePlay = SDL_CreateTextureFromSurface(renderer, menus[i]);
                            }
                            else {
                                SDL_DestroyTexture(textureExit);
                                textureExit = SDL_CreateTextureFromSurface(renderer, menus[i]);
                            }
                        }
                    }
                    else {
                        if (selected[i]) {
                            selected[i] = false;
                            SDL_FreeSurface(menus[i]);
                            menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
                            if (i == 0) {
                                SDL_DestroyTexture(texturePlay);
                                texturePlay = SDL_CreateTextureFromSurface(renderer, menus[i]);
                            }
                            else {
                                SDL_DestroyTexture(textureExit);
                                textureExit = SDL_CreateTextureFromSurface(renderer, menus[i]);
                            }
                        }
                    }
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < NUMMENU; i++) {
                    if (x >= dstRectPlay.x && x <= dstRectPlay.x + dstRectPlay.w &&
                        y >= dstRectPlay.y && y <= dstRectPlay.y + dstRectPlay.h) {
                        for (int j = 0; j < NUMMENU; j++)
                            SDL_FreeSurface(menus[j]);
                        SDL_DestroyTexture(texturePlay);
                        SDL_DestroyTexture(textureExit);
                        SDL_DestroyTexture(textureGameName);
                        return i;
                    }
                }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    for (int i = 0; i < NUMMENU; i++)
                        SDL_FreeSurface(menus[i]);
                    SDL_DestroyTexture(texturePlay);
                    SDL_DestroyTexture(textureExit);
                    SDL_DestroyTexture(textureGameName);
                    return 0;
                }
            }
        }

        SDL_RenderCopy(renderer, textureGameName, NULL, &screen);
        SDL_RenderCopy(renderer, texturePlay, NULL, &dstRectPlay);
        SDL_RenderCopy(renderer, textureExit, NULL, &dstRectExit);
        SDL_RenderPresent(renderer);

        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }
}
