#include "Unit.h"
#include "Game.h"
#include "GameMode2P.h"
#include <iostream>

//const float Unit::speed = 0.5f;
const float Unit::size = 0.48f;
int Unit::healthMax = 3;
int Unit::previousHealthMax = healthMax;
float Unit::difficulty = 1.0;


Unit::Unit(SDL_Renderer* renderer, Vector2D setPos) :
	pos(setPos), timerJustHurt(0.25f){
	texture = TextureLoader::loadTexture(renderer, "S_Walk_Demo_2.bmp");
	textureDead = TextureLoader::loadTexture(renderer, "S_Death.bmp");
}



void Unit::update(float dT, Level& level, std::vector<std::shared_ptr<Unit>>& listUnits) {
	prevPos = pos;
	timerJustHurt.countDown(dT);
	//Determine the distance to the target from the unit's current position.

	float distanceToTarget = (level.getTargetPos() - pos).magnitude();

    if(distanceToTarget < 0.5f){
        Game::CastleHealth--;
        healthCurrent = 0;
    } else {
	//Determine the distance to move this frame.
	float distanceMove = speedCurrent * dT;
	if (distanceMove > distanceToTarget)
		distanceMove = distanceToTarget;

	//Find the normal from the flow field.
	Vector2D directionNormal(level.getFlowNormal((int)pos.x, (int)pos.y));
	//If this reached the target tile, modify directionNormal to point to the target tile.
	if((int)pos.x == (int)level.getTargetPos().x && (int)pos.y == (int)level.getTargetPos().y)
        directionNormal = (level.getTargetPos() - pos).normalize();
	Vector2D posAdd = directionNormal * distanceMove;
    float distanceToTarget = (level.getTargetPos() - pos).magnitude();

    VerticalDirection direction = getVerticalDirection();

    switch (direction) {
    case UP:
        isVerticalMovementUp = true;
        isVerticalMovementDown = false;
        break;
    case DOWN:
        isVerticalMovementDown = true;
        isVerticalMovementUp = false;
        break;
    case NONE:
        isHorizontalMovement = true;
        break;
    }
    isHorizontalMovement = abs(directionNormal.x) > abs(directionNormal.y);
   // isVerticalMovement = abs(directionNormal.y) > abs(directionNormal.x);

//    if(isHorizontalMovement)
//	texture = TextureLoader::loadTexture(renderer, "S_Walk_Demo_2.bmp");
//    else if(isVerticalMovement)
//    texture = TextureLoader::loadTexture(renderer, "D_Walk.bmp");
	//Check if the new position would overlap any other units or not.
	//bool moveOk = true;
	moveOk = true;
	for (int count = 0; count < listUnits.size() && moveOk; count++) {
		auto& unitSelected = listUnits[count];
		if (&unitSelected != nullptr && unitSelected.get() != this &&
            unitSelected->checkOverlap(pos, size)){
			//They overlap so check and see if this unit is moving towards or away from the unit it overlaps.
			Vector2D directionToOther = (unitSelected->pos - pos);

			//level.teleportUnit(unitSelected->pos);
			//Ensure that they're not directly on top of each other.
			if (directionToOther.magnitude() > 0.01f) {
				//Check the angle between the units positions and the direction that this unit is traveling.
				//Ensure that this unit isn't moving directly towards the other unit (by checking the angle between).
				Vector2D normalToOther(directionToOther.normalize());
				float angleBtw = abs(normalToOther.angleBetween(directionNormal));
				/*if (angleBtw < 3.14159265359f / 4.0f)
					//Don't allow the move.
					moveOk = false;
					*/
                for (int count = 0; count < listUnits.size() && moveOk; count++) {
                auto& unit = listUnits[count];
                level.teleportUnit(unit->pos);
                }
			}
		}
	}

	if (moveOk && !Game::isPaused) {
		//Check if it needs to move in the x direction.  If so then check if the new x position, plus an amount of spacing
		//(to keep from moving too close to the wall) is within a wall or not and update the position as required.
		const float spacing = 0.5f;
		int x = (int)(pos.x + posAdd.x + copysign(spacing, posAdd.x));
		int y = (int)(pos.y);
		if (posAdd.x != 0.0f && level.isTileWall(x, y) == false)
			pos.x += posAdd.x;

		//Do the same for the y direction.
		x = (int)(pos.x);
		y = (int)(pos.y + posAdd.y + copysign(spacing, posAdd.y));
		if (posAdd.y != 0.0f && level.isTileWall(x, y) == false)
			pos.y += posAdd.y;

     //   level.teleportUnit(pos);

	if (!isAlive()){
        //timerDeathAnimation.countDown(dT);
        Game::gold += 10;
        Game::currentScore += difficulty * 50;
        if (chunkOrgeDead != nullptr)
            Mix_PlayChannel(-1, chunkOrgeDead, 0);

    }
}
}
}

