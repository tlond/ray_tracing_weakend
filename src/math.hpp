#pragma once

#include <cmath>

template <class T>
struct TVec2
{
    T x, y;
    TVec2() : x(T{}), y(T{}) {}
    TVec2(T x) : x(x), y(x) {}
    TVec2(T x, T y) : x(x), y(y) {}

    auto operator-(const TVec2 &other) const
    {
        return TVec2(x - other.x, y - other.y);
    }

    auto operator+(const TVec2 &other) const
    {
        return TVec2(x + other.x, y + other.y);
    }

    template <class U>
    auto operator*(U scalar) const
    {
        return TVec2(x * scalar, y * scalar);
    }

    template <class U>
    auto operator/(U scalar) const
    {
        return TVec2(x / scalar, y / scalar);
    }

    auto operator-() const {
        return TVec2(-x, -y);
    }

    T length() const
    {
        return sqrt(length_squared());
    }

    T length_squared() const
    {
        return x * x + y * y;
    }
};

// Define a simple 3D vector class
struct Vec3
{
    float x, y, z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Define the negation operator (-)
    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }

    Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 &operator+=(const Vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3 &operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    template <class U>
    Vec3 operator*(U scalar) const
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    template <class U>
    Vec3 operator/(U scalar) const
    {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    float length() const
    {
        return sqrt(length_squared());
    }

    float length_squared() const
    {
        return x * x + y * y + z * z;
    }
};

template <class V>
inline V unit_vector(const V& v)
{
    const auto length = v.length();
    if (length > 0)
        return v / v.length();
    
    return V{};
}

template <class V>
inline V normalize(const V& v)
{
    return unit_vector(v);
}

float dot(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <class T>
auto dot(const TVec2<T> &a, const TVec2<T> &b)
{
    return a.x * b.x + a.y * b.y;
}


Vec3 normalize(const Vec3 &v){
    return unit_vector(v);
}

Vec3 cross(const Vec3 &a, const Vec3 &b)
{
    return Vec3(a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

template <class U>
Vec3 operator*(U scalar, const Vec3 &vec)
{
    return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

template <class U, class T>
auto operator*(U scalar, const TVec2<T> &vec)
{
    return TVec2<T>(vec.x * scalar, vec.y * scalar);
}

using Vec2f = TVec2<float>;
using Vec2d = TVec2<double>;
using Vec2i = TVec2<int>;