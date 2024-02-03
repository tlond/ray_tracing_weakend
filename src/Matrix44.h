#pragma once

#include "Vec4.h"

class Matrix4x4
{
public:
    float data[16];

    Matrix4x4()
    {
        for (int i = 0; i < 16; ++i)
        {
            data[i] = 0.0f;
        }
    }

    // Matrix4x4(const Vec4& col1, const Vec4& col2, const Vec4& col3, const Vec4& col4) {
    //     data[0] = col1.x; data[4] = col2.x; data[8] = col3.x; data[12] = col4.x;
    //     data[1] = col1.y; data[5] = col2.y; data[9] = col3.y; data[13] = col4.y;
    //     data[2] = col1.z; data[6] = col2.z; data[10] = col3.z; data[14] = col4.z;
    //     data[3] = col1.w; data[7] = col2.w; data[11] = col3.w; data[15] = col4.w;
    // }

    // Vec4 operator*(const Vec4& v) const {
    //     return Vec4(
    //         data[0] * v.x + data[4] * v.y + data[8] * v.z + data[12] * v.w,
    //         data[1] * v.x + data[5] * v.y + data[9] * v.z + data[13] * v.w,
    //         data[2] * v.x + data[6] * v.y + data[10] * v.z + data[14] * v.w,
    //         data[3] * v.x + data[7] * v.y + data[11] * v.z + data[15] * v.w
    //     );
    // }

    Vec4 operator*(const Vec4 &v) const
    {
        return Vec4(
            data[0] * v.x + data[1] * v.y + data[2] * v.z + data[3] * v.w,
            data[4] * v.x + data[5] * v.y + data[6] * v.z + data[7] * v.w,
            data[8] * v.x + data[9] * v.y + data[10] * v.z + data[11] * v.w,
            data[12] * v.x + data[13] * v.y + data[14] * v.z + data[15] * v.w);
    }

    Matrix4x4 transpose() const
    {
        Matrix4x4 result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.data[i * 4 + j] = data[j * 4 + i];
            }
        }

        return result;
    }
};

Matrix4x4 Mat4x4Translation(float x, float y, float z)
{
    Matrix4x4 result;

    // Initialize the matrix as an identity matrix
    for (int i = 0; i < 16; ++i)
    {
        result.data[i] = 0.0f;
    }
    result.data[0] = 1.0f;
    result.data[5] = 1.0f;
    result.data[10] = 1.0f;
    result.data[15] = 1.0f;

    // Set the translation components in the last column
    result.data[3] = x;
    result.data[7] = y;
    result.data[11] = z;

    return result;
}

// Matrix4x4 Mat4x4Translation(float x, float y, float z) {
//     Matrix4x4 result;

//     // Initialize the matrix as an identity matrix
//     for (int i = 0; i < 16; ++i) {
//         result.data[i] = 0.0f;
//     }
//     result.data[0] = 1.0f;
//     result.data[5] = 1.0f;
//     result.data[10] = 1.0f;
//     result.data[15] = 1.0f;

//     // Set the translation components
//     result.data[12] = x;
//     result.data[13] = y;
//     result.data[14] = z;

//     return result;
// }

Matrix4x4 Mat4x4FromAxes(const Vec3 &xAxis, const Vec3 &yAxis, const Vec3 &zAxis)
{
    Matrix4x4 result;

    result.data[0] = xAxis.x;
    result.data[1] = xAxis.y;
    result.data[2] = xAxis.z;
    result.data[3] = 0.0f;

    result.data[4] = yAxis.x;
    result.data[5] = yAxis.y;
    result.data[6] = yAxis.z;
    result.data[7] = 0.0f;

    result.data[8] = zAxis.x;
    result.data[9] = zAxis.y;
    result.data[10] = zAxis.z;
    result.data[11] = 0.0f;

    result.data[12] = 0.0f;
    result.data[13] = 0.0f;
    result.data[14] = 0.0f;
    result.data[15] = 1.0f;

    return result;
}

Matrix4x4 mat4x4_mul(const Matrix4x4 &A, const Matrix4x4 &B)
{
    Matrix4x4 result;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.data[i * 4 + j] = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                result.data[i * 4 + j] += A.data[i * 4 + k] * B.data[k * 4 + j];
            }
        }
    }

    return result;
}
