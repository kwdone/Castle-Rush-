#pragma once
#include <memory>
#include <vector>
#include "SDL.h"
#include "Vector2D.h"
#include "Level.h"
#include "TextureLoader.h"
#include "Timer.h"
#include "SoundLoader.h"
#include "Level2P.h"

enum VerticalDirection {
    NONE,
    UP,
    DOWN
};
class Unit
{
public:
	Unit(SDL_Renderer* renderer, Vector2D setPos);
	void update(float dT, Level& level, std::vector<std::shared_ptr<Unit>>& listUnits);
	void update2P(float dT, Level2P& level2P, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	void draw2P(SDL_Renderer* renderer, int tileSize);
	bool checkOverlap(Vector2D posOther, float sizeOther);
	VerticalDirection getVerticalDirection();
	//Added
	 bool isHorizontalMovement;
	 bool isVerticalMovementUp;
	 bool isVerticalMovementDown;
	bool checkInAttackRange(Vector2D posTurret, float weaponRange);
	bool inWeaponRange(Vector2D posTurret, float weaponRange);
    bool isAlive();
    Vector2D getPos();
    void removeHealth(float damage);
    void reduceSpeed(float effect);
    void returnSpeed();
    Timer getTimerJustHurt();
    static int healthMax;
    //const int DEATH_ANIMATION_DURATION = 5.0f;
    static int previousHealthMax;
    static float difficulty;
    Vector2D pos;
    Vector2D prevPos;
    Timer timerAttack = 2.0f;
    Timer timerDeathAnimation = 10.0f;
    bool moveOk = true;
    SDL_Texture* texture = nullptr;
    SDL_Texture* textureDead = nullptr;
//    void attack(std::weak_ptr<Tower> towerWeakPtr);
 //   bool isTowerInRange(std::shared_ptr<Tower> tower);
private:
	//Vector2D pos;
    const float speed = 0.6f;
	static const float size;
    float speedCurrent = speed;
    Mix_Chunk* chunkOrgeDead = SoundLoader::loadSound("Orge.ogg");

	//SDL_Texture* texture = nullptr;
	//Under construction

	Timer timerJustHurt;

	//int healthMax = 3;
	float healthCurrent = healthMax;
};
