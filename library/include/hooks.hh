#pragma once

#include "VTableHook.hh"
#include "iface/CUserCmd.hh"
#include "iface/FrameStage.hh"

namespace sw::hooks
{
    extern vtable::VTableHook* IBaseClientDLL;
    extern vtable::VTableHook* IPanel;
    extern vtable::VTableHook* ClientModeShared;
    extern vtable::VTableHook* IVModelRender;
    extern vtable::VTableHook* SvCheats;

    typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
    typedef bool(__thiscall* CreateMoveFn)(void*, float flInputSampleTime, iface::CUserCmd* cmd);
    typedef void(__fastcall* FrameStageNotifyFn)(void* _this, int edx, sw::iface::FrameStage stage);
    typedef bool(__thiscall* SvCheatsGetFn)(void* _this);

    void HookAll();
    void UnhookAll();
}