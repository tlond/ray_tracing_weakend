#pragma once

#include "math.hpp"
#include "ray.h"
#include "Matrix44.h"
#include "Vec4.h"

//Here's how you can transform the orthographic view volume to the canonical view volume step by step: 
// 1. The first step is to translate the orthographic view volume so that its center is at the origin (0, 0, 0)
// 2. Uniform Scaling (S):
// After the translation, you apply a uniform scaling operation to the view volume.
// This scaling ensures that the dimensions of the view volume match the dimensions of the canonical view volume.
//
// S is the scale matrix with scale factors a, b, and c on its diagonal.
// T is the translation matrix that translates by x, y, and z along the x, y, and z axes,
// M is the combined matrix resulting from the multiplication of S and T
// , which first applies the translation T and then the scaling S. 
//       S        *       T        =       M
// | a  0  0  0 |   | 1  0  0  x |   | a  0  0  ax |
// | 0  b  0  0 | * | 0  1  0  y | = | 0  b  0  by |
// | 0  0  c  0 |   | 0  0  1  z |   | 0  0  c  cz |
// | 0  0  0  1 |   | 0  0  0  1 |   | 0  0  0   1 |
//
// Inverse Transformation (M^(-1)):
// M^(-1) = T^(-1) * S^(-1)

// | 1  0  0  -x |   | 1/a  0    0    0 |   | 1/a  0    0    -x |
// | 0  1  0  -y | * | 0    1/b  0    0 | = | 0    1/b  0    -y |
// | 0  0  1  -z |   | 0    0    1/c  0 |   | 0    0    1/c  -z |
// | 0  0  0   1 |   | 0    0    0    1 |   | 0    0    0     1 |
//
//The general form of an orthographic projection matrix in a right-handed coordinate system 
// | 2 / (right - left)     0                     0                - (right + left) / (right - left) |
// | 0                      2 / (top - bottom)    0                - (top + bottom) / (top - bottom) |
// | 0                      0                     2 / (n - f)      - (n+f) / (n-f)                   |
// | 0                      0                     0                1                                 |
//
// Here's the simplified form of the orthographic projection matrix in this case:
// !!!! n and f has absolute values
// | 1 / r     0         0          0       |
// | 0         1 / t     0          0       |
// | 0         0         2 / (f - n)   - (f + n) / (f - n) |
// | 0         0         0          1       |
//
// Inverse orthographic projection
    // | r         0         0          0       |
    // | 0         t         0          0       |
    // | 0         0         (f - n)/2    (f + n)/2|
    // | 0         0         0          1       |
// r = |n| * aspectRatio * tg(O/2); t = |n| * tg(O/2);
//
// Perspective
// | n         0         0          0       |
// | 0         n         0          0       |
// | 0         0         n + f    -(f * n)  |
// | 0         0         1          0       |
//
// Inverse Perspective !! not quite you will end up with fn*I (I - identety matrix)
// | f         0         0          0       |
// | 0         f         0          0       |
// | 0         0         0        (f * n)   |
// | 0         0         -1        n + f    |




class Camera
{
public:
    Camera(const Vec3 &_position, const Vec3 &_target, const Vec3 &_up,
           float _fov, float _aspectRatio, float nearPlane = 0.1f, float farPlane = 100.f)
        : position(_position), target(_target), up(_up), fov(_fov), aspectRatio(_aspectRatio)
    {
        // for now just inerest in inverse of perspective projection matrix 
        // P^(-1) =InverseOrtoProj * InversPerspective 
        
        const auto theta = (fov * 3.1415926535897932385f / 180.f);
        const auto r = nearPlane * aspectRatio* tan(0.5f * theta);
        const auto t = nearPlane * tan(0.5f * theta);
        const auto f = farPlane;
        const auto n = nearPlane;
        InverseProjection.data[0] = f * r;
        InverseProjection.data[5] = f * t;
        InverseProjection.data[11] = -(f * n); //???? why - signd (f*n);
        InverseProjection.data[14] = (n - f) / 2.f;
        InverseProjection.data[15] = (n + f) / 2.f;

        lookAt(position, target, up);
    }

   void updateMatrices() {
        // Calculate the world-to-camera matrix
        Matrix4x4 translationMatrix = Mat4x4Translation(-position.x, -position.y, -position.z);
        Matrix4x4 rotationMatrix = Mat4x4FromAxes(right, up, zaxis);
        worldToCamera = mat4x4_mul(rotationMatrix, translationMatrix);

        translationMatrix = Mat4x4Translation(position.x, position.y, position.z);
        // Calculate the camera-to-world matrix
        cameraToWorld = mat4x4_mul(translationMatrix, rotationMatrix.transpose());

        auto test = mat4x4_mul(worldToCamera, cameraToWorld);
    }

    // Update camera orientation to look at a target position
    void lookAt(const Vec3& eye, const Vec3& at, const Vec3& upVec) {
        position = eye;

        forward = normalize(at - eye);
        zaxis = normalize(eye - at);
        auto xaxis = normalize(cross(upVec, zaxis));
        auto yaxis = cross(zaxis, xaxis);

        right = xaxis;
        up = yaxis;


          // Determine viewport dimensions.
        auto focal_length = (eye - at).length();
        auto theta = (fov * 3.1415926535897932385f / 180.f);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * aspectRatio;

        // Update matrices
        updateMatrices();
    }

    auto ToWorld(const Vec3 &direction) const
    {
        Vec3 rayDirectionWorld = (cameraToWorld * Vec4(direction, 0.0f)).xyz();
        return normalize(rayDirectionWorld);
    }

    auto WorldToCamera(const Vec3 &point) const
    {
          return (worldToCamera * Vec4(point, 1.f)).xyz();
    }

    Ray generateRay(float ndcX, float ndcY) const
    {
        // // Convert NDC coordinates to camera space
        // float halfFovTan = tan(0.5f * fov * 3.14159265358979323846f / 180.0f);
        // float cameraPlaneX = halfFovTan * ndcX * aspectRatio;
        // float cameraPlaneY = halfFovTan * ndcY;

        // // Calculate ray direction in camera space
        // auto x = cameraPlaneX * right;
        // auto y = cameraPlaneY * up;
        // auto z = forward;
        // Vec3 rayDirection = normalize(x + y + z);
        
        auto rayDirection = InverseProjection * Vec4{ndcX, ndcY, 0.f, 1.f};
        rayDirection /= rayDirection.w;
        return Ray({0.f,0.f,0.f}, normalize(rayDirection.xyz()));
    }

    Ray generateRay(const Vec2f &point) const
    {
        return generateRay(point.x, point.y);
    }

    Ray generateWorldRay(const Vec2f &point) const
    {
        const auto cameraRay = generateRay(point);
        return Ray(position, ToWorld(cameraRay.direction()));
    }

private:
    Vec3 Right() const
    {
        return right;
    }

private:
    Vec3 position;
    Vec3 target;
    Vec3 up;
    Vec3 right;
    Vec3 forward;
    Vec3 zaxis;

    float fov;
    float aspectRatio;
    Matrix4x4 cameraToWorld;
    Matrix4x4 worldToCamera;

    Matrix4x4 InverseProjection;
};
