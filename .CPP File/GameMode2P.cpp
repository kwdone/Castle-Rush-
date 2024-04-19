#include "GameMode2P.h"
#include "Timer.h"
#include <iostream>

int Game2P::gold = 300;
int Game2P::CastleHealth = 10;
int Game2P::currentScore = 0;
int Game2P::elixir = 300;
bool Game2P::lose = false;

Game2P::Game2P(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight):
    placementModeCurrent(PlacementMode::wall),
    level2P(renderer, windowWidth / tileSize, windowHeight / tileSize),
    spawnTimer(0.5f), roundTimer(5.0f) {

    //Run the game.
    if (window != nullptr && renderer != nullptr) {
        //Load the overlay texture.
        textureOverlay = TextureLoader::loadTexture(renderer, "Overlay.bmp");
        textureChoosingTower = TextureLoader::loadTexture(renderer, "ChooseTower.bmp");
        textureSelectionFrame1P = TextureLoader::loadTexture(renderer, "Selection Frame.bmp");
        textureSelectionFrame2P = TextureLoader::loadTexture(renderer, "Enemy Selection Frame.bmp");
        enemyFrame = TextureLoader::loadTexture(renderer, "Enemy2P.bmp");
        towerFrame = TextureLoader::loadTexture(renderer, "Tower2P.bmp");
        towerSelect = TextureLoader::loadTexture(renderer, "Select.bmp");
        enemySelect = TextureLoader::loadTexture(renderer, "Select.bmp");

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
            if(CastleHealth <= 0 || totalRoundsSpawned == 21){  //MODIFIED: Make way for the wave of the boss at level 20
                running = false;
                lose = true;
            }
                ////////////////
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

Game2P::~Game2P() {
    //Clean up.
    TextureLoader::deallocateTextures();
    SoundLoader::deallocateSounds();
}


void Game2P::processEvents(SDL_Renderer* renderer, bool& running) {
    bool mouseDownThisFrame = false;
    Vector2D towerCursor((float)towerCursor_x / tileSize, (float)towerCursor_y / tileSize);
    Vector2D enemyCursor((float)enemyCursor_x / tileSize, (float)enemyCursor_y / tileSize);
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
            case SDL_SCANCODE_J:
                enemyModeCurrent = EnemyPlacementMode::elixirTower;
                if(selectForEnemy_x == 765)
                selectForEnemy_x -= 70;
                if(selectForEnemy_x == 833)
                selectForEnemy_x -= 138;
                break;
            case SDL_SCANCODE_K:
                if(selectForEnemy_x == 695)
                selectForEnemy_x += 70;
                if(selectForEnemy_x == 833)
                selectForEnemy_x -= 68;
                enemyModeCurrent = EnemyPlacementMode::orge;
                break;
            case SDL_SCANCODE_L:
                if(selectForEnemy_x == 695)
                selectForEnemy_x += 138;
                if(selectForEnemy_x == 765)
                selectForEnemy_x += 68;

                //Set the current gamemode.
            case SDL_SCANCODE_1:
                placementModeCurrent = PlacementMode::moneyTower;
                if(selectForTower_x == 155)
                selectForTower_x -= 75;
                else if(selectForTower_x == 222)
                selectForTower_x -= 142;
                break;
//            case SDL_SCANCODE_1:
//                placementModeCurrent = PlacementMode::wall;
//                break;
            case SDL_SCANCODE_2:
                placementModeCurrent = PlacementMode::turret;
                if(selectForTower_x == 80)
                selectForTower_x += 75;
                else if(selectForTower_x == 222)
                selectForTower_x -= 67;
                break;
            case SDL_SCANCODE_3:
                if(selectForTower_x == 80)
                selectForTower_x += 142;
                if(selectForTower_x == 155)
                selectForTower_x += 67;
                placementModeCurrent = PlacementMode::cannon;
                break;
//            case SDL_SCANCODE_4:
//                placementModeCurrent = PlacementMode::cannon;
//                break;
                //Show/hide the overlay
            case SDL_SCANCODE_H:
                overlayVisible = !overlayVisible;
                break;
            case SDL_SCANCODE_RIGHT:
                if (enemyCursor_x <= 832)
                enemyCursor_x += 64;
                break;
            case SDL_SCANCODE_LEFT:
                if (enemyCursor_x >= 480)
                enemyCursor_x -= 64;
                break;
            case SDL_SCANCODE_UP:
                if (enemyCursor_y >= 192)
                enemyCursor_y -= 64;
                break;
            case SDL_SCANCODE_DOWN:
                if (enemyCursor_y <= 320)
                enemyCursor_y += 64;
                break;
            case SDL_SCANCODE_A:
                if (towerCursor_x >= 64)
                towerCursor_x -= 64;
                break;
            case SDL_SCANCODE_S:
                if (towerCursor_y <= 320)
                towerCursor_y += 64;
                break;
            case SDL_SCANCODE_D:
                if (towerCursor_x <= 416)
                towerCursor_x += 64;
                break;
            case SDL_SCANCODE_W:
                if (towerCursor_y >= 192)
                towerCursor_y -= 64;
                break;
            case SDL_SCANCODE_SPACE:
                switch (enemyModeCurrent) {
                case EnemyPlacementMode::elixirTower:
                    addElixirTower(renderer, enemyCursor);
                    break;
                case EnemyPlacementMode::orge:
                    addUnit(renderer, enemyCursor, 4);
                    break;
                }
                break;
            case SDL_SCANCODE_Z:
                addTurret(renderer, towerCursor);
                break;
            case SDL_SCANCODE_X:
                switch (placementModeCurrent){
            case PlacementMode::turret:
                addTurret(renderer, towerCursor);
                break;
            case PlacementMode::cannon:
                addCannon(renderer, towerCursor);
                break;
            case PlacementMode::moneyTower:
                addMoneyTower(renderer, towerCursor);
                break;
                }
                break;
            }
        }
    }


    //Process input from the mouse cursor.
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    //Convert from the window's coordinate system to the game's coordinate system.
    Vector2D posMouse((float)mouseX / tileSize, (float)mouseY / tileSize);

    /*if ((mouseX <= 440 && mouseX >= 380) && (mouseY <= 540 && mouseY >= 490) && mouseDownThisFrame == 1){
            placementModeCurrent = PlacementMode::turret;
    } else if ((mouseX <= 520 && mouseX >= 460) && (mouseY <= 540 && mouseY >= 490) && mouseDownThisFrame == 1){
            placementModeCurrent = PlacementMode::cannon;
    } else if ((mouseX <= 590 && mouseX >= 550) && (mouseY <= 540 && mouseY >= 490) && mouseDownThisFrame == 1){
            placementModeCurrent = PlacementMode::debuffer;
    }
    */

    /*
    if (mouseDownStatus > 0) {
        switch (mouseDownStatus) {
        case SDL_BUTTON_LEFT:
            switch (placementModeCurrent) { ////////////////////////
            case PlacementMode::wall:
                //Add wall at the mouse position.
                level2P.setTileWall((int)posMouse.x, (int)posMouse.y, true);
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

*/
/*        case SDL_BUTTON_RIGHT:
            //Remove wall at the mouse position.
            level2P.setTileWall((int)posMouse.x, (int)posMouse.y, false);
            //Remove turret at the mouse position.
            removeTurretsAtMousePosition(posMouse);
            //Remove debuffer at the mouse positio.
            removeDebuffersAtMousePosition(posMouse);
            break;
        }
    }
    */
}


