
#pragma once
#include <memory>
#include <vector>
#include "SDL.h"
#include "Unit.h"
#include "MathAddon.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Wave.h"

//Note that many classes of turrets can be added in a level with different effect on the enemies.
//This is just the basic turret
class Debuffer{

public:
    Debuffer(SDL_Renderer* renderer, Vector2D setPos);
    void update(SDL_Renderer* renderer, float dT, std::vector<std::shared_ptr<Unit>>& listUnits,  std::vector<shockWave>& listWaves); //It will cause surrounding effect
    void draw(SDL_Renderer* renderer, int tileSize);
    bool checkIfOnTile(int x, int y);
    Vector2D getPos();
private:
    bool updateAngle(float dT);
    //void shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles);

    void shootWave(SDL_Renderer* renderer, std::vector<shockWave>& listWaves);
    void drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
                               int offset, int tileSize);
    std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits);

    Timer timerWeapon;

    Vector2D pos;
    float angle;
    static const float speedAngular, weaponRange;

   // Timer timerWeapon;

    std::weak_ptr<Unit> unitTarget;

    SDL_Texture* textureMain = nullptr, // the texture of the turrer itself
            * textureShadow = nullptr;

    Mix_Chunk* mix_ChunkFrozen = nullptr;
};
