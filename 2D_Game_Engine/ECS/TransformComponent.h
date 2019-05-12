#pragma once
#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:
    
    Vector2D position;
    Vector2D velocity;
    Vector2D orientation = Vector2D(0, 1);
    
    int height = 32;
    int width = 32;
    float scale = 1;
    
    float speed = 3;
    
    
    TransformComponent()
    {
        position.x = 80;
        position.y = 1100;
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
    
    TransformComponent(int w, int h, float sc)
    {
        position.x = 80;
        position.y = 1100;
        height = h;
        width = w;
        scale = sc;
    }
    
    TransformComponent(float x, float y, int w, int h, float sc)
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
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
    
    void normalizeVelocity()
    {
        velocity.Normalize();
    }
};
