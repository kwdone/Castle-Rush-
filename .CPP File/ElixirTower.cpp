#include "ElixirTower.h"
#include "GameMode2P.h"



ElixirTower::ElixirTower(SDL_Renderer* renderer, Vector2D setPos) :
	pos(setPos), angle(0.0f), timerWeapon(3.0f) {
	textureMain = TextureLoader::loadTexture(renderer, "2P_MoneyTower.bmp");
}



void ElixirTower::update(SDL_Renderer* renderer, float dT) {
	//Update timer.
	timerWeapon.countDown(dT);

	increaseElixir(renderer);
}


void ElixirTower::increaseElixir(SDL_Renderer* renderer) {
	//Shoot a projectile towards the target unit if the weapon timer is ready.
	if (timerWeapon.timeSIsZero()) {
		Game2P::elixir += 10;

		timerWeapon.resetToMax();
	}
}



void ElixirTower::draw(SDL_Renderer* renderer, int tileSize) {
	//drawWithoutAnimation(renderer, textureMain, -5, tileSize);
	drawWithoutAnimation(renderer, textureMain, 0, tileSize);
}

////This might need fixing
//void Cannon::drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
//	int offset, int tileSize) {
//	if (renderer != nullptr && textureSelected != nullptr) {
//        //Render the animation of the turret
//		const int WALKING_ANIMATION = 5;
//		SDL_Rect Cannon[WALKING_ANIMATION];
//        int w, h;
//		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
//		SDL_Rect rect = {
//			(int)(pos.x * tileSize) - w / 10 + offset,
//			(int)(pos.y * tileSize) - h / 2 + offset,
//			w / 5,
//			h };
//
//		Cannon[0].x = 0;
//		Cannon[0].y = 0;
//		Cannon[0].w = 102;
//		Cannon[0].h = 84;
//
//		Cannon[1].x = 102;
//		Cannon[1].y = 0;
//		Cannon[1].w = 102;
//		Cannon[1].h = 84;
//
//		Cannon[2].x = 204;
//		Cannon[2].y = 0;
//		Cannon[2].w = 102;
//		Cannon[2].h = 84;
//
//
//		Cannon[3].x = 306;
//		Cannon[3].y = 0;
//		Cannon[3].w = 102;
//		Cannon[3].h = 84;
//
//		Cannon[4].x = 408;
//		Cannon[4].y = 0;
//        Cannon[4].w = 108;
//		Cannon[4].h = 84;
//
//
//		int frame = SDL_GetTicks() / 500;
//		int currentFrame = frame % WALKING_ANIMATION;
//		SDL_Rect currentSprite = Cannon[currentFrame];
//
//
//        //SDL_RenderCopy(renderer, texture, &currentSprite, &rect);
//		++frame;
//		if(frame / 4 >= WALKING_ANIMATION)
//            frame = 0;
//          /*
//		//Draw the image at the turret's position and angle and offset.
//		int w, h;
//		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
//		SDL_Rect rect = {
//			(int)(pos.x * tileSize) - w / 2 + offset,
//			(int)(pos.y * tileSize) - h / 2 + offset,
//			w,
//			h };
//			*/
//		SDL_RenderCopyEx(renderer, textureSelected, &currentSprite, &rect,
//			MathAddon::angleRadToDeg(angle), NULL, SDL_FLIP_NONE);
//	}
//}
//


void ElixirTower::drawWithoutAnimation(SDL_Renderer* renderer, SDL_Texture* textureSelected,
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



bool ElixirTower::checkIfOnTile(int x, int y) {
	return ((int)pos.x == x && (int)pos.y == y);
}

