#pragma once

#include "VTableHook.hh"
#include "iface/CUserCmd.hh"
#include "iface/FrameStage.hh"

namespace sw::hooks
{
    extern vtable::VTableHook* IBaseClientDLL;
    extern vtable::VTableHook* IPanel;
    extern vtable::VTableHook* ClientModeShared;

    typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
    typedef bool(__thiscall* CreateMoveFn)(void*, float flInputSampleTime, iface::CUserCmd* cmd);
    typedef void(__fastcall* FrameStageNotifyFn)(void* _this, int edx, sw::iface::FrameStage stage);

    void HookAll();
    void UnhookAll();
}