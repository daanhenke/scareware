#pragma once

#include "iface/basetypes.hh"

namespace sw::iface
{
    class Vector
    {
    public:
        vec_t x, y, z;

        Vector(vec_t x = 0, vec_t y = 0, vec_t z = 0) : x(x), y(y), z(z) {};
    };
}