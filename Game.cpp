#include "Game.h"

int Game::gold = 300;
int Game::CastleHealth = 10;

Game::Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight) :
    placementModeCurrent(PlacementMode::wall),
    level(renderer, windowWidth / tileSize, windowHeight / tileSize),
    spawnTimer(0.5f), roundTimer(5.0f) {

    //Run the game.
    if (window != nullptr && renderer != nullptr) {
        //Load the overlay texture.
        textureOverlay = TextureLoader::loadTexture(renderer, "Overlay.bmp");
        textureChoosingTower = TextureLoader::loadTexture(renderer, "ChooseTower.bmp");
        //Load the spawn unit sound.
        mix_ChunkSpawnUnit = SoundLoader::loadSound("Orge.ogg");

        //Store the current times for the clock.
        auto time1 = std::chrono::system_clock::now();
        auto time2 = std::chrono::system_clock::now();

        //The amount of time for each frame (60 fps).
        const float dT = 1.0f / 60.0f;

        SDL_Surface* screen;

        //Start the game loop and run until it's time to stop.
        bool running = true;

        while (running) {
            //Determine how much time has elapsed since the last frame.
            time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();
            if(CastleHealth <= 0)
                running = false;
            //If enough time has passed then do everything required to generate the next frame.
            if (timeDeltaFloat >= dT) {
                //Store the new time for the next frame.
                time1 = time2;

                processEvents(renderer, running);
                update(renderer, dT);
                draw(renderer);
            }
        }
    }
}


Game::~Game() {
    //Clean up.
    TextureLoader::deallocateTextures();
    SoundLoader::deallocateSounds();
}



void Game::processEvents(SDL_Renderer* renderer, bool& running) {
    bool mouseDownThisFrame = false;

    //Process events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

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

        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                //Quit the game.
            case SDL_SCANCODE_ESCAPE:
                running = false;
                break;

                //Set the current gamemode.
            case SDL_SCANCODE_1:
                placementModeCurrent = PlacementMode::wall;
                break;
            case SDL_SCANCODE_2:
                placementModeCurrent = PlacementMode::turret;
                break;
            case SDL_SCANCODE_3:
                placementModeCurrent = PlacementMode::debuffer;
                break;
            case SDL_SCANCODE_4:
                placementModeCurrent = PlacementMode::cannon;
                break;
                //Show/hide the overlay
            case SDL_SCANCODE_H:
                overlayVisible = !overlayVisible;
                break;


            }
        }
    }


    //Process input from the mouse cursor.
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    //Convert from the window's coordinate system to the game's coordinate system.
    Vector2D posMouse((float)mouseX / tileSize, (float)mouseY / tileSize);

    if ((mouseX <= 440 && mouseX >= 380) && (mouseY <= 540 && mouseY >= 490) && mouseDownThisFrame == 1){
            placementModeCurrent = PlacementMode::turret;
    } else if ((mouseX <= 520 && mouseX >= 460) && (mouseY <= 540 && mouseY >= 490) && mouseDownThisFrame == 1){
            placementModeCurrent = PlacementMode::cannon;
    } else if ((mouseX <= 590 && mouseX >= 550) && (mouseY <= 540 && mouseY >= 490) && mouseDownThisFrame == 1){
            placementModeCurrent = PlacementMode::debuffer;
    }

    if (mouseDownStatus > 0) {
        switch (mouseDownStatus) {
        case SDL_BUTTON_LEFT:
            switch (placementModeCurrent) { ////////////////////////
            case PlacementMode::wall:
                //Add wall at the mouse position.
                level.setTileWall((int)posMouse.x, (int)posMouse.y, true);
                break;
            case PlacementMode::turret:
                //Add the selected turret at the mouse position.
                if (mouseDownThisFrame)
                    addTurret(renderer, posMouse);
                break;
            case PlacementMode::debuffer:
                //Add the selected debuffer at the mouse position.
                if (mouseDownThisFrame)
                    addDebuffer(renderer, posMouse);
                break;
            case PlacementMode::cannon:
                if (mouseDownThisFrame)
                    addCannon(renderer, posMouse);
                break;
            }
            break;


        case SDL_BUTTON_RIGHT:
            //Remove wall at the mouse position.
            level.setTileWall((int)posMouse.x, (int)posMouse.y, false);
            //Remove turret at the mouse position.
            removeTurretsAtMousePosition(posMouse);
            //Remove debuffer at the mouse positio.
            removeDebuffersAtMousePosition(posMouse);
            break;
        }
    }
}



