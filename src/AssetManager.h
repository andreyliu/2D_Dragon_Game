#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include "SDL2/SDL_ttf.h"
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
    
    void CreateProjectile(Entity &entity, Game::PType type, char option, bool sideOffset);
    
    void CreateEnemy(float walkR, int xpos, int ypos, int xvel, int yvel, Game::PType type, std::string id, int seed);
    
private:
    
    Manager *manager;
    std::map<std::string, SDL_Texture *> textures;
    std::map<std::string, TTF_Font *> fonts;
    
    static void ProjTransform(Entity &projectile, Entity &player, int srcSize, float scale, Vector2D &vel);

    static void P_FireballTransform(Entity &projectile, Entity &player, int srcSize, float scale, Vector2D &vel,
                                    char option, bool sideOffset);
    static void CreateEnemy(Entity &enemy, float wr, float ws, float ps, float fs, std::string id);
};
