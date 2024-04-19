#pragma once
#include <memory>
#include "SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Unit.h"
#include "MathAddon.h"



class Projectile
{
public:
    Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal, int damage);
    void update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits);
    void draw(SDL_Renderer* renderer, int tileSize);
    bool getCollisionOccurred();
    void updateAngle(float dT);
    static float upgradedDamage;
    void applyUpgradeDamage();
    int damageCaused = 1;
    static bool upgraded;
    void setUpgraded(bool flag);
    Vector2D pos;
private:
    void checkCollision(std::vector<std::shared_ptr<Unit>>& listUnits);

    Vector2D directionNormal;
    static const float speed, size, distanceTraveledMax;
    float distanceTraveled = 0.0f;
    float angle;
    SDL_Texture* texture = nullptr;
    static const float speedAngular;

    std::weak_ptr<Unit> unitTarget;


    bool collisionOccurred = false;
};
