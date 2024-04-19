#pragma once
#include <vector>
#include <chrono>
#include <memory>
#include "SDL.h"
#include <SDL_ttf.h>
#include "Unit.h"
#include "Turret.h"
#include "Debuffer.h"
#include "Projectile.h"
#include "Level.h"
#include "Timer.h"
#include "Wave.h"
#include "Cannon.h"
#include "CannonBall.h"
#include "SoundLoader.h"


class Game
{
private:
	enum class PlacementMode {
	    debuffer,
		wall,
		turret,
		cannon
	} placementModeCurrent;


public:
	Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight);
	~Game();

    //int showmenu(SDL_Surface* screen, SDL_Renderer* renderer, TTF_Font* font);

	TTF_Font* font = TTF_OpenFont("LoveDays.ttf", 20);
	static int gold;
	static int CastleHealth;
    static int currentScore;
    static int totalRoundsSpawned;
    static bool isPaused;
    static bool backToSelectionScreen;
    bool noTower(int x, int y);
    static bool win, lose;
    static int spawnUnitCount;
private:
	void processEvents(SDL_Renderer* renderer, bool& running);
	void update(SDL_Renderer* renderer, float dT);
	void updateUnits(float dT);
	void updateProjectiles(float dT);
	void updateWave(float dT);
	void updateCannonBall(float dT);
	void updateSpawnUnitsIfRequired(SDL_Renderer* renderer, float dT);
	void draw(SDL_Renderer* renderer);
	void addUnit(SDL_Renderer* renderer, Vector2D posMouse, int healthMax);
	void addTurret(SDL_Renderer* renderer, Vector2D posMouse);
	void addDebuffer(SDL_Renderer* renderer, Vector2D posMouse);
	void addCannon(SDL_Renderer* renderer, Vector2D posMouse);
	void removeTurretsAtMousePosition(Vector2D posMouse);
	void removeCannonsAtMousePosition(Vector2D posMouse);
	void removeDebuffersAtMousePosition(Vector2D posMouse);
	void pauseGame(SDL_Renderer* renderer);
	void PlayAgain(std::vector<Turret> &listTurrets, std::vector<Cannon> &listCannons, std::vector<Debuffer> &listDebuffers,
                     std::vector<std::shared_ptr<Unit>> &listUnits);
    //int showMenu(SDL_Surface* screen, SDL_Renderer* renderer, TTF_Font* font);

	int mouseDownStatus = 0;

    //WARNING FOR CONSTRUCTION
	const int tileSize = 64;
	Level level;

	std::vector<std::shared_ptr<Unit>> listUnits;
	std::vector<Turret> listTurrets;
	std::vector<Projectile> listProjectiles;
	std::vector<Debuffer> listDebuffers;
	std::vector<shockWave> listWaves;
	std::vector<Cannon> listCannons;
	std::vector<CannonBall> listBalls;

	SDL_Texture* textureOverlay = nullptr;
	SDL_Texture* textureChoosingTower = nullptr;
	SDL_Texture* texturePause = nullptr;
	SDL_Texture* textureSetting = nullptr;
	SDL_Texture* textureDead = nullptr;
	SDL_Texture* texturePauseMenu = nullptr;
	bool overlayVisible = false;
	void renderGold(SDL_Renderer* renderer, float p_x, float p_y, const char* p_text,
             TTF_Font* font, SDL_Color textColor);

	const char* getGoldText();
	const char* getHealthText();
	const char* getWaveText();
	const char* getScoreText();

	Timer spawnTimer, roundTimer;
	//MODIFIED
	int firstWaveSpawnUnitCount = 10;
	SDL_Color Yellow = {255 ,199 , 0};
	TTF_Font* font32 = TTF_OpenFont("C:/Windows/LoveDays.ttf", 16);

	Mix_Chunk* mix_ChunkSpawnUnit = nullptr;
	//int totalRoundsSpawned = 3;
	int healthIncrementedPerRound = 1;
    //int gold = 100;
	//TTF_Font* font = TTF_OpenFont("LoveDays.ttf", 20);
};
