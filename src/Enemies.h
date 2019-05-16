#pragma once
#include <string>

class Enemies
{
public:
    void LoadEnemies(std::string path, int mapWidth, int mapHeight);
    void AddEnemy(int xpos, int ypos, int xvel, int yvel, int walkRange, int ID);
};
