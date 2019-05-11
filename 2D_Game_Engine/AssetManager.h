#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Game.h"

class AssetManager
{
public:
    AssetManager(Manager *man);
    ~AssetManager();
    
    //game objects
    
    // texture management
    void AddTexture(std::string ID, const char *path);
    
    SDL_Texture *GetTexture(std::string ID);
    
    void AddFont(std::string ID, std::string path, int fontSize);
    
    TTF_Font *GetFont(std::string ID);
    
    void CreateProjectile(Entity &entity, Game::Roles type, char option);
    
private:
    
    Manager *manager;
    std::map<std::string, SDL_Texture *> textures;
    std::map<std::string, TTF_Font *> fonts;
    
    static const int pSpeed = 0.5;
    
    void CreateProjectile(Entity &projectile, Entity &player, int range, int speed, int dmg, std::string id, char option);
};
