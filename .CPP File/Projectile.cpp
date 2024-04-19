#include "Projectile.h"


const float Projectile::speed = 10.0f, Projectile::size = 0.2f,
	Projectile::distanceTraveledMax = 20.0f;
float Projectile::upgradedDamage = 3;
bool Projectile::upgraded = false;

const float Projectile::speedAngular = MathAddon::angleDegToRad(180.0f);


Projectile::Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal, int damage) :
	pos(setPos), directionNormal(setDirectionNormal), angle(3.0f) {
	texture = TextureLoader::loadTexture(renderer, "Arrow (1).bmp");
}


void Projectile::update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits) {
    // Update the angle to face the target unit
    updateAngle(dT);

    // Move the projectile forward.
    float distanceMove = speed * dT;
    pos += directionNormal * distanceMove;

    // Check for collisions
    distanceTraveled += distanceMove;
    if (distanceTraveled >= distanceTraveledMax)
        collisionOccurred = true;

    checkCollision(listUnits);
}

/*
void Projectile::update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits) {
    if (updateAngle(dT)) {
        // If the angle is updated, update the directionNormal as well
        directionNormal = Vector2D(cos(angle), sin(angle));
    }
	//Move the projectile forward.
	float distanceMove = speed * dT;
	pos += directionNormal * distanceMove;

	distanceTraveled += distanceMove;
	if (distanceTraveled >= distanceTraveledMax)
		collisionOccurred = true;
	checkCollision(listUnits);

}
*/
void Projectile::setUpgraded(bool flag) {
    upgraded = flag;
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
//                if(!upgraded)
//				unitSelected->removeHealth(damage);
//				else unitSelected->removeHealth(3);
				unitSelected->removeHealth(upgraded ? upgradedDamage : 1);
				collisionOccurred = true;
			}
		}
	}
}

/*

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
*/

void Projectile::updateAngle(float dT) {
    if (auto unitTargetSP = unitTarget.lock()) {
        // Determine the direction vector towards the target.
        Vector2D directionToTarget = (unitTargetSP->getPos() - pos).normalize();

        // Calculate the angle between the current direction and the direction towards the target.
        float targetAngle = atan2(directionToTarget.y, directionToTarget.x);
        float angleDiff = targetAngle - angle;

        // Normalize the angle difference to be within the range [-pi, pi].
        while (angleDiff > M_PI) angleDiff -= 2.0f * M_PI;
        while (angleDiff < -M_PI) angleDiff += 2.0f * M_PI;

        // Determine the angle to rotate this frame.
        float maxAngleRotate = speedAngular * dT;
        float angleRotate = std::min(maxAngleRotate, std::abs(angleDiff)) * (angleDiff > 0 ? 1 : -1);

        // Update the projectile's angle.
        angle += angleRotate;

        // Update the directionNormal.
        directionNormal = Vector2D(cos(angle), sin(angle));
    }
}
