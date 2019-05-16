#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "ECS.h"
#include "Components.h"

class ColliderComponent : public Component
{
    
public:
    
    SDL_Rect collider = { 0, 0, 0, 0 };
    std::string tag;
    
    SDL_Texture *tex;
    SDL_Rect srcR, destR;
    
    TransformComponent *transform;
    
    ColliderComponent(std::string t)
    {
        tag = t;
    }
    
    ColliderComponent(std::string t, int xpos, int ypos, int size)
    {
        tag = t;
        collider.x = xpos;
        collider.y = ypos;
        collider.h = collider.w = size;
    }
    
    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
        
        tex= TextureManager::LoadTexture("/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/ColTex.png");
        
        srcR = { 0, 0, 32, 32 };
        destR = { collider.x, collider.y, collider.w, collider.h };
        
    }
    
    void update() override
    {
        if (tag != "terrain")
        {
            collider.x = static_cast<int>(transform->position.x);
            collider .y = static_cast<int>(transform->position.y);
            collider.w = static_cast<int>(transform->width * transform->scale);
            collider.h = static_cast<int>(transform->height * transform->scale);
        }
        
        destR.x = collider.x - Game::camera.x;
        destR.y = collider.y - Game::camera.y;
        
    }
    
    void draw() override
    {
        if (tag != "terrain") return;
        TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
    }
};