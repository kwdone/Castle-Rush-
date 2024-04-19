#pragma once
#include <memory>
#include "SDL.h"
#include "MathAddon.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "SoundLoader.h"
#include "Unit.h"
#include "CannonBall.h"
#include "Timer.h"
//Note that many classes of turrets can be added in a level with different effect on the enemies.
//This is just the basic turret
class MoneyTower{

public:
    MoneyTower(SDL_Renderer* renderer, Vector2D setPos);
    void update(SDL_Renderer* renderer, float dT);
    void increaseMoney(SDL_Renderer* renderer);
    //void increaseElixir(SDL_Renderer* renderer);
    void draw(SDL_Renderer* renderer, int tileSize);
    bool checkIfOnTile(int x, int y);
    int moneyTowerHealth = 2;
    bool updateAngle(float dT);
    void drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
                               int offset, int tileSize);
    void drawWithoutAnimation(SDL_Renderer* renderer, SDL_Texture* textureSelected,
                               int offset, int tileSize);

    Vector2D pos;
    float angle;

    Timer timerWeapon;
    std::weak_ptr<Unit> unitTarget;

    SDL_Texture* textureMain = nullptr; // the texture of the turrer itself
   // SDL_Texture* textureEnemy = nullptr;

};

