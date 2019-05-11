#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include <memory>

class ProjectileComponent : public Component
{
public:
    ProjectileComponent(Entity &source, int rng, int sp, Vector2D vel, int dmg)
    : range(rng), speed(sp), velocity(vel), damage(dmg)
    {
        std::unique_ptr<DisplaySize> playerSize(source.getComponent<SpriteComponent>().getSize());
        auto &position = source.getComponent<TransformComponent>().position;
        int w = playerSize->width / 2;
        int h = playerSize->height / 2;
        startX += w + position.x;
        startY += h + position.y;
        visibleHorizon = std::max(w, h);
    }
    
    ~ProjectileComponent()
    {}
    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        transform->velocity = velocity;
        transform->speed = speed;
        std::unique_ptr<DisplaySize> spriteSize(sprite->getSize());
        startX -= spriteSize->width / 2;
        startY -= spriteSize->height / 2;
        transform->position.x = startX;
        transform->position.y = startY;
        sprite->setVisible(false);
    }
    
    void update() override
    {
        distance += speed;
        if (!sprite->Visible() && distance >= visibleHorizon) sprite->setVisible(true);
        if (distance > range)
        {
            std::cout << "Out Of Range" << std::endl;
            entity-> destroy();
        }
    }
    
private:
    TransformComponent *transform;
    SpriteComponent *sprite;
    int range = 0;
    int speed = 0;
    int distance = 0;
    Vector2D velocity;
    int damage = 0;
    int startX = 0;
    int startY = 0;
    int visibleHorizon = 0;
};
