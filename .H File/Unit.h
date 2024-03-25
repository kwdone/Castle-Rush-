#pragma once
#include <memory>
#include <vector>
#include "SDL.h"
#include "Vector2D.h"
#include "Level.h"
#include "TextureLoader.h"
#include "Timer.h"
#include "SoundLoader.h"

class Game;



class Unit
{
public:
	Unit(SDL_Renderer* renderer, Vector2D setPos);
	void update(float dT, Level& level, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkOverlap(Vector2D posOther, float sizeOther);
	//Added
	bool checkInAttackRange(Vector2D posTurret, float weaponRange);
	bool inWeaponRange(Vector2D posTurret, float weaponRange);
    bool isAlive();
    Vector2D getPos();
    void removeHealth(float damage);
    void reduceSpeed(float effect);
    void returnSpeed();
    Timer getTimerJustHurt();

private:
	Vector2D pos;
    const float speed = 0.6f;
	static const float size;
    float speedCurrent = speed;
    Mix_Chunk* chunkOrgeDead = SoundLoader::loadSound("Orge.ogg");

	SDL_Texture* texture = nullptr;
	//Under construction

	Timer timerJustHurt;

	const int healthMax = 3;
	float healthCurrent = healthMax;
};
