#include "Projectile.h"


const float Projectile::speed = 10.0f, Projectile::size = 0.2f,
	Projectile::distanceTraveledMax = 20.0f;

const float Projectile::speedAngular = MathAddon::angleDegToRad(180.0f);


Projectile::Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal) :
	pos(setPos), directionNormal(setDirectionNormal), angle(0.0f) {
	texture = TextureLoader::loadTexture(renderer, "Arrow.bmp");

}



void Projectile::update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits) {
	//Move the projectile forward.
	float distanceMove = speed * dT;
	pos += directionNormal * distanceMove;

	distanceTraveled += distanceMove;
	if (distanceTraveled >= distanceTraveledMax)
		collisionOccurred = true;
	checkCollision(listUnits);

}



void Projectile::draw(SDL_Renderer* renderer, int tileSize) {
	if (renderer != nullptr) {
		//Draw the image at the projectile's position.
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 2,
			(int)(pos.y * tileSize) - h / 2,
			w,
			h };
		//SDL_RenderCopy(renderer, texture, NULL, &rect);
		SDL_RenderCopyEx(renderer, texture, NULL, &rect,
			MathAddon::angleRadToDeg(angle), NULL, SDL_FLIP_NONE);
	}
}



bool Projectile::getCollisionOccurred() {
	return collisionOccurred;
}



void Projectile::checkCollision(std::vector<std::shared_ptr<Unit>>& listUnits) {
	//Check for a collision with any of the units.
	if (collisionOccurred == false) {
		//Check if this overlaps any of the enemy units or not.
		for (int count = 0; count < listUnits.size() && collisionOccurred == false; count++) {
			auto& unitSelected = listUnits[count];
			if (unitSelected != nullptr && unitSelected->checkOverlap(pos, size)) {
				unitSelected->removeHealth(1);
				collisionOccurred = true;
			}
		}
	}
}




bool Projectile::updateAngle(float dT) {
	//Rotate towards the target unit if needed and output if it's pointing towards it or not.
	if (auto unitTargetSP = unitTarget.lock()) {
		//Determine the direction normal to the target.
		Vector2D directionNormalTarget = (unitTargetSP->getPos() - pos).normalize();

		//Determine the angle to the target.
		float angleToTarget = directionNormalTarget.angleBetween(Vector2D(angle));

		//Update the angle as required.
		//Determine the angle to move this frame.
		float angleMove = -copysign(speedAngular * dT, angleToTarget);
		if (abs(angleMove) > abs(angleToTarget)) {
			//It will point directly at it's target this frame.
			angle = directionNormalTarget.angle();
			return true;
		}
		else {
			//It won't reach it's target this frame.
			angle += angleMove;
		}
	}

	return false;
}
