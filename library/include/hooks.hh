#pragma once

#include "VTableHook.hh"

namespace sw::hooks
{
    extern vtable::VTableHook* IBaseClientDLL;
    extern vtable::VTableHook* IPanel;

    typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);

    void HookAll();
    void UnhookAll();
}