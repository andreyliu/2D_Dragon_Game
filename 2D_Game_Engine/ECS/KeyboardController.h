#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
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
                    transform->velocity.y = -1;
                    transform->normalizeVelocity();
                    sprite->Play("Up");
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    transform->normalizeVelocity();
                    sprite->Play("Left");
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    transform->normalizeVelocity();
                    sprite->Play("Right");
                    break;
                case SDLK_s:
                    transform->velocity.y = 1;
                    transform->normalizeVelocity();
                    sprite->Play("Down");
                    break;
                    
                case SDLK_SPACE:
                    sprite->attackMode();
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
