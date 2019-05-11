#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
private:
    void changeVdir(const char *dirName, int dir)
    {
        transform->velocity.y = dir;
        transform->orientation.Clear();
        transform->orientation.y = dir;
        transform->normalizeVelocity();
        sprite->Play(dirName);

    }
    
    void changeHdir(const char *dirName, int dir)
    {
        transform->velocity.x = dir;
        transform->orientation.Clear();
        transform->orientation.x = dir;
        transform->normalizeVelocity();
        sprite->Play(dirName);
        
    }
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }
    
    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
                case SDLK_w:
                    changeVdir("Up", -1);
                    break;
                case SDLK_a:
                    changeHdir("Left", -1);
                    break;
                case SDLK_d:
                    changeHdir("Right", 1);
                    break;
                case SDLK_s:
                    changeVdir("Down", 1);
                    break;
                case SDLK_SPACE:
                    sprite->attackMode();
                    Game::assets->CreateProjectile(player, Game::playerRole);
                    break;
                    
                default:
                    break;
            }
        }
        
        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
                case SDLK_w:
                    transform->velocity.y = 0;
                    transform->normalizeVelocity();
                    break;
                case SDLK_a:
                    transform->velocity.x = 0;
                    transform->normalizeVelocity();
                    break;
                case SDLK_d:
                    transform->velocity.x = 0;
                    transform->normalizeVelocity();
                    break;
                case SDLK_s:
                    transform->velocity.y = 0;
                    transform->normalizeVelocity();
                    break;
                case SDLK_ESCAPE:
                    Game::isRunning = false;
                    break;
                    
                case SDLK_SPACE:
                    sprite->reverseAttackMode();
                    break;
                default:
                    break;
            }
            
        }
    }
};
