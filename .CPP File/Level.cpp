#include "Level.h"
#include "Game.h"
int Level::chooseLevel = 0;
bool Level::openPortal = false;

Level::Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY) :
    tileCountX(setTileCountX), tileCountY(setTileCountY),
    targetX(setTileCountX / 2), targetY(setTileCountY / 2), timerForLevel3(15.0f) {
    textureTileWall = TextureLoader::loadTexture(renderer, "FieldsTile_38.bmp");
    textureTileTarget = TextureLoader::loadTexture(renderer, "New Castle.bmp");
    textureTileEnemySpawner = TextureLoader::loadTexture(renderer, "Enemy Gate.bmp");
    texturePortal = TextureLoader::loadTexture(renderer, "Portal.bmp");
    textureTileEmpty = TextureLoader::loadTexture(renderer, "FieldsTile_35.bmp");
    textureTileArrowUp = TextureLoader::loadTexture(renderer, "Tile Arrow Up.bmp");
    textureTileArrowUpRight = TextureLoader::loadTexture(renderer, "Tile Arrow Up Right.bmp");
    textureTileArrowRight = TextureLoader::loadTexture(renderer, "Tile Arrow Right.bmp");
    textureTileArrowDownRight = TextureLoader::loadTexture(renderer, "Tile Arrow Down Right.bmp");
    textureTileArrowDown = TextureLoader::loadTexture(renderer, "Tile Arrow Down.bmp");
    textureTileArrowDownLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Down Left.bmp");
    textureTileArrowLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Left.bmp");
    textureTileArrowUpLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Up Left.bmp");
    textureBush = TextureLoader::loadTexture(renderer, "Bush.bmp");
    textureTree = TextureLoader::loadTexture(renderer, "Tree.bmp");
    textureTreeShadow = TextureLoader::loadTexture(renderer, "TreeShadow.bmp");
    textureRock = TextureLoader::loadTexture(renderer, "Rock.bmp");
    textureRuin = TextureLoader::loadTexture(renderer, "Brown_ruins1.bmp");
    textureRuinStair = TextureLoader::loadTexture(renderer, "Brown_ruins2.bmp");
    textureGreenPortal = TextureLoader::loadTexture(renderer, "Green Portal.bmp");
    textureGrass = TextureLoader::loadTexture(renderer, "3.bmp");
    textureFlower = TextureLoader::loadTexture(renderer, "4.bmp");
    textureCamp3 = TextureLoader::loadTexture(renderer, "Camp3.bmp");
    textureCamp4 = TextureLoader::loadTexture(renderer, "1.bmp");
    textureCampFire = TextureLoader::loadTexture(renderer, "CampFire.bmp");
    size_t listTilesSize = (size_t)tileCountX * tileCountY;
    listTiles.assign(listTilesSize, Tile{});
    int xMax = tileCountX - 1;
    int yMax = tileCountY - 1;

    if(chooseLevel == 1){
    load_map(Types1);
    }
    else if(chooseLevel == 2){
    load_map(Types2);
    targetY = setTileCountY / 2 - 2;
    }
    else if(chooseLevel == 3){
    load_map(Types3);
   // setTileType(xMax, yMax, TileType::wall);
    targetX = 14;
    targetY = 2;
    }


    //SPECIFIC PLACE TO MAKE CHANGE
    //Add an enemy spawner at each corner. Remember this part because in the future I would like to change it
    //These indicate the four corners. It can be changed on levels
    setTileType(0, 0, TileType::enemySpawner);
    setTileType(xMax, 0, TileType::enemySpawner);
    setTileType(0, yMax, TileType::enemySpawner);
    setTileType(xMax, yMax, TileType::enemySpawner);
    if(chooseLevel == 3){
        setTileType(xMax, yMax, TileType::empty);
        setTileType(xMax, 0, TileType::wall);
    }

    if(chooseLevel == 2){
    portalDestinations[0] = Vector2D(2, 1);
    portalDestinations[1] = Vector2D(5, 5);
    setTileType(2, 1, TileType::portal);
    setTileType(5, 5, TileType::portal);
    }

    calculateFlowField();

}

