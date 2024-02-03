#pragma once

#include "math.hpp"
class Ray
{
public:
    Ray() {}

    Ray(const Vec3 &origin, const Vec3 &direction) : orig(origin), dir(direction) {}

    Vec3 origin() const { return orig; }
    Vec3 direction() const { return dir; }

    Vec3 at(float t) const
    {
        return orig + dir * t;
    }

private:
    Vec3 orig;
    Vec3 dir;
};

