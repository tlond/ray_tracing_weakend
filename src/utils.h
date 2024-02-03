#pragma once

#include "math.hpp"
#include <random>
// https://www.scratchapixel.com/lessons/
//         Raster Space                            NDC Space                                Screen Space
//  +----------+----------+----------+   +----------+----------+----------+   +----------+----------+----------+
//  |   (0,0)  |          |   (2,0)  |   |   (0,0)  |          |    (1,0) |   |   (-1,1) |          |    (1,1) |
//  |          |          |          |   |          |          |          |   |          |          |          |
//  +----------+----------+----------+   +----------+----------+----------+   +----------+----------+----------+
//  |          |          |          |   |          |          |          |   |          |          |          |
//  |   (0,1)  |          |   (2,1)  |   |          |          |          |   |   (-1,0) |          |    (1,0) |
//  +----------+----------+----------+   +----------+----------+----------+   +----------+----------+----------+
//  |          |          |          |   |          |          |          |   |          |          |          |
//  |   (0,2)  |          |   (2,2)  |   |   (0,1)  |          |    (1,1) |   |   (-1,-1)|          |    (1,-1)|
//  +----------+----------+----------+   +----------+----------+----------+   +----------+----------+----------+

auto raster_to_NDC(Vec2f point, int img_width, int img_height)
{
    return Vec2f((point.x + 0.5f) / img_width, (point.x + 0.5f) / img_height);
}

auto raster_to_NDC(Vec2i point, int img_width, int img_height)
{
    return Vec2f((point.x + 0.5f) / img_width, (point.y + 0.5f) / img_height);
}

auto NDC_to_screen_space(Vec2f point)
{
    return Vec2f(2 * point.x - 1, 1 - 2 * point.y);
}

Vec3 generate_random_vec3(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);
    std::uniform_real_distribution<float> distZ(minZ, maxZ);

    float randomX = distX(rng);
    float randomY = distY(rng);
    float randomZ = distZ(rng);

    return Vec3(randomX, randomY, randomZ);
}


inline Vec3 random_in_unit_sphere() {
    while (true) {
        auto p = generate_random_vec3(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
        if (p.length_squared() < 1)
            return p;
    }
}

inline Vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline Vec3 random_on_hemisphere(const Vec3& normal) {
    Vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

bool near_zero(const Vec3& v, float epsilon = 1e-8) {
    return (std::fabs(v.x) < epsilon) && (std::fabs(v.y) < epsilon) && (std::fabs(v.z) < epsilon);
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.f);
    Vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vec3 r_out_parallel = -sqrt(fabs(1.f - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}


inline auto degrees_to_radians(float degrees) {
    return degrees * 3.1415926535897932385f / 180.f;
}

namespace gpt
{
    Vec2f rasterToNDC(int x, int y, int width, int height)
    {
        // Convert raster coordinates to NDC coordinates
        float ndcX = (2.0f * x) / (width - 1) - 1.0f;
        float ndcY = 1.0f - (2.0f * y) / (height - 1);

        return Vec2f(ndcX, ndcY);
    }

    Vec2f rasterToNDC(const Vec2i &pixel, int width, int height)
    {
        // Convert raster coordinates to NDC coordinates
        return rasterToNDC(pixel.x, pixel.y, width, height);
    }
    
    Vec2f ndcToScreen(const Vec2f &ndc, int screenWidth, int screenHeight)
    {
        // Convert NDC coordinates to screen space
        float screenX = (ndc.x + 1.0f) * 0.5f * screenWidth;
        float screenY = (1.0f - ndc.y) * 0.5f * screenHeight;

        return Vec2f(screenX, screenY);
    }
} // namespace gpt