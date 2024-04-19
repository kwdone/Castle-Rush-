#pragma once
#include <queue>
#include <vector>
#include <string>
#include "SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Timer.h"


class Level
{
    public:
    enum class TileType : char {
        empty,
        wall,
        enemySpawner,
        edge,
        portal,
        no_tower,
        bush_occupied,
        tree_occupied,
        greenPortal
        };
    private:
	static const unsigned char flowDistanceMax = 255;

	struct Tile {
		TileType type = TileType::empty;
		int flowDirectionX = 0;
		int flowDirectionY = 0;
		unsigned char flowDistance = flowDistanceMax;
	};


public:
	Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY);
	void draw(SDL_Renderer* renderer, int tileSize);
	void drawWithPortal(SDL_Renderer* renderer, int tileSize, int roundsSpawned);
	void drawWithAnimation(SDL_Renderer* renderer, int pos_x, int pos_y, SDL_Texture* object);
	void drawObject(SDL_Renderer* renderer, int pos_x, int pos_y, SDL_Texture* object);
    void teleportUnit(Vector2D& unitPosition);
    void teleportUnitGreen(Vector2D& unitPosition);
    Vector2D getRandomEnemySpawnerLocation();
    bool isTilePath(int x, int y);
	bool isTileWall(int x, int y);
	bool isBushOccupied(int x, int y);
	bool isTreeOccupied(int x, int y);
	bool noTower(int x, int y);
	void setTileWall(int x, int y, bool setWall);
	Vector2D getTargetPos();
	Vector2D getFlowNormal(int x, int y);
    static int chooseLevel;
    static bool openPortal;
    TileType getTileType(int x, int y);
private:
    Timer timerForLevel3;
    SDL_Texture* texturePortal;
    SDL_Texture* textureGreenPortal;
    Vector2D portalDestinations[4];
    //TileType getTileType(int x, int y);
    void setTileType(int x, int y, TileType tileType);
	void drawTile(SDL_Renderer* renderer, int x, int y, int tileSize);
	void calculateFlowField();
	void calculateDistances();
	void calculateFlowDirections();
    void load_map(std::vector<int> Types);
///////
	std::vector<Tile> listTiles;

    //std::vector<int> Types = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 ,0 ,0 ,0 ,0};
    const int tileCountX, tileCountY;
	int targetX = 0, targetY = 0;

    std::vector<int> Types1 = {0, 1, 1, 1, 1, 4, 4, 4, 4, 2, 1, 1, 1, 1, 0,
                              0, 0, 0, 0, 1, 4, 4, 4, 2, 1, 3, 1, 1, 0, 0,
                              0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0,
                              1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
                              1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
                              1, 0, 1, 0, 4, 4, 1, 0, 4, 4, 1, 0, 1, 0, 1,
                              1, 0, 0, 0, 4, 4, 4, 1, 4, 4, 1, 0, 0, 0, 1,
                              1, 0, 1, 4, 4, 4, 4, 4, 4, 3, 1, 0, 1, 0, 1,
                              0, 0, 1, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 0, 0};

    std::vector<int> Types2 = {0, 0, 1, 1, 1, 4, 4, 4, 4, 4, 1, 1, 1, 1, 0,
                              1, 0, 0, 0, 0, 1, 4, 4, 4, 1, 0, 0, 0, 0, 0,
                              1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
                              1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1,
                              1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
                              1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
                              1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
                              0, 0, 1, 1, 4, 4, 4, 4, 4, 4, 4, 1, 1, 0, 0,
                              0, 0, 1, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 0, 1};

    std::vector<int> Types3 = {0, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 0,
                              0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
                              0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0,
                              0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0,
                              0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0,
                              0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0,
                              0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0,
                              0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0,
                              0, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 1};
	SDL_Texture* textureTileWall = nullptr,
		*textureTileTarget = nullptr,
		*textureTileEnemySpawner = nullptr,
		*textureTileEmpty = nullptr,
		*textureTileArrowUp = nullptr,
		*textureTileArrowUpRight = nullptr,
		*textureTileArrowRight = nullptr,
		*textureTileArrowDownRight = nullptr,
		*textureTileArrowDown = nullptr,
		*textureTileArrowDownLeft = nullptr,
		*textureTileArrowLeft = nullptr,
		*textureTileArrowUpLeft = nullptr,
		*textureBush = nullptr,
		*textureTree = nullptr,
		*textureTreeShadow = nullptr,
		*textureRock = nullptr,
		*textureRuin = nullptr,
		*textureRuinStair = nullptr,
		*textureCamp3 = nullptr,
		*textureCamp4 = nullptr,
		*textureFlower = nullptr,
		*textureGrass = nullptr,
		*textureCampFire = nullptr,
		*textureFlag = nullptr;
};
