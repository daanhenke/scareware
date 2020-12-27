#pragma once

#include "iface/common.hh"
#include "iface/Vector.hh"
#include "iface/IClientUnknown.hh"
#include "iface/IClientRenderable.hh"
#include "iface/IClientNetworkable.hh"
#include "iface/IClientThinkable.hh"

namespace sw::iface
{
    class __declspec (novtable) IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
    {
    public:
        Vector GetEyePosition()
        {
            DEFINE_MEMBER_GETTER(__thiscall* GetEyePosition_t, Vector);
            return CALL_MEMBER_GETTER(GetEyePosition_t, this, 284);
        }
    };
}