void Level::draw(SDL_Renderer* renderer, int tileSize) {


    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (isTilePath(x, y)) {
                int w, h;
                SDL_QueryTexture(textureTileEmpty, NULL, NULL, &w, &h);
                SDL_Rect rect = {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2 - h / 2,
                    w,
                    h
                };
                SDL_RenderCopy(renderer, textureTileEmpty, NULL, &rect);
            }
        }
    }

/*
    //Draw the arrow (and empty) tiles. Uncomment to Draw the flow field
    for (int count = 0; count < listTiles.size(); count++)
        drawTile(renderer, (count % tileCountX), (count / tileCountX), tileSize);

        */
    //Draw the enemy spawner tiles.
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (getTileType(x, y) == TileType::enemySpawner) {
                SDL_Rect rect1 = { x * tileSize, y * tileSize, tileSize, tileSize };
                SDL_RenderCopy(renderer, textureTileEmpty, NULL, &rect1);
                SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
                SDL_RenderCopy(renderer, textureTileEnemySpawner, NULL, &rect);
            }
        }
    }

    //Draw the target tile.
    if (textureTileTarget != nullptr) {
        SDL_Rect rect = { targetX * tileSize, targetY * tileSize, tileSize , tileSize + 42};
        SDL_RenderCopy(renderer, textureTileTarget, NULL, &rect);
    }

    //Draw the wall tiles.
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (isTileWall(x, y)) {
                int w, h;
                SDL_QueryTexture(textureTileWall, NULL, NULL, &w, &h);
                SDL_Rect rect = {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2 - h / 2,
                    w,
                    h
                };
                SDL_RenderCopy(renderer, textureTileWall, NULL, &rect);
            }
        }
    }


    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (getTileType(x, y) == TileType::portal || getTileType(x, y) == TileType::greenPortal) {
                int w, h;
                SDL_QueryTexture(textureTileEmpty, NULL, NULL, &w, &h);
                SDL_Rect rect = {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2 - h / 2,
                    w,
                    h
                };
                SDL_RenderCopy(renderer, textureTileEmpty, NULL, &rect);
            }
        }
    }

    //Draw tiles not allowing towers
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (noTower(x, y)) {
                int w, h;
                SDL_QueryTexture(textureTileWall, NULL, NULL, &w, &h);
                SDL_Rect rect = {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2 - h / 2,
                    w,
                    h
                };
                SDL_RenderCopy(renderer, textureTileWall, NULL, &rect);
            }
        }
    }

    //Draw decor
    //Draw bushes
    if(chooseLevel == 1){
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (isBushOccupied(x, y)) {
                    int w, h;
                    SDL_QueryTexture(textureTileWall, NULL, NULL, &w, &h);
                SDL_Rect rect2 = {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2 - h / 2,
                    w,
                    h
                };
                SDL_RenderCopy(renderer, textureTileWall, NULL, &rect2);
                    SDL_QueryTexture(textureBush, NULL, NULL, &w, &h);
                    SDL_Rect rect = {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2,
                    w,
                    h};
                    SDL_RenderCopy(renderer, textureBush, NULL, &rect);
            }
        }
    }

    //Draw trees
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (isTreeOccupied(x, y)) {
                    int w, h;
                    SDL_QueryTexture(textureTileWall, NULL, NULL, &w, &h);
                SDL_Rect rect2 = {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2 - h / 2,
                    w,
                    h
                };
                SDL_RenderCopy(renderer, textureTileWall, NULL, &rect2);
                    SDL_QueryTexture(textureTree, NULL, NULL, &w, &h);
                    SDL_Rect rect = {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2,
                    w,
                    h};
                    SDL_RenderCopy(renderer, textureTree, NULL, &rect);
            }
        }
    }
    int shadow_w, shadow_h;
    SDL_QueryTexture(textureTreeShadow, NULL, NULL, &shadow_w, &shadow_h);
    SDL_Rect tree_shadow1 = {
        270,
        365,
        shadow_w,
        shadow_h
    };

    SDL_Rect tree_shadow2 = {
        310,
        75,
        shadow_w,
        shadow_h
    };

    SDL_RenderCopy(renderer, textureTreeShadow, NULL, &tree_shadow1);
    SDL_RenderCopy(renderer, textureTreeShadow, NULL, &tree_shadow2);
    int w, h;
    SDL_QueryTexture(textureTree, NULL, NULL, &w, &h);
    SDL_Rect giant_tree1 = {
         280,
         320,
         w * 1.2,
         h * 1.2
    };

     SDL_Rect giant_tree2 = {
         320,
         30,
         w * 1.2,
         h * 1.2
    };


    SDL_RenderCopy(renderer, textureTree, NULL, &giant_tree1);
    SDL_RenderCopy(renderer, textureTree, NULL, &giant_tree2);


    //Draw rocks
    drawObject(renderer, 350, 400, textureRock);
    //
    drawObject(renderer, 540, 320, textureRuin);
    drawObject(renderer, 480, 380, textureRuinStair);

    drawObject(renderer, 150, 500, textureCamp3);
    drawObject(renderer, 240, 520, textureCamp4);

    //drawObject(renderer, 200, 480, textureTreeShadow);
    drawWithAnimation(renderer, 240, 480, textureCampFire);

    }

    if(chooseLevel == 2){
        drawObject(renderer, 480, 0, textureTree);

        drawObject(renderer, 400, 0, textureTree);

        drawObject(renderer, 300, 500, textureTree);

        drawObject(renderer, 320, 0 , textureTree);

        drawObject(renderer, 580, 0, textureTree);

        drawObject(renderer, 350, 50, textureTree);

        drawObject(renderer, 420, 50, textureTree);

    }
       for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (getTileType(x,y) == TileType::portal) {
                SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
                SDL_RenderCopy(renderer, texturePortal, NULL, &rect);
            }
