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
    char tile;
    std::fstream mapFile;
    mapFile.open(path);
    
    for (int col = 0; col < sizeY; col++)
    {
        for (int row = 0; row < sizeX; row++)
        {
            mapFile.get(tile);
            std::cout << tile << std::endl;
            Game::AddTile(atoi(&tile), row * 32, col * 32);
            mapFile.ignore();
        }
    }
    
    mapFile.close();
}

