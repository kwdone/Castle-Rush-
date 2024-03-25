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
    Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal);
    void update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits);
    void draw(SDL_Renderer* renderer, int tileSize);
    bool getCollisionOccurred();
    bool updateAngle(float dT);

private:
    void checkCollision(std::vector<std::shared_ptr<Unit>>& listUnits);

    Vector2D pos, directionNormal;
    static const float speed, size, distanceTraveledMax;
    float distanceTraveled = 0.0f;
    float angle;
    SDL_Texture* texture = nullptr;
    static const float speedAngular;

    std::weak_ptr<Unit> unitTarget;


    bool collisionOccurred = false;
};
