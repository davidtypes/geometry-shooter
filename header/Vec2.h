//
// Created by David Valenzuela on 12/28/23.
//

#ifndef TRYINGSFML_VEC2_H
#define TRYINGSFML_VEC2_H


class Vec2
{
public:
    float x;
    float y;

    Vec2();
    Vec2(float a, float b);

    bool operator == (const Vec2& rside) const;
    bool operator != (const Vec2& rside) const;

    Vec2 operator + (const Vec2& rside) const;
    Vec2 operator - (const Vec2& rside) const;
    Vec2 operator * (const float s) const;
    Vec2 operator / (const float s) const;

    void operator += (const Vec2& rside);
    void operator -= (const Vec2& rside);
    void operator *= (const float s);
    void operator /= (const float s);

    float length() const;
    float dist(const Vec2& rside) const;
    void normalize();
};


#endif //TRYINGSFML_VEC2_H
