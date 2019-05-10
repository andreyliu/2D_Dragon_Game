#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include <SDL2/SDL_ttf.h>

class AssetManager
{
public:
    AssetManager(Manager *man);
    ~AssetManager();
    
    //game objects
    
    void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
    
    // texture management
    void AddTexture(std::string ID, const char *path);
    
    SDL_Texture *GetTexture(std::string ID);
    
    void AddFont(std::string ID, std::string path, int fontSize);
    
    TTF_Font *GetFont(std::string ID);
    
private:
    
    Manager *manager;
    std::map<std::string, SDL_Texture *> textures;
    std::map<std::string, TTF_Font *> fonts;
};
