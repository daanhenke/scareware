#pragma once

#include "iface/basetypes.hh"
#include <math.h>
#include "iface/VMatrix.hh"

#define M_PI 3.14159265358979323846

namespace sw::iface
{
    class Vector
    {
    public:
        vec_t x, y, z;

        Vector(vec_t _x = 0, vec_t _y = 0, vec_t _z = 0) : x(_x), y(_y), z(_z) {};

        inline float Length()
        {
            float sqrtVal = x * x + y * y + z * z;
            return sqrt(sqrtVal);
        }

        inline float Length2D()
        {
            float sqrtVal = x * x + y * y;
            return sqrt(sqrtVal);
        }

        inline Vector& operator+(Vector& rhs)
        {
            return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        inline Vector& operator-(Vector& rhs)
        {
            return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        inline Vector& operator*(Vector& rhs)
        {
            return Vector(x * rhs.x, y * rhs.y, z * rhs.z);
        }

        inline Vector& operator*(float rhs)
        {
            return Vector(x * rhs, y * rhs, z * rhs);
        }

        inline float DistanceTo(const Vector& rhs)
        {
            Vector delta(x - rhs.x, y - rhs.y, z - rhs.z);

            return delta.Length();
        }

        inline auto DotProduct(const Vector& rhs)
        {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        inline auto MatrixTransform(const VMatrix& matrix)
        {
            return Vector(
                DotProduct(Vector(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2])) + matrix.m[0][3],
                DotProduct(Vector(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2])) + matrix.m[1][3],
                DotProduct(Vector(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2])) + matrix.m[2][3]
            );
        }
    };
}