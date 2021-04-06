#pragma once

#include "iface/common.hh"

namespace sw::iface
{
    class CBaseHudChat
    {
    public:
        template <typename... Args>
        void Printf(int filter, const char* fmt, Args... args)
        {
            (*reinterpret_cast<void(__cdecl***)(void*, int, const char*, ...)>(this))[26](this, filter, fmt, args...);
        }
    };
}