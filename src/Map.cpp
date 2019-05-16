#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern Manager manager;

int Map::scaledSize = 0;

Map::Map(std::string tID, int ms, int ts)
: texID(tID), mapScale(ms), tileSize(ts)
{
    scaledSize = ms * ts;
}

Map::~Map()
{
    
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
    char c;
    std::fstream mapFile;
    mapFile.open(path);
    
    int srcX, srcY;
    
    mapW = sizeX * scaledSize;
    mapH = sizeY * scaledSize;
    
    for (int col = 0; col < sizeY; col++)
    {
        for (int row = 0; row < sizeX; row++)
        {
            mapFile.get(c);
            srcY = atoi(&c);
            mapFile.get(c);
            srcX = atoi(&c);
            AddTile(srcX * tileSize, srcY * tileSize, row * scaledSize, col * scaledSize);
            mapFile.ignore();
        }
    }
    
    mapFile.ignore();
    
    for (int col = 0; col < sizeY; col++)
    {
        for (int row = 0; row < sizeX; row++)
        {
            mapFile.get(c);
            if (c == '1')
            {
                auto &tcol(manager.addEntity());
                tcol.addComponent<ColliderComponent>("terrain", row * scaledSize, col * scaledSize, scaledSize);
                tcol.addGroup(Game::groupColliders);
            }
            mapFile.ignore();
        }
    }
    
    
    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupMap);
}
