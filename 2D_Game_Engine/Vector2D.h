#pragma once
#include <iostream>

class Vector2D
{
public:
    static Vector2D *ZeroVector;
    float x;
    float y;
    
    Vector2D();
    Vector2D(float x, float y);
    
    Vector2D &Add(const Vector2D &vec);
    Vector2D &Subtract(const Vector2D &vec);
    Vector2D &Multiply(const Vector2D &vec);
    Vector2D &Divide(const Vector2D &vec);
    
    Vector2D &Multiply(const float factor);
    
    friend Vector2D &operator+(Vector2D &v1, const Vector2D &v2);
    friend Vector2D &operator-(Vector2D &v1, const Vector2D &v2);
    friend Vector2D &operator*(Vector2D &v1, const Vector2D &v2);
    friend Vector2D &operator/(Vector2D &v1, const Vector2D &v2);
    
    Vector2D &operator+=(const Vector2D &vec);
    Vector2D &operator-=(const Vector2D &vec);
    Vector2D &operator*=(const Vector2D &vec);
    Vector2D &operator/=(const Vector2D &vec);
    
    friend Vector2D &operator*(Vector2D &v1, const float i);
    friend Vector2D &operator*(const float i, Vector2D &v1);
    Vector2D &operator*=(const float i);
    Vector2D *Diff(const Vector2D &vec);
    
    Vector2D &Zero();
    Vector2D *TurnTo(Vector2D &dest);
    float L2Norm();
    void Normalize();
    void Clear();

    friend std::ostream &operator<<(std::ostream &stream, const Vector2D &vec);
};