//            if (getTileType(x,y) == TileType::greenPortal) {
//                SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
//                SDL_RenderCopy(renderer, textureGreenPortal, NULL, &rect);
//            }
        }

    }

    timerForLevel3.countDown(1.0f / 60.0f);
    if(Game::totalRoundsSpawned % 2 == 0 && chooseLevel == 2){
        setTileType(1, 7, TileType::portal);
        setTileType(5, 3, TileType::portal);
        setTileType(2, 1, TileType::empty);
        setTileType(5, 5, TileType::empty);

        portalDestinations[0] = Vector2D(1, 7);
        portalDestinations[1] = Vector2D(5, 3);

        setTileType(13, 7, TileType::greenPortal);
        setTileType(9, 5, TileType::greenPortal);
      //  setTileType(2, 1, TileType::empty);
      //  setTileType(5, 5, TileType::empty);

        portalDestinations[2] = Vector2D(13, 7);
        portalDestinations[3] = Vector2D(9, 5);
    }
    else if(Game::totalRoundsSpawned % 2 == 1 && chooseLevel == 2){
        setTileType(1, 7, TileType::empty);
        setTileType(5, 3, TileType::empty);
        setTileType(2, 1, TileType::portal);
        setTileType(5, 5, TileType::portal);

        portalDestinations[0] = Vector2D(2, 1);
        portalDestinations[1] = Vector2D(5, 5);

    }


    if(Game::totalRoundsSpawned % 2 == 0 && chooseLevel == 3 && Game::spawnUnitCount == 8 && timerForLevel3.timeSIsZero()){
        setTileType(1, 1, TileType::wall);
        setTileType(2, 1, TileType::wall);
        setTileType(7, 1, TileType::wall);
        setTileType(8, 1, TileType::wall);
        setTileType(3, 8, TileType::wall);
        setTileType(4, 8, TileType::no_tower);
        setTileType(5, 8, TileType::no_tower);
        setTileType(6, 8, TileType::no_tower);
        setTileType(11, 7, TileType::wall);

        setTileType(1, 3, TileType::empty);
        setTileType(2, 3, TileType::empty);
        setTileType(4, 4, TileType::empty);
        setTileType(5, 4, TileType::empty);
        setTileType(7, 4, TileType::empty);
        setTileType(8, 4, TileType::empty);
        setTileType(11, 5, TileType::empty);
        setTileType(12, 5, TileType::empty);
        setTileType(13, 5, TileType::empty);
        timerForLevel3.resetToMax();
    } else if(Game::totalRoundsSpawned % 2 == 1 && chooseLevel == 3 && Game::spawnUnitCount == 8 && timerForLevel3.timeSIsZero()){
        setTileType(1, 3, TileType::wall);
        setTileType(2, 3, TileType::wall);
        setTileType(7, 1, TileType::wall);
        setTileType(8, 1, TileType::wall);
        setTileType(4, 4, TileType::wall);
        setTileType(5, 4, TileType::wall);
        setTileType(7, 4, TileType::wall);
        setTileType(8, 4, TileType::wall);
        setTileType(11, 5, TileType::wall);
        setTileType(12, 5, TileType::wall);
        setTileType(13, 5, TileType::wall);

        setTileType(1, 1, TileType::empty);
        setTileType(2, 1, TileType::empty);
        setTileType(7, 1, TileType::empty);
        setTileType(8, 1, TileType::empty);
        setTileType(3, 8, TileType::empty);
        setTileType(4, 8, TileType::empty);
        setTileType(5, 8, TileType::empty);
        setTileType(6, 8, TileType::empty);
        setTileType(11, 7, TileType::empty);
        timerForLevel3.resetToMax();
    }

}

