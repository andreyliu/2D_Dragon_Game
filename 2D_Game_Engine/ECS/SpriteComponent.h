#pragma once
#include "Components.h"
#include "SDL2/SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include <string>
#include "../AssetManager.h"

struct DisplaySize
{
    int width;
    int height;
    
    DisplaySize(int w, int h)
    {
        width = w;
        height = h;
    }
};

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    const char *currAnimation;
    std::string ID;
    
    bool animated = false;
    bool visible = true;
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
        this->ID = ID;
    }
    
    SpriteComponent(std::string ID, bool isAnimated)
    {
        setTex(ID);
        this->ID = ID;
        animated = isAnimated;
    }
    
    ~SpriteComponent()
    {}
    
    void setTex(std::string ID)
    {
        texture = Game::assets->GetTexture(ID);
    }
    
    void addAnimation(int index, int nFrames, int speed, const char *name)
    {
        Animation anim = Animation(index, nFrames, speed);
        animations.emplace(name, anim);
        Play(name);
    }
    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
        destRect.w = static_cast<int>(transform->width * transform->scale);
        destRect.h = static_cast<int>(transform->height * transform->scale);
        if (animated && ID == "player")
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
        if (!visible) return;
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }
    
    void Play(const char *animName)
    {
        currAnimation = animName;
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
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
    
    DisplaySize *getSize()
    {
        return new DisplaySize(destRect.w, destRect.h);
    }
    
    void setVisible(bool val)
    {
        visible = val;
    }
    
    bool Visible()
    {
        return visible;
    }
};



