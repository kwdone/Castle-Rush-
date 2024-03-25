#include "Wave.h"

const float shockWave::speed = 10.0f, shockWave::size = 0.3f,
    shockWave::distanceTraveledMax = 20.0f;

shockWave::shockWave(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal) :
    pos(setPos),  directionNormal(setDirectionNormal)  {
        texture = TextureLoader::loadTexture(renderer, "Icicle.bmp");
    }

void shockWave::update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits) {
   //Move the projectile forward.
	float distanceMove = speed * dT;
	pos += directionNormal * distanceMove;

	distanceTraveled += distanceMove;
	if (distanceTraveled >= distanceTraveledMax)
		collisionOccurred = true;

	checkCollision(listUnits);
}

void shockWave::draw(SDL_Renderer* renderer, int tileSize) {
    if (renderer != nullptr) {
        //Draw the image at the debuffer's position.
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect rect = {
            (int)(pos.x * tileSize) - w / 2,
            (int)(pos.y * tileSize) - h / 2,
            w,
            h
        };
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

bool shockWave::getCollisionOccurred(){
    return collisionOccurred;
}

void shockWave::checkCollision(std::vector<std::shared_ptr<Unit>>& listUnits) {
//Check for a collision with any of the units.
	if (collisionOccurred == false) {
		//Check if this overlaps any of the enemy units or not.
		for (int count = 0; count < listUnits.size() && collisionOccurred == false; count++) {
			auto& unitSelected = listUnits[count];
			if (unitSelected != nullptr && unitSelected->checkOverlap(pos, size)) {
				unitSelected->removeHealth(0.5);
				unitSelected->reduceSpeed(0.4f);
				collisionOccurred = true;
			}/*
			else if(!collisionOccurred){
                unitSelected->returnSpeed();
			}
			*/

		}
	}

}




