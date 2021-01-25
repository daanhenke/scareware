#pragma once

#include "iface/basetypes.hh"
#include "iface/Vector.hh"

namespace sw::iface
{
    struct VMatrix
    {
        union {
            struct {
                vec_t _11, _12, _13, _14;
                vec_t _21, _22, _23, _24;
                vec_t _31, _32, _33, _34;
                vec_t _41, _42, _43, _44;
            };

            vec_t m[4][4];
        };

        //inline auto origin()
        //{
        //    return Vector(_14, _24, _34);
        //}
    };
}