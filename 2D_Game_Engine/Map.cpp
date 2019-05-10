#include "Map.h"
#include "Game.h"
#include <fstream>

Map::Map()
{

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
    
    for (int col = 0; col < sizeY; col++)
    {
        for (int row = 0; row < sizeX; row++)
        {
            mapFile.get(c);
            srcY = atoi(&c);
            mapFile.get(c);
            srcX = atoi(&c);
            Game::AddTile(srcX * 32, srcY * 32, row * 64, col * 64);
            mapFile.ignore();
        }
    }
    
    mapFile.close();
}

