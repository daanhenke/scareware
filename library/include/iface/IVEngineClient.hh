#pragma once

#include "iface/common.hh"

namespace sw::iface
{
    class IVEngineClient
    {
    public:
        bool IsInGame()
        {
            DEFINE_MEMBER_GETTER(__thiscall* IsInGame_t, bool);
            return CALL_MEMBER_GETTER(IsInGame_t, this, 26);
        }
    };
}