#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include <memory>

class ProjectileComponent : public Component
{
public:
    ProjectileComponent(int rng, float sp, Vector2D vel, int dmg)
    : range(rng), speed(sp), velocity(vel), damage(dmg)
    {}
    
    ~ProjectileComponent()
    {}
    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        transform->velocity = velocity;
        transform->speed = speed;
        std::unique_ptr<Vector2D> spriteSize(entity->getComponent<SpriteComponent>().getSize());
        transform->position.x -= spriteSize->x / 2;
        transform->position.y -= spriteSize->y / 2;
        
    }
    
//    TransformComponent &getTransform()
//    {
//        return *transform;
//    }
    
    void update() override
    {
        distance += speed;
        if (distance > range)
        {
//            std::cout << "Out Of Range" << std::endl;
            entity-> destroy();
        }
    }
    
private:
    TransformComponent *transform;
    Vector2D velocity;
    int range = 0;
    float speed = 0;
    int distance = 0;
    int damage = 0;
};
