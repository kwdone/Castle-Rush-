#pragma once
#include <queue>
#include <vector>
#include <string>
#include "SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include <SDL_image.h>


class Level2P
{
private:
    enum class TileType : char {
        empty,
        wall,
        enemySpawner,
        enemyLane,
        walkable
        };
	static const unsigned char flowDistanceMax = 255;

	struct Tile {
		TileType type = TileType::empty;
		int flowDirectionX = 0;
		int flowDirectionY = 0;
		unsigned char flowDistance = flowDistanceMax;
	};


public:
	Level2P(SDL_Renderer* renderer, int setTileCountX, int setTileCountY);
	void draw(SDL_Renderer* renderer, int tileSize);
	void drawObject(SDL_Renderer* renderer, int pos_x, int pos_y, SDL_Texture* object);

    //Vector2D getRandomEnemySpawnerLocation();
    bool isTilePath(int x, int y);
	bool isTileWall(int x, int y);
	//bool isBushOccupied(int x, int y);
	//bool isTreeOccupied(int x, int y);
	//bool noTower(int x, int y);
	void setTileWall(int x, int y, bool setWall);
	Vector2D getTargetPos();
	Vector2D getFlowNormal(int x, int y);


private:
    TileType getTileType(int x, int y);
    void setTileType(int x, int y, TileType tileType);
	void drawTile(SDL_Renderer* renderer, int x, int y, int tileSize);
	void calculateFlowField();
	void calculateDistances();
	void calculateFlowDirections();
    void load_map();
///////
	std::vector<Tile> listTiles;

    //std::vector<int> Types = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 ,0 ,0 ,0 ,0};
	const int tileCountX, tileCountY;

	const int targetX = 0, targetY = 0;

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
		*textureTileWallDarker = nullptr,
		*textureRuinStair = nullptr;
};

