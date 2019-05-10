#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:
    
    Vector2D position;
    Vector2D velocity;
    
    int height = 161;
    int width = 191;
    float scale = 1;
    
    int speed = 3;
    
    
    TransformComponent()
    {
        TransformComponent(400, 320);
    }
    
    TransformComponent(float sc)
    {
        TransformComponent();
        scale = sc;
    }
    
    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    
    TransformComponent(float x, float y, int h, int w, int sc)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }
    
    void init() override
    {
        velocity.Zero();
    }
    void update() override
    {
//        position.x += velocity.x * speed;
//        position.y += velocity.y * speed;
    }
};