void Unit::update2P(float dT, Level2P& level2P, std::vector<std::shared_ptr<Unit>>& listUnits) {
	timerJustHurt.countDown(dT);
	//Determine the distance to the target from the unit's current position.
	float distanceToTarget = (level2P.getTargetPos() - pos).magnitude();


    if(distanceToTarget < 0.5f){
        Game::CastleHealth--;
        Game2P::CastleHealth--;
        healthCurrent = 0;
    } else {
	//Determine the distance to move this frame.
	float distanceMove = speedCurrent * dT;
	if (distanceMove > distanceToTarget)
		distanceMove = distanceToTarget;

	//Find the normal from the flow field.
	Vector2D directionNormal(level2P.getFlowNormal((int)pos.x, (int)pos.y));
	//If this reached the target tile, modify directionNormal to point to the target tile.
	if((int)pos.x == (int)level2P.getTargetPos().x && (int)pos.y == (int)level2P.getTargetPos().y)
        directionNormal = (level2P.getTargetPos() - pos).normalize();
	Vector2D posAdd = directionNormal * distanceMove;

	//Check if the new position would overlap any other units or not.
	//bool moveOk = true;
	for (int count = 0; count < listUnits.size() && moveOk; count++) {
		auto& unitSelected = listUnits[count];
		if (&unitSelected != nullptr && unitSelected.get() != this &&
            unitSelected->checkOverlap(pos, size)){
			//They overlap so check and see if this unit is moving towards or away from the unit it overlaps.
			Vector2D directionToOther = (unitSelected->pos - pos);
			//Ensure that they're not directly on top of each other.
			if (directionToOther.magnitude() > 0.01f) {
				//Check the angle between the units positions and the direction that this unit is traveling.
				//Ensure that this unit isn't moving directly towards the other unit (by checking the angle between).
				Vector2D normalToOther(directionToOther.normalize());
				float angleBtw = abs(normalToOther.angleBetween(directionNormal));
				/*if (angleBtw < 3.14159265359f / 4.0f)
					//Don't allow the move.
					moveOk = false;
					*/

			}
		}
	}

	if (moveOk) {
		//Check if it needs to move in the x direction.  If so then check if the new x position, plus an amount of spacing
		//(to keep from moving too close to the wall) is within a wall or not and update the position as required.
		const float spacing = 0.5f;
		int x = (int)(pos.x + posAdd.x + copysign(spacing, posAdd.x));
		int y = (int)(pos.y);
		if (posAdd.x != 0.0f && level2P.isTileWall(x, y) == false)
			pos.x += posAdd.x;

		//Do the same for the y direction.
		x = (int)(pos.x);
		y = (int)(pos.y + posAdd.y + copysign(spacing, posAdd.y));
		if (posAdd.y != 0.0f && level2P.isTileWall(x, y) == false)
			pos.y += posAdd.y;


	if (!isAlive()){
        if (chunkOrgeDead != nullptr)
            Mix_PlayChannel(-1, chunkOrgeDead, 0);
	}

    }
}
}
////Render animation for the unit here

