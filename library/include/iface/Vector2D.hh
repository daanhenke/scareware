#pragma once

#include "iface/basetypes.hh"

namespace sw::iface
{
    class Vector2D
    {
    public:
        vec_t x, y;

        Vector2D(vec_t x = 0.f, vec_t y = 0.f) : x(x), y(y)
        {}
    };
}