void Level::drawObject(SDL_Renderer* renderer, int pos_x, int pos_y, SDL_Texture* object){
    int w, h;
    SDL_QueryTexture(object, NULL, NULL, &w, &h);
    SDL_Rect object_Rect = {
        pos_x,
        pos_y,
        w,
        h
    };
    SDL_RenderCopy(renderer, object, NULL, &object_Rect);

}

void Level::drawWithAnimation(SDL_Renderer* renderer, int pos_x, int pos_y, SDL_Texture* object){
        const int WALKING_ANIMATION = 6;
		SDL_Rect SpriteOrge[WALKING_ANIMATION];
        int w, h;
        SDL_QueryTexture(object, NULL, NULL, &w, &h);
        SDL_Rect rect = {
            pos_x - w / 12,
			pos_y - h / 2,
			w / 6,
			h };

		SpriteOrge[0].x = 0;
		SpriteOrge[0].y = 0;
		SpriteOrge[0].w = 64;
		SpriteOrge[0].h = 64;

		SpriteOrge[1].x = 64;
		SpriteOrge[1].y = 0;
		SpriteOrge[1].w = 64;
		SpriteOrge[1].h = 64;

		SpriteOrge[2].x = 128;
		SpriteOrge[2].y = 0;
		SpriteOrge[2].w = 64;
		SpriteOrge[2].h = 64;

		SpriteOrge[3].x = 192;
		SpriteOrge[3].y = 0;
		SpriteOrge[3].w = 64;
		SpriteOrge[3].h = 64;

		SpriteOrge[4].x = 256;
		SpriteOrge[4].y = 0;
		SpriteOrge[4].w = 64;
		SpriteOrge[4].h = 64;

		SpriteOrge[5].x = 320;
		SpriteOrge[5].y = 0;
		SpriteOrge[5].w = 64;
		SpriteOrge[5].h = 64;

		int frame = SDL_GetTicks() / 100;
		int currentFrame = frame % WALKING_ANIMATION;
		SDL_Rect currentSprite = SpriteOrge[currentFrame];

        SDL_RenderCopy(renderer, object, &currentSprite, &rect);
		++frame;
		if(frame / 4 >= WALKING_ANIMATION)
            frame = 0;
}


void Level::drawTile(SDL_Renderer* renderer, int x, int y, int tileSize) {
    //Set the default texture image to be empty.
    SDL_Texture* textureSelected = textureTileEmpty;

    //Ensure that the input tile exists.
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY) {
        Tile& tileSelected = listTiles[index];

        //Select the correct tile texture based on the flow direction.
        /*if (tileSelected.flowDirectionX == 0 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUp;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUpRight;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == 0)
            textureSelected = textureTileArrowRight;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDownRight;
        else if (tileSelected.flowDirectionX == 0 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDown;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDownLeft;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == 0)
            textureSelected = textureTileArrowLeft;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUpLeft;
*/
    }


    //Draw the tile.
    if (textureSelected != nullptr) {
        SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
        SDL_RenderCopy(renderer, textureSelected, NULL, &rect);
    }
}



