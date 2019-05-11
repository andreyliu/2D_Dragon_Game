#pragma once
#include "Components.h"
#include "SDL2/SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include <string>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    const char *currAnimation;
    
    bool animated = false;
    int frames = 0;
    int speed = 100;
    
public:
    
    int animIndex = 0;
    
    std::map<const char *, Animation> animations;
    
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
    SpriteComponent() = default;
    SpriteComponent(std::string ID)
    {
        setTex(ID);
    }
    
    SpriteComponent(std::string ID, bool isAnimated)
    {
        animated = isAnimated;
        if (isAnimated)
        {
            Animation up = Animation(0, 3, 300);
            Animation right = Animation(1, 3, 300);
            Animation down = Animation(2, 3, 300);
            Animation left = Animation(3, 3, 300);
            
            animations.emplace("Up", up);
            animations.emplace("Right", right);
            animations.emplace("Down", down);
            animations.emplace("Left", left);
            
            Play("Down");
        }
        
        setTex(ID);
    }
    
    ~SpriteComponent()
    {}
    
    void setTex(std::string ID)
    {
        texture = Game::assets->GetTexture(ID);
    }
    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        
        srcRect.x = srcRect.y = 0;
        srcRect.w = 191;
        srcRect.h = 161;
        destRect.w = transform->width;
        destRect.h = transform->height;
    }
    
    void update() override
    {
        if (animated)
        {
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
        }
        
        srcRect.y = animIndex * transform->height;
        
        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
        destRect.w = static_cast<int>(transform->width * transform->scale);
        destRect.h = static_cast<int>(transform->height * transform->scale);
    }
    
    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }
    
    void Play(const char *animName)
    {
        currAnimation = animName;
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
    
    void setAnimation(bool val)
    {
        animated = val;
        if (val) {
            Play(currAnimation);
        }
    }
    
    void attackMode()
    {
        if (strncmp(currAnimation, "Up", 2) == 0 || strncmp(currAnimation, "Down", 4) == 0)
        {
            return;
        }
        animated = false;
        srcRect.x = 573;
    }
    
    void reverseAttackMode()
    {
        if (strncmp(currAnimation, "Up", 2) == 0 || strncmp(currAnimation, "Down", 4) == 0)
        {
            return;
        }
        animated = true;
        srcRect.x = 0;
        Play(currAnimation);
    }
};



