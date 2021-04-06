#pragma once

#include "iface/common.hh"
#include "iface/CBaseHudChat.hh"

namespace sw::iface
{
    class ClientModeShared
    {
    public:
        auto GetHudChat()
        {
            return *reinterpret_cast<CBaseHudChat**>(uintptr_t(this) + 28);
        }
    };
}