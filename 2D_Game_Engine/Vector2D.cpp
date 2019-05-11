#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D()
{
    Vector2D(0.0f, 0.0f);
}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D &Vector2D::Add(const Vector2D &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector2D &Vector2D::Subtract(const Vector2D &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2D &Vector2D::Multiply(const Vector2D &vec)
{
    this->x *= vec.x;
    this->y *= vec.y;
    return *this;
}

Vector2D &Vector2D::Divide(const Vector2D &vec)
{
    this->x /= vec.x;
    this->y /= vec.y;
    return *this;
}

Vector2D &operator+(Vector2D &v1, const Vector2D &v2)
{
    return v1.Add(v2);
}

Vector2D &operator-(Vector2D &v1, const Vector2D &v2)
{
    return v1.Subtract(v2);
}

Vector2D &operator*(Vector2D &v1, const Vector2D &v2)
{
    return v1.Multiply(v2);
}

Vector2D &operator/(Vector2D &v1, const Vector2D &v2)
{
    return v1.Divide(v2);
}

Vector2D &Vector2D::operator+=(const Vector2D &vec)
{
    return this->Add(vec);
}

Vector2D &Vector2D::operator-=(const Vector2D &vec)
{
    return this->Subtract(vec);
}

Vector2D &Vector2D::operator*=(const Vector2D &vec)
{
    return this->Multiply(vec);
}

Vector2D &Vector2D::operator/=(const Vector2D &vec)
{
    return this->Divide(vec);
}

Vector2D &Vector2D::operator*(const int &i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

Vector2D &Vector2D::Zero()
{
    this->x = this-> y = 0;
    return *this;
}

void Vector2D::Normalize()
{
    float norm = std::sqrt(this->x * this->x + this->y * this->y);
    if (norm == 0.0f)
    {
        Vector2D::Clear();
        return;
    }
    this->x /= norm;
    this->y /= norm;
}

void Vector2D::Clear()
{
    this->x = 0;
    this->y = 0;

}

std::ostream &operator<<(std::ostream &stream, const Vector2D &vec)
{
    stream << "(" << vec.x << ", " << vec.y << ")";
    return stream; 
}
