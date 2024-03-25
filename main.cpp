#include <iostream>
#include "SDL.h"
#include "Game.h"
#include <SDL_ttf.h>
#include "SDL_mixer.h"
#include "Menu.h"


int main(int argc, char* args[]) {
	//Seed the random number generator with the current time so that it will generate different
	//numbers every time the game is run.
	srand((unsigned)time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Error: Couldn't initialize SDL Video or Audio = " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		//Create the window.
		SDL_Window* window = SDL_CreateWindow("Tower Base Defense",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 576, 0);
		if (window == nullptr) {
			std::cout << "Error: Couldn't create window = " << SDL_GetError() << std::endl;
			return 1;
		}
		else {

            //Set up the audio mixer
            bool isSDLMixerLoaded = (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == 0);
            if (isSDLMixerLoaded == false) {
                std::cout << "Error: Couldn't initialize Mix_OpenAudio = " << Mix_GetError() << '\n';
            }
            else {
                Mix_AllocateChannels(32);

                //Output the name of the audio driver
                std::cout << "Audio drive = " << SDL_GetCurrentAudioDriver() << '\n';
            }
			//Create a renderer for GPU accelerated drawing.
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
				SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr) {
				std::cout << "Error: Couldn't create renderer = " << SDL_GetError() << std::endl;
				return 1;
			}
			else {
				//Ensure transparent graphics are drawn correctly.
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

				//Output the name of the render driver.
				SDL_RendererInfo rendererInfo;
				SDL_GetRendererInfo(renderer, &rendererInfo);
				std::cout << "Renderer = " << rendererInfo.name << std::endl;

				//Get the dimensions of the window.
				int windowWidth = 0, windowHeight = 0;
				SDL_GetWindowSize(window, &windowWidth, &windowHeight);

                TTF_Font* font = NULL;
                TTF_Font* font40 = NULL;

               if (TTF_Init() < 0) {
        std::cerr << "Error: Couldn't initialize SDL_ttf - " << TTF_GetError() << std::endl;
        SDL_Quit();  // Make sure to clean up SDL before exiting
        return 1;
    }
                font = TTF_OpenFont("C:/Windows/LoveDays.ttf", 18);
                font40 = TTF_OpenFont("C:/Stuff/Pixelated.ttf", 80);

                if (!font) {
    std::cerr << "Error: Couldn't load font - " << TTF_GetError() << std::endl;
    // Handle the error, return, or exit the program as appropriate.
}
                SDL_Surface* surface = NULL;

               /* if (!surface) {
    std::cerr << "Error: Unable to load BMP image - " << SDL_GetError() << std::endl;
}
*/

                //int i = showMenu(surface, renderer, font);

                //if (i == 1){
				//Start the game.
				/*Mix_Chunk* background = SoundLoader::loadSound("Kingdom Dance.ogg");
				if (background != nullptr)
                    Mix_PlayChannel(-1, background, 0);*/
                Mix_Music* background = Mix_LoadMUS("C:/Stuff/Kingdom Dance.ogg");
                Mix_Music* menu_background = Mix_LoadMUS("C:/Stuff/Medieval Music Wild Boars Inn.ogg");
                    if (!background) {
                        std::cerr << "Error: Couldn't load background music - " << Mix_GetError() << '\n';
                        // Handle the error, return, or exit the program as appropriate.
                        }

                    if (!menu_background) {
                        std::cerr << "Error: Couldn't load background music - " << Mix_GetError() << '\n';
                        // Handle the error, return, or exit the program as appropriate.
                        }

                /*if (Mix_PlayMusic(background, -1) == -1) {
    std::cerr << "Error: Couldn't play background music - " << Mix_GetError() << '\n';
    // Handle the error, return, or exit the program as appropriate.
}
*/
                Menu menu;
                 if (Mix_PlayMusic(menu_background, -1) == -1) {
    std::cerr << "Error: Couldn't play background music - " << Mix_GetError() << '\n';
    // Handle the error, return, or exit the program as appropriate.
}
                int i = menu.showMenu(window, renderer, font40);

                if(i == 0){

                Mix_FreeMusic(menu_background);
                     if (Mix_PlayMusic(background, -1) == -1) {
    std::cerr << "Error: Couldn't play background music - " << Mix_GetError() << '\n';
    // Handle the error, return, or exit the program as appropriate.
}
				Game game(window, renderer, windowWidth, windowHeight);
                }
                else {
				//Clean up.
				SDL_DestroyRenderer(renderer);
                TTF_CloseFont(font);

			//Clean up.
			SDL_DestroyWindow(window);


                Mix_FreeMusic(background);
                }
                if(isSDLMixerLoaded) {
                    Mix_CloseAudio();
                    Mix_Quit();
                }
		}
        TTF_Quit();
		//Clean up.
		SDL_Quit();
	//}
	}
	return 0;
}
}
