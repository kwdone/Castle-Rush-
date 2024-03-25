#include "Cannon.h"


const float Cannon::speedAngular = MathAddon::angleDegToRad(180.0f), Cannon::weaponRange = 3.0f;




Cannon::Cannon(SDL_Renderer* renderer, Vector2D setPos) :
	pos(setPos), angle(0.0f), timerWeapon(2.5f) {
	textureMain = TextureLoader::loadTexture(renderer, "Starry Cannon.bmp");
	textureHolder = TextureLoader::loadTexture(renderer, "Inanimate Cannon.bmp");
	mix_ChunkShoot = SoundLoader::loadSound("Explosion.ogg");
}



void Cannon::update(SDL_Renderer* renderer, float dT, std::vector<std::shared_ptr<Unit>>& listUnits,
	std::vector<CannonBall>& listBalls) {
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
	if (unitTarget.expired())
		unitTarget = findEnemyUnit(listUnits);

	//Update the angle and shoot a projectile if needed.
	if (updateAngle(dT))
		shootProjectile(renderer, listBalls);
}


bool Cannon::updateAngle(float dT) {
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


void Cannon::shootProjectile(SDL_Renderer* renderer, std::vector<CannonBall>& listBalls) {
	//Shoot a projectile towards the target unit if the weapon timer is ready.
	if (timerWeapon.timeSIsZero()) {
		listBalls.push_back(CannonBall(renderer, pos, Vector2D(angle)));

        //Play the shoot sound.
        if (mix_ChunkShoot != nullptr)
            Mix_PlayChannel(-1, mix_ChunkShoot, 0);

		timerWeapon.resetToMax();
	}
}



void Cannon::draw(SDL_Renderer* renderer, int tileSize) {
	//drawWithoutAnimation(renderer, textureMain, -5, tileSize);
	if(enemyFound())
	drawTextureWithOffset(renderer, textureMain, 0, tileSize);
	else
	drawWithoutAnimation(renderer, textureHolder, 0, tileSize);
}

//This might need fixing
void Cannon::drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
	int offset, int tileSize) {
	if (renderer != nullptr && textureSelected != nullptr) {
        //Render the animation of the turret
		const int WALKING_ANIMATION = 5;
		SDL_Rect Cannon[WALKING_ANIMATION];
        int w, h;
		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
		SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 10 + offset,
			(int)(pos.y * tileSize) - h / 2 + offset,
			w / 5,
			h };

		Cannon[0].x = 0;
		Cannon[0].y = 0;
		Cannon[0].w = 102;
		Cannon[0].h = 84;

		Cannon[1].x = 102;
		Cannon[1].y = 0;
		Cannon[1].w = 102;
		Cannon[1].h = 84;

		Cannon[2].x = 204;
		Cannon[2].y = 0;
		Cannon[2].w = 102;
		Cannon[2].h = 84;


		Cannon[3].x = 306;
		Cannon[3].y = 0;
		Cannon[3].w = 102;
		Cannon[3].h = 84;

		Cannon[4].x = 408;
		Cannon[4].y = 0;
        Cannon[4].w = 108;
		Cannon[4].h = 84;


		int frame = SDL_GetTicks() / 500;
		int currentFrame = frame % WALKING_ANIMATION;
		SDL_Rect currentSprite = Cannon[currentFrame];


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



void Cannon::drawWithoutAnimation(SDL_Renderer* renderer, SDL_Texture* textureSelected,
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



bool Cannon::checkIfOnTile(int x, int y) {
	return ((int)pos.x == x && (int)pos.y == y);
}

bool Cannon::enemyFound(){
    return !unitTarget.expired();
}

std::weak_ptr<Unit> Cannon::findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits) {
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
