#pragma once

#include "iface/common.hh"

namespace sw::iface
{
    class IClientNetworkable
    {
    public:
        bool IsDormant()
        {
            DEFINE_MEMBER_GETTER(__thiscall* IsDormant_t, bool);
            return CALL_MEMBER_GETTER(IsDormant_t, this, 8);
        }
    };
}