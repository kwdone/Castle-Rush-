#include "Debuffer.h"
#include "Game.h"
const float Debuffer::speedAngular = MathAddon::angleDegToRad(180.0f), Debuffer::weaponRange = 3.0f;


Debuffer::Debuffer(SDL_Renderer* renderer, Vector2D setPos) :
    pos(setPos), angle(0.0f), timerWeapon(2.0f) {
        textureMain = TextureLoader::loadTexture(renderer, "Ice Tower.bmp");
        textureShadow = TextureLoader::loadTexture(renderer, "Turret Shadow.bmp");
        mix_ChunkFrozen = SoundLoader::loadSound("Frozen.ogg");
    }


void Debuffer::update(SDL_Renderer* renderer, float dT, std::vector<std::shared_ptr<Unit>>& listUnits, std::vector<shockWave>& listWaves ) {
   //Make the turret spin in circle
   /* angle += speedAngular * dT;
    if (angle > MathAddon::angleDegToRad(360.0f))
        angle -= MathAddon::angleDegToRad(360.0f);
        */
    //Check if a target has been found but is no longer alive or out of range.
    timerWeapon.countDown(dT);
    if (auto unitTargetSP = unitTarget.lock()) {
        if (unitTargetSP->isAlive() == false ||
            (unitTargetSP->getPos() - pos).magnitude() > weaponRange) {
                //Reset it
                unitTarget.reset();
            }
    }
    //Find an enemy unit.
    if (unitTarget.expired())
    unitTarget = findEnemyUnit(listUnits);

   if(updateAngle(dT) && !Game::isPaused)
    shootWave(renderer, listWaves);
    }



bool Debuffer::updateAngle(float dT) {
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

void Debuffer::shootWave(SDL_Renderer* renderer, std::vector<shockWave>& listWaves) {
	//Shoot a projectile towards the target unit if the weapon timer is ready.
	if (timerWeapon.timeSIsZero()) {
		listWaves.push_back(shockWave(renderer, pos, Vector2D(angle)));


     if (mix_ChunkFrozen != nullptr)
            Mix_PlayChannel(-1, mix_ChunkFrozen, 0);
		timerWeapon.resetToMax();
	}
}
void Debuffer::draw(SDL_Renderer* renderer, int tileSize) {
   // drawTextureWithOffset(renderer, textureShadow, 5, tileSize);
    drawTextureWithOffset(renderer, textureMain, 0, tileSize);

}

void Debuffer::drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
                                     int offset, int tileSize) {
                    if (renderer != nullptr && textureSelected != nullptr) {
                        //Draw the image at the tower's position and angle
                        int w, h;
                        SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
                        SDL_Rect rect = {
                            (int)(pos.x * tileSize) - w/2 + offset,
                            (int)(pos.y * tileSize) - h/2 + offset,
                            w,
                            h};
                        /*SDL_RenderCopyEx(renderer, textureSelected, NULL, &rect,
                                         MathAddon::angleRadToDeg(angle), NULL, SDL_FLIP_NONE);
                               */
                        SDL_RenderCopy(renderer, textureSelected, NULL, &rect);
                    }



                                     }

std::weak_ptr<Unit> Debuffer::findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits) {
	//Find the closest enemy unit to this turret.
	std::weak_ptr<Unit> closestUnit;
	float distanceClosest = 0.0f;

	//Loop through the entire list of units.
	for (auto& unitSelected : listUnits) {
		//Ensure that the selected unit exists.
		if (unitSelected != nullptr) {
			//Calculate the distance to the selected unit.
			float distanceCurrent = (pos - unitSelected->getPos()).magnitude();
			//Check if the unit is within range, and no closest unit has been found or the
			//selected unit is closer than the previous closest unit.
			if (distanceCurrent <= weaponRange &&
				(closestUnit.expired() || distanceCurrent < distanceClosest)) {
				//Then set the closest unit to the selected unit.
				closestUnit = unitSelected;
				distanceClosest = distanceCurrent;
			}
		}
	}

	return closestUnit;
}


bool Debuffer::checkIfOnTile(int x, int y) {
    return ((int)pos.x == x && (int)pos.y == y);
}


/*Vector2D getPos(){
    return pos;
}
*/
