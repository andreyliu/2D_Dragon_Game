#pragma once

#include "ECS.h"
#include "SDL2/SDL.h"
#include "../Game.h"
#include "../TextureManager.h"
#include "../Vector2D.h"

class TileComponent : public Component
{
public:

    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    Vector2D position;
    
    TileComponent() = default;
    
    TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string ID)
    {
        texture = Game::assets->GetTexture(ID);
        
        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = tsize;
        position.x = static_cast<float>(xpos);
        position.y = static_cast<float>(ypos);
        destRect.w = destRect.h = tsize * tscale;
    }
    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }
    
    void update() override
    {
        destRect.x = static_cast<int>(position.x - Game::camera.x);
        destRect.y = static_cast<int>(position.y - Game::camera.y);
    }
    
    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
};