void Game2P::update(SDL_Renderer* renderer, float dT) {
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
    for(auto& towerSelected : listMoneyTowers)
        towerSelected.update(renderer, dT);

    for(auto& tower : listMoneyTowers) {
    if(tower.moneyTowerHealth <= 0) {
        removeMoneyTowersAtMousePosition(tower.pos);
        continue; // Skip to next turret if this one is destroyed
    }

    for(auto& unit : listUnits) {
        if((tower.pos - unit->pos).magnitude() <= 0.75f) {
            unit->moveOk = false;
            unit->timerAttack.countDown(dT);
            unit->texture = TextureLoader::loadTexture(renderer, "S_Attack.bmp");

            // Check if the unit's attack timer reached zero AND the unit is attacking the turret
            if(unit->timerAttack.timeSIsZero()) {
                if((tower.pos - unit->pos).magnitude() <= 0.75f) {
                    tower.moneyTowerHealth--;
                    unit->timerAttack.resetToMax();
                }
            }
        }
    }
}

// Handle destroyed turrets
for(auto& tower : listMoneyTowers) {
    if(tower.moneyTowerHealth <= 0) {
        for(auto& unit : listUnits) {
            unit->moveOk = true;
            unit->texture = TextureLoader::loadTexture(renderer, "S_Walk_Demo_2.bmp");
        }
    }
}


    for(auto& towerSelected : listElixirTowers)
        towerSelected.update(renderer, dT);

    for(auto& turretSelected : listTurrets)
        turretSelected.update(renderer, dT, listUnits, listProjectiles);
/*
    for(auto& turretSelected : listTurrets){
        for (auto& unitSelected : listUnits){
        if((turretSelected.pos - unitSelected->pos).magnitude() <= 0.75f){
            unitSelected->moveOk = false;
            unitSelected->timerAttack.countDown(dT);
            unitSelected->texture = TextureLoader::loadTexture(renderer, "S_Attack.bmp");

            if(unitSelected->timerAttack.timeSIsZero()){
            turretSelected.turretHealth--;
            }
            if(turretSelected.turretHealth <= 0){
                for(auto& unit : listUnits){
                unit->moveOk = true;
                unit->texture = TextureLoader::loadTexture(renderer, "S_Walk_Demo_2.bmp");
                }
            }
            unitSelected->timerAttack.resetToMax();
        }
        }
        if(turretSelected.turretHealth == 0)
            removeTurretsAtMousePosition(turretSelected.pos);
        }

*/
    // Check for collisions between turrets and units
for(auto& turret : listTurrets) {
    if(turret.turretHealth <= 0) {
        removeTurretsAtMousePosition(turret.pos);
        continue; // Skip to next turret if this one is destroyed
    }

    for(auto& unit : listUnits) {
        if((turret.pos - unit->pos).magnitude() <= 0.75f) {
            unit->moveOk = false;
            unit->timerAttack.countDown(dT);
            unit->texture = TextureLoader::loadTexture(renderer, "S_Attack.bmp");

            // Check if the unit's attack timer reached zero AND the unit is attacking the turret
            if(unit->timerAttack.timeSIsZero()) {
                if((turret.pos - unit->pos).magnitude() <= 0.75f) {
                    turret.turretHealth--;
                    unit->timerAttack.resetToMax();
                }
            }
        }
    }
}

// Handle destroyed turrets
for(auto& turret : listTurrets) {
    if(turret.turretHealth <= 0) {
        for(auto& unit : listUnits) {
            unit->moveOk = true;
            unit->texture = TextureLoader::loadTexture(renderer, "S_Walk_Demo_2.bmp");
        }
    }
}


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

    for(auto& cannons : listCannons) {
    if(cannons.cannonHealth <= 0) {
        removeCannonsAtMousePosition(cannons.pos);
        continue; // Skip to next turret if this one is destroyed
    }

    for(auto& unit : listUnits) {
        if((cannons.pos - unit->pos).magnitude() <= 0.75f) {
            unit->moveOk = false;
            unit->timerAttack.countDown(dT);
            unit->texture = TextureLoader::loadTexture(renderer, "S_Attack.bmp");

            // Check if the unit's attack timer reached zero AND the unit is attacking the turret
            if(unit->timerAttack.timeSIsZero()) {
                if((cannons.pos - unit->pos).magnitude() <= 0.75f) {
                    cannons.cannonHealth--;
                    unit->timerAttack.resetToMax();
                }
            }
        }
    }
}

// Handle destroyed turrets
for(auto& cannons : listCannons) {
    if(cannons.cannonHealth <= 0) {
        for(auto& unit : listUnits) {
            unit->moveOk = true;
            unit->texture = TextureLoader::loadTexture(renderer, "S_Walk_Demo_2.bmp");
        }
    }
}
    //Update the balls
    updateCannonBall(dT);


//    updateSpawnUnitsIfRequired(renderer, dT);



}


