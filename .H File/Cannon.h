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
class Cannon{

public:
    Cannon(SDL_Renderer* renderer, Vector2D setPos);
    void update(SDL_Renderer* renderer, float dT, std::vector<std::shared_ptr<Unit>>& listUnits,
                    std::vector<CannonBall>& listBalls);

    void draw(SDL_Renderer* renderer, int tileSize);
    bool checkIfOnTile(int x, int y);
    bool enemyFound();
    int cannonHealth = 5;
    Vector2D pos;
private:
    bool updateAngle(float dT);
    void shootProjectile(SDL_Renderer* renderer, std::vector<CannonBall>& listBalls);
    void drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
                               int offset, int tileSize);
    void drawWithoutAnimation(SDL_Renderer* renderer, SDL_Texture* textureSelected,
                               int offset, int tileSize);
    std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits);

   // Vector2D pos;
    float angle;
    static const float speedAngular, weaponRange;

    Timer timerWeapon;

    std::weak_ptr<Unit> unitTarget;

    SDL_Texture* textureMain = nullptr, // the texture of the turrer itself
            * textureHolder = nullptr;

    Mix_Chunk* mix_ChunkShoot = nullptr;

};