void Unit::draw(SDL_Renderer* renderer, int tileSize) {
    if(isHorizontalMovement)
	texture = TextureLoader::loadTexture(renderer, "S_Walk_Demo_2.bmp");
    else if(isVerticalMovementDown)
    texture = TextureLoader::loadTexture(renderer, "D_Walk.bmp");
    else if(isVerticalMovementUp)
    texture = TextureLoader::loadTexture(renderer, "U_Walk.bmp");

	if (renderer != nullptr) {
		//Set the texture's draw color to red if this unit was hurt recently
		if (timerJustHurt.timeSIsZero() == false)
            SDL_SetTextureColorMod(texture, 255, 0, 0);
        else
            SDL_SetTextureColorMod(texture, 255, 255, 255);

        if(!isAlive()){
            timerDeathAnimation.countDown(600.0f);
        const int DEATH_ANIMATION = 6;
		SDL_Rect SpriteOrge[DEATH_ANIMATION];
        int w, h;
        SDL_QueryTexture(textureDead, NULL, NULL, &w, &h);
        SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 12,
			(int)(pos.y * tileSize) - h / 2,
			w / 6,
			h };

		SpriteOrge[0].x = 0;
		SpriteOrge[0].y = 0;
		SpriteOrge[0].w = 96;
		SpriteOrge[0].h = 96;

		SpriteOrge[1].x = 96;
		SpriteOrge[1].y = 0;
		SpriteOrge[1].w = 96;
		SpriteOrge[1].h = 96;

		SpriteOrge[2].x = 192;
		SpriteOrge[2].y = 0;
		SpriteOrge[2].w = 96;
		SpriteOrge[2].h = 96;

		SpriteOrge[3].x = 288;
		SpriteOrge[3].y = 0;
		SpriteOrge[3].w = 96;
		SpriteOrge[3].h = 96;

		SpriteOrge[4].x = 384;
		SpriteOrge[4].y = 0;
		SpriteOrge[4].w = 96;
		SpriteOrge[4].h = 96;

		SpriteOrge[5].x = 480;
		SpriteOrge[5].y = 0;
		SpriteOrge[5].w = 96;
		SpriteOrge[5].h = 96;

		int frame = SDL_GetTicks() / 100;
		int currentFrame = frame % DEATH_ANIMATION;
		SDL_Rect currentSprite = SpriteOrge[currentFrame];

        SDL_RenderCopy(renderer, textureDead, &currentSprite, &rect);
		++frame;
		if(frame / 6 >= DEATH_ANIMATION)
            frame = 0;

        } else {
		const int WALKING_ANIMATION = 6;
		SDL_Rect SpriteOrge[WALKING_ANIMATION];
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 12,
			(int)(pos.y * tileSize) - h / 2,
			w / 6,
			h };

		SpriteOrge[0].x = 0;
		SpriteOrge[0].y = 0;
		SpriteOrge[0].w = 96;
		SpriteOrge[0].h = 96;

		SpriteOrge[1].x = 96;
		SpriteOrge[1].y = 0;
		SpriteOrge[1].w = 96;
		SpriteOrge[1].h = 96;

		SpriteOrge[2].x = 192;
		SpriteOrge[2].y = 0;
		SpriteOrge[2].w = 96;
		SpriteOrge[2].h = 96;

		SpriteOrge[3].x = 288;
		SpriteOrge[3].y = 0;
		SpriteOrge[3].w = 96;
		SpriteOrge[3].h = 96;

		SpriteOrge[4].x = 384;
		SpriteOrge[4].y = 0;
		SpriteOrge[4].w = 96;
		SpriteOrge[4].h = 96;

		SpriteOrge[5].x = 480;
		SpriteOrge[5].y = 0;
		SpriteOrge[5].w = 96;
		SpriteOrge[5].h = 96;

		int frame = SDL_GetTicks() / 100;
		int currentFrame = frame % WALKING_ANIMATION;
		SDL_Rect currentSprite = SpriteOrge[currentFrame];

        SDL_RenderCopy(renderer, texture, &currentSprite, &rect);
		++frame;
		if(frame / 4 >= WALKING_ANIMATION)
            frame = 0;
        }
	}
}

