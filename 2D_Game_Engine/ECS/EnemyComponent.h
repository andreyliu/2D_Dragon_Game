#pragma once
#include "Components.h"
#include "../Game.h"
#include <memory>
#include <ctime>
#include <cstdlib>

//int EnemyComponent::projRange = 300;
//int EnemyComponent::sensingDist = 500;
//int EnemyComponent::baseDamage = 40;

class EnemyComponent : public Component
{
public:
    const float walkRange;
    const float walkSpeed;
    const float projSpeed;
    const float followSpeed;
    const static int projRange = 200;
    const static int sensingDist = 400; // 500
    const static int baseDamage = 40;
    int enemyID;
    //initialize
    Uint32 lastAttack = 0;
//    bool moving = false;
    
    EnemyComponent(float wr, float ws, float ps, float fs)
    : walkRange(wr), walkSpeed(ws), projSpeed(ps), followSpeed(fs)
    {}
    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        transform->speed = walkSpeed;
        sprite = &entity->getComponent<SpriteComponent>();
        sprite->spriteFlip = transform->velocity.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    }
    
    void update() override
    {
        float dist = PlayerDist();
        if (dist > sensingDist) //
        {
            transform->speed = walkSpeed;
            walk();
            return;
        }
        
        if (dist < projRange)
        {
            int x = RandomVecValue();
            transform->velocity.x = x;
            transform->velocity.y = x == 0 ? RandomBinValue(): RandomVecValue();
            transform->velocity.Normalize();
        }
        else {
            transform->speed = followSpeed;
            //        // TODO if there is a target, follow and shoot stuff with a delay
            std::unique_ptr<Vector2D> tempV(transform->position.TurnTo(player.getComponent<TransformComponent>().position));
            tempV->Normalize();
            transform->velocity = *tempV;
        }
        attack();
    }
    
    void walk()
    {
        if (walkRange == 0)
        {
            sprite->Play("Idle");
            return;
        }
        
        dist += walkSpeed;
    
        if (dist >= walkRange)
        {
            TurnAround();
            
        }
        
    }
    
    void attack()
    {
        Uint32 tick = SDL_GetTicks();
        if (tick - lastAttack < delay)
        {
            return;
        }
        lastAttack = tick;
        Game::assets->CreateProjectile(*entity, Game::enemyBasicT, 'N', false);
        
    }
    
private:
//    Entity *target = nullptr;
    TransformComponent *transform;
    SpriteComponent *sprite;
    float dist = 0;
    const static Uint32 delay = 2500;
    float minDist = 150;
    
    void TurnAround()
    {
        transform->velocity *= -1;
        sprite->spriteFlip = sprite->spriteFlip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        dist = 0;
    }
    
    float PlayerDist()
    {
        std::unique_ptr<Vector2D> diff(transform->position.Diff(player.getComponent<TransformComponent>().position));
        return diff->L2Norm();
    }
    
    int RandomVecValue()
    {
        srand(enemyID);  // Initialize random number generator.
        int r = rand() % 3;
        return r - 1;
    }
    
    int RandomBinValue()
    {
        srand(enemyID);  // Initialize random number generator.
        int r = rand() & 1;
        return (r == 0) ? -1 : 1;
    }
    
};
