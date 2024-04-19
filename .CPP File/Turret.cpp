#include "Turret.h"
#include "Game.h"

const float Turret::speedAngular = MathAddon::angleDegToRad(180.0f), Turret::weaponRange = 3.5f;




Turret::Turret(SDL_Renderer* renderer, Vector2D setPos) :
	pos(setPos), angle(0.0f), timerWeapon(1.0f) {
	textureMain = TextureLoader::loadTexture(renderer, "Golden Ballista.bmp");
	textureHolder = TextureLoader::loadTexture(renderer, "Inanimate Ballista.bmp");
	mix_ChunkShoot = SoundLoader::loadSound("Arrow Shoot.ogg");

}



void Turret::update(SDL_Renderer* renderer, float dT, std::vector<std::shared_ptr<Unit>>& listUnits,
	std::vector<Projectile>& listProjectiles) {
	//Update timer.
	timerWeapon.countDown(dT);

	//Check if a target has been found but is no longer alive or is out of weapon range.
	if (auto unitTargetSP = unitTarget.lock()) {
		if (unitTargetSP->isAlive() == false ||
			(unitTargetSP->getPos() - pos).magnitude() > weaponRange) {
			//Then reset it.
			unitTarget.reset();
		}
	}

	//Find a target if needed.
	if (unitTarget.expired() && !Game::isPaused)
		unitTarget = findEnemyUnit(listUnits);

	//Update the angle and shoot a projectile if needed.
	if (updateAngle(dT) && !Game::isPaused)
		shootProjectile(renderer, listProjectiles);
}


bool Turret::updateAngle(float dT) {
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


void Turret::shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles) {
	//Shoot a projectile towards the target unit if the weapon timer is ready.
	if (timerWeapon.timeSIsZero()) {
        int damageToUse = upgraded ? upgradedDamage : baseDamage;
		listProjectiles.push_back(Projectile(renderer, pos, Vector2D(angle), damageToUse));

//        if(upgraded)
//            Projectile::upgraded = true;

        //Play the shoot sound.
        if (mix_ChunkShoot != nullptr)
            Mix_PlayChannel(-1, mix_ChunkShoot, 0);

		timerWeapon.resetToMax();
	}
}



void Turret::draw(SDL_Renderer* renderer, int tileSize) {
	//drawWithoutAnimation(renderer, textureMain, -5, tileSize);
	if(enemyFound() && !Game::isPaused)
	drawTextureWithOffset(renderer, textureMain, 0, tileSize);
	else
	drawWithoutAnimation(renderer, textureHolder, 0, tileSize);
}


void Turret::drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
	int offset, int tileSize) {
	if (renderer != nullptr && textureSelected != nullptr) {
        //Render the animation of the turret
		const int WALKING_ANIMATION = 6;
		SDL_Rect Ballista[WALKING_ANIMATION];
        int w, h;
		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
		SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 12 + offset,
			(int)(pos.y * tileSize) - h / 2 + offset,
			w / 6,
			h };

		Ballista[0].x = 0;
		Ballista[0].y = 0;
		Ballista[0].w = 68;
		Ballista[0].h = 65;

		Ballista[1].x = 68;
		Ballista[1].y = 0;
		Ballista[1].w = 68;
		Ballista[1].h = 65;

		Ballista[2].x = 136;
		Ballista[2].y = 0;
		Ballista[2].w = 68;
		Ballista[2].h = 65;

		Ballista[3].x = 204;
		Ballista[3].y = 0;
		Ballista[3].w = 68;
		Ballista[3].h = 65;

		Ballista[4].x = 273;
		Ballista[4].y = 0;
		Ballista[4].w = 65;
		Ballista[4].h = 65;

		Ballista[5].x = 338;
		Ballista[5].y = 0;
		Ballista[5].w = 65;
		Ballista[5].h = 65;

		int frame = SDL_GetTicks() / 150;
		int currentFrame = frame % WALKING_ANIMATION;
		SDL_Rect currentSprite = Ballista[currentFrame];


        //SDL_RenderCopy(renderer, texture, &currentSprite, &rect);
		++frame;
		if(frame / 4 >= WALKING_ANIMATION)
            frame = 0;
          /*
		//Draw the image at the turret's position and angle and offset.
		int w, h;
		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
		SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 2 + offset,
			(int)(pos.y * tileSize) - h / 2 + offset,
			w,
			h };
			*/
		SDL_RenderCopyEx(renderer, textureSelected, &currentSprite, &rect,
			MathAddon::angleRadToDeg(angle), NULL, SDL_FLIP_NONE);
	}
}



void Turret::drawWithoutAnimation(SDL_Renderer* renderer, SDL_Texture* textureSelected,
	int offset, int tileSize) {
	if (renderer != nullptr && textureSelected != nullptr) {
		//Draw the image at the turret's position and angle and offset.
		int w, h;
		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
		SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 2 + offset,
			(int)(pos.y * tileSize) - h / 2 + offset,
			w,
			h };
		SDL_RenderCopyEx(renderer, textureSelected, NULL, &rect,
			MathAddon::angleRadToDeg(angle), NULL, SDL_FLIP_NONE);
	}
}



bool Turret::checkIfOnTile(int x, int y) {
	return ((int)pos.x == x && (int)pos.y == y);
}

bool Turret::enemyFound(){
    return !unitTarget.expired();
}

std::weak_ptr<Unit> Turret::findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits) {
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

/*
void Turret::upgradeTurret(std::vector<Turret>& listTurrets, const Vector2D& position, std::vector<Projectile>& listProjectiles) {
    for (auto& turret : listTurrets) {
        if (turret.checkIfOnTile(position.x, position.y)) {
            turret.upgraded = true;
            turret.timerWeapon = 0.4f;  // Modify the actual turret's timerWeapon
            break;  // Exit loop after upgrading the first matching turret
        }
    }

}
*/void Turret::upgradeTurret(std::vector<Turret>& listTurrets, const Vector2D& position, std::vector<Projectile>& listProjectiles, SDL_Renderer* renderer) {
    // Check if the current turret matches the upgrade position
    if (checkIfOnTile(position.x, position.y)) {
        // Check if the turret is already upgraded
        if (!upgraded) {
            timerWeapon = 0.5f;  // Modify the actual turret's timerWeapon
            upgraded = true;

            // Set upgraded flag only for projectiles from this turret
            for (auto& projectile : listProjectiles) {
                // Assuming 1.0f as close enough to consider it's from this turret
                if ((projectile.pos - position).magnitude() < 1.0f) {
                    projectile.setUpgraded(true);
                }
            }

            // Mark the turret in the list as upgraded
            for (auto& turret : listTurrets) {
                if (turret.checkIfOnTile(position.x, position.y)) {
                    turret.upgraded = true;
                    turret.textureMain = TextureLoader::loadTexture(renderer, "Fire_Ballista.bmp");
                    turret.textureHolder = TextureLoader::loadTexture(renderer, "Fire_Ballista_Inanimate.bmp");
                    break; // Exit loop after upgrading the first matching turret
                }
            }
        }
    }
}