void Game::update(SDL_Renderer* renderer, float dT) {
    //Update the units
    updateUnits(dT);

   /* for (auto it = listUnits.begin(); it != listUnits.end();) {
        (*it)->update(dT, level, listUnits);
        //MODIFIED
        if ((*it)->isAlive() == false){
            it = listUnits.erase(it);
        }
        else
            ++it;
    }
    */

    //Update the turrets.
    for(auto& turretSelected : listTurrets)
        turretSelected.update(renderer, dT, listUnits, listProjectiles);

    //Update the projectiles.
    updateProjectiles(dT);

    //Update the debuffers.
    for(auto& debuffSelected : listDebuffers)
        debuffSelected.update(renderer, dT, listUnits, listWaves);

    //Update the waves.
    updateWave(dT);

    //Update the cannons
    for(auto& cannonSelected : listCannons)
        cannonSelected.update(renderer, dT, listUnits, listBalls);

    //Update the balls
    updateCannonBall(dT);


    updateSpawnUnitsIfRequired(renderer, dT);



}


void Game::updateUnits(float dT){
    auto it = listUnits.begin();
    while (it != listUnits.end()) {
        bool increment = true;

        if((*it) != nullptr) {
            (*it) -> update(dT, level, listUnits);

            //Check whether the unit is alive or not. If not then erase it and don't increment the iterator
            if(!(*it) -> isAlive()) {
                it = listUnits.erase(it);
                increment = false;
            }
        }
        if (increment)
            it++;
    }
}

void Game::updateProjectiles(float dT) {
    //Loop through the list of projectiles and update all of them
    auto it = listProjectiles.begin();
    while (it != listProjectiles.end()) {
        (*it).update(dT, listUnits);

        //Check if the projectile has collided or not, erase it if needed, and update the iterator.
        if ((*it).getCollisionOccurred())
            it = listProjectiles.erase(it);
        else it++;
    }
}

void Game::updateWave(float dT) {
    //Loop through the list of projectiles and update all of them
    auto it = listWaves.begin();
    while (it != listWaves.end()) {
        (*it).update(dT, listUnits);

        //Check if the projectile has collided or not, erase it if needed, and update the iterator.
        if ((*it).getCollisionOccurred())
            it = listWaves.erase(it);
        else it++;
    }
}

void Game::updateCannonBall(float dT) {
    //Loop through the list of projectiles and update all of them
    auto it = listBalls.begin();
    while (it != listBalls.end()) {
        (*it).update(dT, listUnits);

        //Check if the projectile has collided or not, erase it if needed, and update the iterator.
        if ((*it).getCollisionOccurred())
            it = listBalls.erase(it);
        else it++;
    }
}


void Game::updateSpawnUnitsIfRequired(SDL_Renderer* renderer, float dT) {
    spawnTimer.countDown(dT);

    //Check if the round needs to start.
    if (listUnits.empty() && spawnUnitCount == 0) {
        roundTimer.countDown(dT);
        if (roundTimer.timeSIsZero()) {
            spawnUnitCount = 10;
            roundTimer.resetToMax();
        }
    }

    //Add a unit if needed.
    if (spawnUnitCount > 0 && spawnTimer.timeSIsZero()) {
        addUnit(renderer, level.getRandomEnemySpawnerLocation());

        //Play the spawn unit sound.
        /*if (mix_ChunkSpawnUnit != nullptr)
            Mix_PlayChannel(-1, mix_ChunkSpawnUnit, 0);
*/
        spawnUnitCount--;
        spawnTimer.resetToMax();
    }
}



