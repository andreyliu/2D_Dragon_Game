#include "Enemies.h"
#include "Map.h"
#include "Game.h"
#include "AssetManager.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

void Enemies::LoadEnemies(std::string path, int mapWidth, int mapHeight)
{
    std::vector<int> walkRanges;
    std::vector<int> vel;
    
    char c;
    std::fstream file;
    file.open(path);
    std::string line;
    std::getline(file, line);
    int numEnemies = std::stoi(line, nullptr);
    
    file.ignore();

    for (int i = 0; i < numEnemies; i++)
    {
        std::getline(file, line, ',');
        int wRange = std::stoi(line, nullptr);
        walkRanges.push_back(wRange);
    }
    
    file.ignore();
    file.ignore();
    
    for (int i = 0; i < numEnemies * 2; i++)
    {
        std::getline(file, line, ',');
        int wRange = std::stoi(line, nullptr);
        vel.push_back(wRange);
    }
    
    file.ignore();
    file.ignore();

    int num;
    int counter = 0;
    
    for (int row = 0; row < mapHeight; row++)
    {
        for (int col = 0; col < mapWidth; col++)
        {
            file.get(c);
            num = atoi(&c);
            if (num == 1)
            {
                counter++;
                int xvel = vel.back();
                vel.pop_back();
                int yvel = vel.back();
                vel.pop_back();
                Enemies::AddEnemy(col * Map::scaledSize, row * Map::scaledSize, xvel, yvel, walkRanges.back(), counter);
                walkRanges.pop_back();
            }
            file.ignore();
        }
    }
    file.close();
}



void Enemies::AddEnemy(int xpos, int ypos, int xvel, int yvel, int walkRange, int seed)
{
    Game::assets->CreateEnemy(walkRange, xpos, ypos, xvel, yvel, Game::enemyBasicT, "enemy", seed);
}