void Game2P::updateUnits(float dT){
    auto it = listUnits.begin();
    while (it != listUnits.end()) {
        bool increment = true;

        if((*it) != nullptr) {
            (*it) -> update2P(dT, level2P, listUnits);

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




void Game2P::updateProjectiles(float dT) {
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

void Game2P::updateWave(float dT) {
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

void Game2P::updateCannonBall(float dT) {
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


void Game2P::draw(SDL_Renderer* renderer) {
    //Draw.
    //Set the draw color to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //Clear the screen.
    SDL_RenderClear(renderer);


    //Draw everything here.
    //Draw the level.
    level2P.draw(renderer, tileSize);
    for(auto& towerSelected : listMoneyTowers)
        towerSelected.draw(renderer, tileSize);


    for(auto& towerSelected : listElixirTowers)
        towerSelected.draw(renderer, tileSize);
    //Draw the enemy units.
    for (auto& unitSelected : listUnits)
        if (unitSelected != nullptr)
        unitSelected->draw2P(renderer, tileSize);

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
//    if (textureOverlay != nullptr && overlayVisible) {
//        int w = 0, h = 0;
//        SDL_QueryTexture(textureOverlay, NULL, NULL, &w, &h);
//        SDL_Rect rect = { 40, 40, w, h };
//        SDL_RenderCopy(renderer, textureOverlay, NULL, &rect);
//    }

    //Draw the choosing menu.
//    if (textureChoosingTower != nullptr) {
//        int w, h;
//        SDL_QueryTexture(textureChoosingTower, NULL, NULL, &w, &h);
//        SDL_Rect rect = {300, 350, w, h};
//        SDL_RenderCopy(renderer, textureChoosingTower, NULL, &rect);
//    }

    if (textureSelectionFrame1P != nullptr) {
        int w, h;
        SDL_QueryTexture(textureSelectionFrame1P, NULL, NULL, &w, &h);
        SDL_Rect rect = {towerCursor_x, towerCursor_y, w, h};
        SDL_RenderCopy(renderer, textureSelectionFrame1P, NULL, &rect);

    }

    if (textureSelectionFrame2P != nullptr) {
        int w, h;
        SDL_QueryTexture(textureSelectionFrame2P, NULL, NULL, &w, &h);
        SDL_Rect rect = {enemyCursor_x, enemyCursor_y, w, h};
        SDL_RenderCopy(renderer, textureSelectionFrame2P, NULL, &rect);


    }

    if (enemyFrame != nullptr) {
        int w, h;
        SDL_QueryTexture(enemyFrame, NULL, NULL, &w, &h);
        SDL_Rect rect = {600, -30, w, h};
        SDL_RenderCopy(renderer, enemyFrame, NULL, &rect);

    }

    if (towerFrame != nullptr) {
        int w, h;
        SDL_QueryTexture(towerFrame, NULL, NULL, &w, &h);
        SDL_Rect rect = {0, -20, w - 10, h - 10};
        SDL_RenderCopy(renderer, towerFrame, NULL, &rect);

    }

    if (towerSelect != nullptr) {
        int w, h;
        SDL_QueryTexture(towerSelect, NULL, NULL, &w, &h);
        SDL_Rect rect = {selectForTower_x, selectForTower_y, w - 175, h - 190};
        SDL_RenderCopy(renderer, towerSelect, NULL, &rect);

    }
    if (enemySelect != nullptr) {
        int w, h;
        SDL_QueryTexture(towerSelect, NULL, NULL, &w, &h);
        SDL_Rect rect = {selectForEnemy_x, selectForEnemy_y, w - 175, h - 190};
        SDL_RenderCopy(renderer, enemySelect, NULL, &rect);

    }

    renderGold(renderer, 200, 5, getGoldText(),
                     font32, Yellow);
    if(CastleHealth >= 0)
    renderGold(renderer, 350, 5, getHealthText(),
                   font32, Yellow);
 //   renderCenter(0, 3 + 32, getStrokeText(), font32, black);
//    renderGold(renderer, 550, 5, getWaveText(),
//                    font32, Yellow);

//    renderGold(renderer, 450, 20, getScoreText(),
//                   font32, {255, 255, 255});

    renderGold(renderer, 700, 5, getElixirText(),
                    font32, {53, 53, 84});
    //Send the image to the window.
    SDL_RenderPresent(renderer);
}


void Game2P::addUnit(SDL_Renderer* renderer, Vector2D posMouse, int healthMax) {
   /* listUnits.push_back(std::make_shared<Unit>(renderer, posMouse));
    for(int i = 0; i < listUnits.size(); i++){
        listUnits[i].healthMax++;
    }
    */
    if(elixir >= 20){
    elixir -= 20;
    auto newUnit = std::make_shared<Unit>(renderer, posMouse);
    newUnit->healthMax = healthMax;

    //std::cout << newUnit->healthMax << '\n';
    listUnits.push_back(newUnit);
    }
    else {};
}


void Game2P::addTurret(SDL_Renderer* renderer, Vector2D posMouse) {
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    //if(!level2P.noTower((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f))
        if(gold >= 100){
    gold -= 100;
    listTurrets.push_back(Turret(renderer, pos));
        }
    else {};

}

void Game2P::addDebuffer(SDL_Renderer* renderer, Vector2D posMouse) {
    //if(!level2P.noTower((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f)){
    if(gold >= 125){
        gold -= 125;
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    listDebuffers.push_back(Debuffer(renderer, pos));
    }
     else {};/////

}

void Game2P::addMoneyTower(SDL_Renderer* renderer, Vector2D posMouse) {
    if(gold >= 50){
        gold -= 50;
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    listMoneyTowers.push_back(MoneyTower(renderer, pos));
    }
     else {};

}

void Game2P::addElixirTower(SDL_Renderer* renderer, Vector2D posMouse) {
    if(elixir >= 50){
        elixir -= 50;
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    listElixirTowers.push_back(ElixirTower(renderer, pos));
    }
     else {};
}



void Game2P::addCannon(SDL_Renderer* renderer, Vector2D posMouse) {
    //if(!level2P.noTower((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f)){
    if(gold >= 175){
        gold -= 175;
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5f);
    listCannons.push_back(Cannon(renderer, pos));
    }
     else {};//////

}
void Game2P::removeTurretsAtMousePosition(Vector2D posMouse) {
   for (auto it = listTurrets.begin(); it != listTurrets.end();){
    if ((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
        it = listTurrets.erase(it);
    else
        it++;
   }
}

void Game2P::removeDebuffersAtMousePosition(Vector2D posMouse) {
    for (auto it = listDebuffers.begin(); it != listDebuffers.end();) {
        if((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
            it = listDebuffers.erase(it);
        else
            it++;
    }
}



void Game2P::removeCannonsAtMousePosition(Vector2D posMouse) {
    for (auto it = listCannons.begin(); it != listCannons.end();) {
        if((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
            it = listCannons.erase(it);
        else
            it++;
    }
}

void Game2P::removeMoneyTowersAtMousePosition(Vector2D posMouse) {
    for (auto it = listMoneyTowers.begin(); it != listMoneyTowers.end();) {
        if((*it).checkIfOnTile((int)posMouse.x, (int)posMouse.y))
            it = listMoneyTowers.erase(it);
        else
            it++;
    }
}

const char* Game2P::getGoldText(){
    std::string s = std::to_string(gold);
    s = "GOLD: " + s;
    return s.c_str();
}

const char* Game2P::getHealthText(){
    std::string s = std::to_string(CastleHealth);
    s = "HEALTH: " + s;
    return s.c_str();
}

const char* Game2P::getWaveText(){
    std::string s = std::to_string(totalRoundsSpawned - 2);
    s = "WAVE: " + s;
    return s.c_str();
}

const char* Game2P::getScoreText(){
    std::string s = std::to_string(currentScore);
    s = "SCORE: " + s;
    return s.c_str();
}

const char* Game2P::getElixirText(){
    std::string s = std::to_string(elixir);
    s = "ELIXIR: " + s;
    return s.c_str();
}

void Game2P::renderGold(SDL_Renderer* renderer, float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
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