void Unit::draw2P(SDL_Renderer* renderer, int tileSize) {
	if (renderer != nullptr) {
		//Set the texture's draw color to red if this unit was hurt recently
		if (timerJustHurt.timeSIsZero() == false)
            SDL_SetTextureColorMod(texture, 255, 0, 0);
        else
            SDL_SetTextureColorMod(texture, 255, 255, 255);

		const int WALKING_ANIMATION = 6;
		SDL_Rect SpriteOrge[WALKING_ANIMATION];
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 12,
			(int)(pos.y * tileSize) - h / 2,
			w / 6,
			h };

		SpriteOrge[0].x = 0;
		SpriteOrge[0].y = 0;
		SpriteOrge[0].w = 96;
		SpriteOrge[0].h = 96;

		SpriteOrge[1].x = 96;
		SpriteOrge[1].y = 0;
		SpriteOrge[1].w = 96;
		SpriteOrge[1].h = 96;

		SpriteOrge[2].x = 192;
		SpriteOrge[2].y = 0;
		SpriteOrge[2].w = 96;
		SpriteOrge[2].h = 96;

		SpriteOrge[3].x = 288;
		SpriteOrge[3].y = 0;
		SpriteOrge[3].w = 96;
		SpriteOrge[3].h = 96;

		SpriteOrge[4].x = 384;
		SpriteOrge[4].y = 0;
		SpriteOrge[4].w = 96;
		SpriteOrge[4].h = 96;

		SpriteOrge[5].x = 480;
		SpriteOrge[5].y = 0;
		SpriteOrge[5].w = 96;
		SpriteOrge[5].h = 96;

		int frame = SDL_GetTicks() / 100;
		int currentFrame = frame % WALKING_ANIMATION;
		SDL_Rect currentSprite = SpriteOrge[currentFrame];

        SDL_RenderCopy(renderer, texture, &currentSprite, &rect);
		++frame;
		if(frame / 4 >= WALKING_ANIMATION)
            frame = 0;
	}
}

bool Unit::checkOverlap(Vector2D posOther, float sizeOther) {
	return (posOther - pos).magnitude() <= (sizeOther + size) / 2.0f;
}

//Added code
bool Unit::inWeaponRange(Vector2D posTurret, float weaponRange){
    return (posTurret - pos).magnitude() <= weaponRange;
}


bool Unit::isAlive() {
    return (healthCurrent > 0);
}

Vector2D Unit::getPos() {
    return pos;
    }

void Unit::removeHealth(float damage) {
    if(damage > 0) {
        healthCurrent -= damage;
        if(healthCurrent < 0)
            healthCurrent = 0;

            timerJustHurt.resetToMax();
    }
}

void Unit::reduceSpeed(float effect) {
    if(effect > 0) {
        if(speedCurrent > effect)
        speedCurrent -= effect;
        else if(speedCurrent <= effect)
            speedCurrent = 0.3f;

        timerJustHurt.resetToMax();
    }
}

void Unit::returnSpeed(){
    speedCurrent = 0.6f;

    //timerJustHurt.resetToMax();
}


Timer Unit::getTimerJustHurt(){
    return timerJustHurt;
}



VerticalDirection Unit::getVerticalDirection() {
    // Calculate the movement vector
    Vector2D movement = pos - prevPos;

    // Check the sign of the y-component of the movement vector
    if (movement.y > 0) {
        // Unit is moving downward
        return DOWN;
    } else if (movement.y < 0) {
        // Unit is moving upward
        return UP;
    } else {
        // Unit is not moving vertically
        return NONE;
    }
}