Vector2D Level::getRandomEnemySpawnerLocation() {
    //Create a list of all tiles that are enemy spawners.
    std::vector<int> listSpawnerIndices;
    for (int count = 0; count < listTiles.size(); count++) {
        auto& tileSelected = listTiles[count];
        if (tileSelected.type == TileType::enemySpawner)
            listSpawnerIndices.push_back(count);
    }

    //If one or more spawners are found, pick one at random and output it's center position.
    if (listSpawnerIndices.empty() == false) {
        int index = listSpawnerIndices[rand() % listSpawnerIndices.size()];
        return Vector2D((float)(index % tileCountX) + 0.5f, (float)(index / tileCountX) + 0.5f);
    }

    return Vector2D(0.5f, 0.5f);
}




bool Level::isTileWall(int x, int y) {
    return (getTileType(x, y) == TileType::wall);
}

bool Level::isTilePath(int x, int y) {
    return (getTileType(x, y) == TileType::empty);
}

bool Level::isBushOccupied(int x, int y) {
    return (getTileType(x, y) == TileType::bush_occupied);
 }

bool Level::isTreeOccupied(int x, int y) {
    return (getTileType(x, y) == TileType::tree_occupied);
}

bool Level::noTower(int x, int y){
    return (getTileType(x, y) == TileType::no_tower);
}
void Level::setTileWall(int x, int y, bool setWall) {
    if (getTileType(x, y) != TileType::enemySpawner)
        setTileType(x, y, (setWall ? TileType::wall : TileType::empty));
}

Level::TileType Level::getTileType(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return listTiles[index].type;

    return TileType::empty;
}


void Level::load_map(std::vector<int> Types){
   // vector<Tile> loadedTiles(Types.size());
   //int index = x + y * tileCountX;
 /*   std::vector<int> Types = {1, 1, 1, 1, 1, 4, 4, 4, 4, 2, 1, 1, 1, 1, 1,
                              0, 0, 0, 0, 1, 4, 4, 4, 2, 1, 3, 1, 1, 0, 0,
                              0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0,
                              1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
                              1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1,
                              1, 0, 1, 0, 4, 4, 1, 0, 4, 4, 1, 0, 1, 0, 1,
                              1, 0, 0, 0, 4, 4, 4, 1, 4, 4, 1, 0, 0, 0, 1,
                              1, 0, 1, 4, 4, 4, 4, 4, 4, 3, 1, 0, 1, 0, 1,
                              1, 0, 1, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 0, 1};
                              */
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            int index = x + y * tileCountX;
    //int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY) {
            if(Types[index] == 0){
        listTiles[index].type = TileType::empty;
        }
            if(Types[index] == 1){
                listTiles[index].type = TileType::wall;
            }

            if(Types[index] == 2){
                listTiles[index].type = TileType::bush_occupied;
            }

            if(Types[index] == 3){
                listTiles[index].type = TileType::tree_occupied;
            }
            if(Types[index] == 4){
                listTiles[index].type = TileType::no_tower;
            }

}
}
}
    }



void Level::setTileType(int x, int y, TileType tileType) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY) {
        listTiles[index].type = tileType;
        calculateFlowField();
    }
}



Vector2D Level::getTargetPos() {
    return Vector2D((float)targetX + 0.5f, (float)targetY + 0.5f);
}


void Level::calculateFlowField() {
    //Ensure the target is in bounds.
    int indexTarget = targetX + targetY * tileCountX;
    if (indexTarget > -1 && indexTarget < listTiles.size() &&
        targetX > -1 && targetX < tileCountX &&
        targetY > -1 && targetY < tileCountY) {

        //Reset the tile flow data.
        for (auto& tileSelected : listTiles) {
            tileSelected.flowDirectionX = 0;
            tileSelected.flowDirectionY = 0;
            tileSelected.flowDistance = flowDistanceMax;
        }

        //Calculate the flow field.
        calculateDistances();
        calculateFlowDirections();
    }
}

