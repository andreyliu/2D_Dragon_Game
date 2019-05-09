#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <SDL2/SDL.h>

class TileComponent : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    
    SDL_Rect tileRect;
    int tileID;
    const char *path;
    
    TileComponent() = default;
    
    TileComponent(int x, int y, int w, int h, int tileID)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        this->tileID = tileID;
        
        switch (tileID)
        {
            case 0:
                path = "/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/water.png";
                break;
            case 1:
                path = "/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/dirt.png";
                break;
            case 2:
                path = "/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/grass.png";
                break;
            default:
                break;
        }
    }
    
    void init() override
    {
        entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 1);
        transform = &entity->getComponent<TransformComponent>();
        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};
