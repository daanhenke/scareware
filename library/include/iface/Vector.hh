#pragma once

#include "iface/basetypes.hh"

namespace sw::iface
{
    class Vector
    {
    public:
        vec_t x, y, z;

        Vector(vec_t _x = 0, vec_t _y = 0, vec_t _z = 0) : x(_x), y(_y), z(_z) {};
    };
}