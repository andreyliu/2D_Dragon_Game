#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include <memory>

class KeyboardController : public Component
{
private:
    
    Uint32 lastAttack = 0;
    
    void RoarAttack()
    {
        if (Game::event.key.repeat == 0)
        {
            return;
        }
        sprite->attackMode();
        bool side = player.getComponent<TransformComponent>().orientation.x != 0;
        Uint32 tick = SDL_GetTicks();
        if (tick - lastAttack < 50) return;
//        std::cout << "ROAR ATTACK" << std::endl;
        lastAttack = tick;
        Game::assets->CreateProjectile(player, Game::playerFlameT, 'N', side);
    }
    
    void FireballAttack(char option)
    {
        bool side = player.getComponent<TransformComponent>().orientation.x != 0;
        Uint32 tick = SDL_GetTicks();
        if (tick - lastAttack < 300) return;
        lastAttack = tick;
        Game::assets->CreateProjectile(player, Game::playerT, option, side);
    }
    
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
        if (Game::event.type == SDL_KEYDOWN) // && Game::event.key.repeat == 0
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
                case SDLK_k:
                    FireballAttack('N');
                    break;
                    
                case SDLK_j:
                    FireballAttack('D');
                    break;
                    
                case SDLK_l:
                    FireballAttack('U');
                    break;
                    
                case SDLK_SPACE:
                    RoarAttack();
                    break;
                    
                case SDLK_ESCAPE:
                    Game::isRunning = false;
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
                case SDLK_SPACE:
                    sprite->reverseAttackMode();
                    break;
                default:
                    break;
            }
            
        }
    }
};
