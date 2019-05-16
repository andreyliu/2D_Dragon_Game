#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include <memory>

class ProjectileComponent : public Component
{
public:
    ProjectileComponent(int rng, float sp, int dmg)
    : range(rng), speed(sp), damage(dmg)
    {}
    
    ~ProjectileComponent()
    {}
    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        transform->speed = speed;
        std::unique_ptr<Vector2D> spriteSize(entity->getComponent<SpriteComponent>().getSize());
        transform->position.x -= spriteSize->x / 2;
        transform->position.y -= spriteSize->y / 2;
    }
    
    void update() override
    {

        distance += speed;
        if (distance > range)
        {
            entity-> destroy();
        }
    }
    
private:
    TransformComponent *transform;
    int range = 0;
    float speed = 0;
    float distance = 0;
    int damage = 0;
};
