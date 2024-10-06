//
// Created by David Valenzuela on 12/28/23.
//

#include "../header/Vec2.h"
#include <cmath>

Vec2::Vec2()
{
    x = 0.0;
    y = 0.0;
}

Vec2::Vec2(float a, float b)
{
    x = a;
    y = b;
}

bool Vec2::operator==(const Vec2 &rside) const
{
    return (x == rside.x && y == rside.y);
}

bool Vec2::operator!=(const Vec2 &rside) const
{
    return (x != rside.x || y != rside.y);
}

Vec2 Vec2::operator+(const Vec2 &rside) const
{
    return {x + rside.x, y + rside.y};
}

Vec2 Vec2::operator-(const Vec2 &rside) const
{
    return {x - rside.x, y - rside.y};
}

Vec2 Vec2::operator*(const float s) const
{
    return {x * s, y * s};
}

Vec2 Vec2::operator/(const float s) const
{
    return {x / s, y / s};
}

void Vec2::operator+=(const Vec2 &rside)
{
    x += rside.x;
    y += rside.y;
}

void Vec2::operator-=(const Vec2 &rside)
{
    x -= rside.x;
    y -= rside.y;
}

void Vec2::operator*=(const float s)
{
    x *= s;
    y *= s;
}

void Vec2::operator/=(const float s)
{
    x /= s;
    y /= s;
}

float Vec2::length() const
{
    return sqrtf(x * x + y * y);
}

float Vec2::dist(const Vec2 &rside) const
{
    return (Vec2(rside.x - x, rside.y - y).length());
}

void Vec2::normalize()
{
    float firstlength = this->length();
    x /= firstlength;
    y /= firstlength;
}