//BFS Implemented
void Level::calculateDistances() {
    int indexTarget = targetX + targetY * tileCountX;

    //Create a queue that will contain the indices to be checked.
    std::queue<int> listIndicesToCheck;
    //Set the target tile's flow value to 0 and add it to the queue.
    listTiles[indexTarget].flowDistance = 0;
    listIndicesToCheck.push(indexTarget);

    //The offset of the neighboring tiles to be checked.
    const int listNeighbors[][2] = { { -1, 0}, {1, 0}, {0, -1}, {0, 1} };

    //Loop through the queue and assign distance to each tile.
    while (listIndicesToCheck.empty() == false) {
        int indexCurrent = listIndicesToCheck.front();
        listIndicesToCheck.pop();

        //Check each of the neighbors;
        for (int count = 0; count < 4; count++) {
            int neighborX = listNeighbors[count][0] + indexCurrent % tileCountX;
            int neighborY = listNeighbors[count][1] + indexCurrent / tileCountX;
            int indexNeighbor = neighborX + neighborY * tileCountX;

            //Ensure that the neighbor exists and isn't a wall.
            if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                neighborX > -1 && neighborX < tileCountX &&
                neighborY > -1 && neighborY < tileCountY &&
                listTiles[indexNeighbor].type != TileType::wall) {

                //Check if the tile has been assigned a distance yet or not.
                if (listTiles[indexNeighbor].flowDistance == flowDistanceMax) {
                    //If not the set it's distance and add it to the queue.
                    listTiles[indexNeighbor].flowDistance = listTiles[indexCurrent].flowDistance + 1;
                    listIndicesToCheck.push(indexNeighbor);
                }
            }
        }
    }
}


void Level::calculateFlowDirections() {
    //The offset of the neighboring tiles to be checked.
    const int listNeighbors[][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1} };

    for (int indexCurrent = 0; indexCurrent < listTiles.size(); indexCurrent++) {
        //Ensure that the tile has been assigned a distance value.
        if (listTiles[indexCurrent].flowDistance != flowDistanceMax) {
            //Set the best distance to the current tile's distance.
            unsigned char flowFieldBest = listTiles[indexCurrent].flowDistance;

            //Check each of the neighbors;
            for (int count = 0; count < 8; count++) {
                int offsetX = listNeighbors[count][0];
                int offsetY = listNeighbors[count][1];

                int neighborX = offsetX + indexCurrent % tileCountX;
                int neighborY = offsetY + indexCurrent / tileCountX;
                int indexNeighbor = neighborX + neighborY * tileCountX;

                //Ensure that the neighbor exists.
                if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                    neighborX > -1 && neighborX < tileCountX &&
                    neighborY > -1 && neighborY < tileCountY) {
                    //If the current neighbor's distance is lower than the best then use it.
                    if (listTiles[indexNeighbor].flowDistance < flowFieldBest) {
                        flowFieldBest = listTiles[indexNeighbor].flowDistance;
                        listTiles[indexCurrent].flowDirectionX = offsetX;
                        listTiles[indexCurrent].flowDirectionY = offsetY;
                    }
                }
            }
        }
    }
}
/*
void Level::teleportUnit(Vector2D& unitPosition) {
    //Check if the unit is on a portal
    float distanceToPortal = (portalDestinations[0] - unitPosition).magnitude();
    int x = (int)unitPosition.x;
    int y = (int)unitPosition.y;
    if (getTileType(x, y) == TileType::portal) {
       // int destinationIndex = (x <= portalDestinations[0].x && y == portalDestinations[0].y) ? 1 : 0;
       // int destinationIndex = (distanceToPortal <= 1) ? 1 : 0;
       if(distanceToPortal <= 1)
        unitPosition = portalDestinations[1];
    }
}
*/
void Level::teleportUnit(Vector2D& unitPosition) {
    // Check if the unit is on portal destination 0
    float distanceToPortal0 = (portalDestinations[0] - unitPosition).magnitude();
   // float distanceToPortal2 = (portalDestinations[2] - unitPosition).magnitude();
    if (distanceToPortal0 <= 0.5f) {
        // Teleport the unit to portal destination 1
        unitPosition = portalDestinations[1];
    }
}





Vector2D Level::getFlowNormal(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return Vector2D((float)listTiles[index].flowDirectionX, (float)listTiles[index].flowDirectionY).normalize();

    return Vector2D();
}
