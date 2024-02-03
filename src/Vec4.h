#pragma once

#include "math.hpp"

struct Vec4
{
public:
    float x, y, z, w;

    Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    // Constructor that takes a Vec3 and a float
    Vec4(const Vec3& v3, float _w ) : x(v3.x), y(v3.y), z(v3.z), w(_w) {}

    Vec3 xyz() const { return Vec3(x, y, z); }

    Vec4 &operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }
};


Vec4 normalize(const Vec4& v) {
    float lengthSq = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    if (lengthSq > 0.0f) {
        float invLength = 1.0f / sqrt(lengthSq);
        return Vec4(v.x * invLength, v.y * invLength, v.z * invLength, v.w * invLength);
    }
    return v; // Return v unchanged if its length is 0
}