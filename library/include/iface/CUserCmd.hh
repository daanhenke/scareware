#pragma once

#include <Windows.h>

#include "QAngle.hh"
#include "Vector.hh"

namespace sw::iface
{
    enum
    {
        IN_ATTACK = 1 << 0,
        IN_JUMP = 1 << 1,
        IN_DUCK = 1 << 2,
        IN_FORWARD = 1 << 3,
        IN_BACK = 1 << 4,
        IN_USE = 1 << 5,
        IN_MOVELEFT = 1 << 9,
        IN_MOVERIGHT = 1 << 10,
        IN_ATTACK2 = 1 << 11,
        IN_SCORE = 1 << 16,
        IN_BULLRUSH = 1 << 22
    };
    
    class CUserCmd
    {
    public:
        int command_number;
        int tick_count;
        QAngle viewangles;
        Vector aimdirection;
        float forwardmove;
        float sidemove;
        float upmove;
        int buttons;
        BYTE impulse;
        int weaponselect;
        int weaponsubtype;
        int random_seed;
        short mousedx;
        short mousedy;
        bool hasbeenpredicted;
    };
}