void Game::draw(SDL_Renderer* renderer) {
    //Draw.
    //Set the draw color to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //Clear the screen.
    SDL_RenderClear(renderer);


    //Draw everything here.
    //Draw the level.
    level.draw(renderer, tileSize);

    //Draw the enemy units.
    for (auto& unitSelected : listUnits)
        if (unitSelected != nullptr)
        unitSelected->draw(renderer, tileSize);

    //Draw the turrets.
    for(auto& turretSelected : listTurrets)
        turretSelected.draw(renderer, tileSize);

    //Draw the projectiles.
    for(auto& projectileSelected : listProjectiles)
        projectileSelected.draw(renderer, tileSize);

    //Draw the debuffers.
    for(auto& debuffSelected : listDebuffers)
        debuffSelected.draw(renderer, tileSize);

    //Draw the wave.
    for(auto& waveSelected : listWaves)
        waveSelected.draw(renderer, tileSize);

    //Draw the cannons.
    for(auto& cannonSelected : listCannons)
        cannonSelected.draw(renderer, tileSize);
    //Draw the cannonball.
    for(auto& ballSelected : listBalls)
        ballSelected.draw(renderer, tileSize);

    //Draw the overlay.
    if (textureOverlay != nullptr && overlayVisible) {
        int w = 0, h = 0;
        SDL_QueryTexture(textureOverlay, NULL, NULL, &w, &h);
        SDL_Rect rect = { 40, 40, w, h };
        SDL_RenderCopy(renderer, textureOverlay, NULL, &rect);
    }

    //Draw the choosing menu.
    if (textureChoosingTower != nullptr) {
        int w, h;
        SDL_QueryTexture(textureChoosingTower, NULL, NULL, &w, &h);
        SDL_Rect rect = {300, 350, w, h};
        SDL_RenderCopy(renderer, textureChoosingTower, NULL, &rect);
    }

    renderGold(renderer, 450, 5, getGoldText(),
                     font32, Yellow);
    if(CastleHealth >= 0)
    renderGold(renderer, 350, 5, getHealthText(),
                    font32, Yellow);
    //renderCenter(0, 3 + 32, getStrokeText(), font32, black);


    //Send the image to the window.
    SDL_RenderPresent(renderer);
}



void Game::addUnit(SDL_Renderer* renderer, Vector2D posMouse) {
    listUnits.push_back(std::make_shared<Unit>(renderer, posMouse));
}

void Game::addTurret(SDL_Renderer* renderer, Vector2D posMouse) {
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    if(!level.noTower((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f))
        if(gold >= 100){
    gold -= 100;
    listTurrets.push_back(Turret(renderer, pos));
        }
    else {};

}

void Game::addDebuffer(SDL_Renderer* renderer, Vector2D posMouse) {
    if(!level.noTower((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f)){
    if(gold >= 125){
        gold -= 125;
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    listDebuffers.push_back(Debuffer(renderer, pos));
    }
    } else {};

}

void Game::addCannon(SDL_Renderer* renderer, Vector2D posMouse) {
    if(!level.noTower((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f)){
    if(gold >= 175){
        gold -= 175;
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    listCannons.push_back(Cannon(renderer, pos));
    }
    } else {};

}
void Game::removeTurretsAtMousePosition(Vector2D posMouse) {
   for (auto it = listTurrets.begin(); it != listTurrets.end();){
    if ((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
        it = listTurrets.erase(it);
    else
        it++;
   }
}

void Game::removeDebuffersAtMousePosition(Vector2D posMouse) {
    for (auto it = listDebuffers.begin(); it != listDebuffers.end();) {
        if((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
            it = listDebuffers.erase(it);
        else
            it++;
    }
}



void Game::removeCannonsAtMousePosition(Vector2D posMouse) {
    for (auto it = listCannons.begin(); it != listCannons.end();) {
        if((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
            it = listCannons.erase(it);
        else
            it++;
    }
}

const char* Game::getGoldText(){
    std::string s = std::to_string(gold);
    s = "GOLD: " + s;
    return s.c_str();
}

const char* Game::getHealthText(){
    std::string s = std::to_string(CastleHealth);
    s = "HEALTH: " + s;
    return s.c_str();
}

void Game::renderGold(SDL_Renderer* renderer, float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
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
