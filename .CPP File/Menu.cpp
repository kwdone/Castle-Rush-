#include "Menu.h"
#include "TextureLoader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int Menu::showMenu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    int windowWidth = 0, windowHeight = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    Uint32 time;
    int x, y;
    const int NUMMENU = 4;
    const char* labels[NUMMENU] = { "CLASSIC", "2P MODE","EXIT", "RECORD" };
    SDL_Surface* menus[NUMMENU];
    bool selected[NUMMENU] = { false, false, false, false };
    SDL_Color color[2] = { {107, 142, 35}, {85, 107, 47} };
    SDL_Rect screen = { 0, 0, 960, 576 };
    SDL_Rect dstBoard = { 200, 150, 551, 300};
    SDL_Texture* highScoreBoard = TextureLoader::loadTexture(renderer, "Board.bmp");
    SDL_Texture* textureGameName = TextureLoader::loadTexture(renderer, "orig.bmp");
    SDL_RenderCopy(renderer, textureGameName, NULL, &screen);
    SDL_RenderPresent(renderer);

    menus[0] = TTF_RenderText_Solid(font, labels[0], color[0]);
    SDL_Texture* texturePlay = SDL_CreateTextureFromSurface(renderer, menus[0]);
    SDL_Rect dstRectPlay = { windowWidth / 2 - menus[0]->w / 2, windowHeight / 2 - menus[0]->h, menus[0]->w, menus[0]->h };

    menus[1] = TTF_RenderText_Solid(font, labels[1], color[0]);
    SDL_Texture* texture2P = SDL_CreateTextureFromSurface(renderer, menus[1]);
    SDL_Rect dstRect2P = { windowWidth / 2 - menus[1]->w/2, windowHeight / 2 + 100 - menus[1]->h, menus[1]->w, menus[1]->h };

    menus[2] = TTF_RenderText_Solid(font, labels[2], color[0]);
    SDL_Texture* textureExit = SDL_CreateTextureFromSurface(renderer, menus[2]);
    SDL_Rect dstRectExit = { windowWidth / 2 - menus[2]->w / 2, windowHeight / 2 + 200 - menus[2]->h, menus[2]->w, menus[2]->h };

    menus[3] = TTF_RenderText_Solid(font, labels[3], color[0]);
    SDL_Texture* textureRecord = SDL_CreateTextureFromSurface(renderer, menus[3]);
    SDL_Rect dstRectRecord = { windowWidth / 2 - menus[3]->w/2, windowHeight / 2 + 300 - menus[3]->h, menus[3]->w, menus[3]->h };


    bool showHighScore = false;

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
                SDL_DestroyTexture(texture2P);
                SDL_DestroyTexture(textureGameName);
                SDL_DestroyTexture(textureRecord);
                SDL_DestroyTexture(highScoreBoard);
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
                        }
                    }
                    else if (x >= dstRect2P.x && x <= dstRect2P.x + dstRect2P.w &&
                            y >= dstRect2P.y && y <= dstRect2P.y + dstRect2P.h) {
                        if (!selected[i]) {
                            selected[i] = true;
                            SDL_FreeSurface(menus[i]);
                            menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);

                            if (i == 1) {
                                SDL_DestroyTexture(texture2P);
                                texture2P = SDL_CreateTextureFromSurface(renderer, menus[i]);
                            }
                        }
                    }
                    else if (x >= dstRectExit.x && x <= dstRectExit.x + dstRectExit.w &&
                            y >= dstRectExit.y && y <= dstRectExit.y + dstRectExit.h) {
                        if (!selected[i]) {
                            selected[i] = true;
                            SDL_FreeSurface(menus[i]);
                            menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);

                            if (i == 2) {
                            SDL_DestroyTexture(textureExit);
                            textureExit = SDL_CreateTextureFromSurface(renderer, menus[i]);
                            }
                        }
                    }
                    else if (x >= dstRectRecord.x && x <= dstRectRecord.x + dstRectRecord.w &&
                            y >= dstRectRecord.y && y <= dstRectRecord.y + dstRectRecord.h) {
                        if (!selected[3]) {
                            selected[3] = true;
                            SDL_FreeSurface(menus[i]);
                            menus[3] = TTF_RenderText_Solid(font, labels[3], color[1]);

                            SDL_DestroyTexture(textureRecord);
                            textureRecord = SDL_CreateTextureFromSurface(renderer, menus[3]);
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
                            if (i == 1) {
                                SDL_DestroyTexture(texture2P);
                                texture2P = SDL_CreateTextureFromSurface(renderer, menus[i]);
                            }
                            if (i == 2) {
                                SDL_DestroyTexture(textureExit);
                                textureExit= SDL_CreateTextureFromSurface(renderer, menus[i]);
                            }
                            if (i == 3) {
                                SDL_DestroyTexture(textureRecord);
                                textureRecord = SDL_CreateTextureFromSurface(renderer, menus[i]);
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
                        SDL_DestroyTexture(texture2P);
                        SDL_DestroyTexture(textureGameName);
                        SDL_DestroyTexture(textureRecord);
                        SDL_DestroyTexture(highScoreBoard);
                        return i;
                    }
                    else if (x >= dstRectExit.x && x <= dstRectExit.x + dstRectExit.w &&
                            y >= dstRectExit.y && y <= dstRectExit.y + dstRectExit.h) {
                        for (int j = 0; j < NUMMENU; j++)
                            SDL_FreeSurface(menus[j]);
                        SDL_DestroyTexture(texturePlay);
                        SDL_DestroyTexture(textureExit);
                        SDL_DestroyTexture(texture2P);
                        SDL_DestroyTexture(textureGameName);
                        SDL_DestroyTexture(textureRecord);
                        SDL_DestroyTexture(highScoreBoard);
                        return 1;
                    }
                    else if (x >= dstRect2P.x && x <= dstRect2P.x + dstRect2P.w &&
                            y >= dstRect2P.y && y <= dstRect2P.y + dstRect2P.h) {
                             for (int j = 0; j < NUMMENU; j++)
                             SDL_FreeSurface(menus[j]);
                        SDL_DestroyTexture(texturePlay);
                        SDL_DestroyTexture(textureExit);
                        SDL_DestroyTexture(texture2P);
                        SDL_DestroyTexture(textureGameName);
                        SDL_DestroyTexture(textureRecord);
                        SDL_DestroyTexture(highScoreBoard);
                        return 2;
                            }
                    else if (x >= dstRectRecord.x && x <= dstRectRecord.x + dstRectRecord.w &&
                            y >= dstRectRecord.y && y <= dstRectRecord.y + dstRectRecord.h) {
                        if (i == 2) {
                            showHighScore = true;
                        }
                    }
                    else {
                        showHighScore = false;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    for (int i = 0; i < NUMMENU; i++)
                        SDL_FreeSurface(menus[i]);
                    SDL_DestroyTexture(texturePlay);
                    SDL_DestroyTexture(textureExit);
                    SDL_DestroyTexture(texture2P);
                    SDL_DestroyTexture(textureGameName);
                    SDL_DestroyTexture(textureRecord);
                    SDL_DestroyTexture(highScoreBoard);
                    return 0;
                }
            }
        }

        if (showHighScore) {
            SDL_RenderCopy(renderer, highScoreBoard, NULL, &dstBoard);
            renderText(renderer, 290, 270, getHighScoreText(), fontSmaller, {238, 232, 170});
            SDL_RenderPresent(renderer);
        } else {
            SDL_RenderCopy(renderer, textureGameName, NULL, &screen);
            SDL_RenderCopy(renderer, texturePlay, NULL, &dstRectPlay);
            SDL_RenderCopy(renderer, texture2P, NULL, &dstRect2P);
            SDL_RenderCopy(renderer, textureExit, NULL, &dstRectExit);
            SDL_RenderCopy(renderer, textureRecord, NULL, &dstRectRecord);
            //SDL_RenderCopy(renderer, texture2P, NULL, &dstRect2P);
            SDL_RenderPresent(renderer);
        }

        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }
}

void Menu::renderText(SDL_Renderer* renderer, float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
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

const char* Menu::getHighScoreText() {
    std::vector<int> scores;
    std::ifstream input("HighScore.txt");
    if (input.is_open()) {
        int score;
        while (input >> score) {
            scores.push_back(score);
        }
        input.close();
    }
    int highestScore = *std::max_element(scores.begin(), scores.end());
    std::string s = "HIGHEST SCORE: " + std::to_string(highestScore);
    return s.c_str();
}
