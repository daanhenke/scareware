#pragma once

#include "VTableHook.hh"
#include "iface/CUserCmd.hh"
#include "iface/FrameStage.hh"
#include "iface/IGameEvent.hh"
#include "iface/ModelRender.hh"

namespace sw::hooks
{
    extern vtable::VTableHook* IBaseClientDLL;
    extern vtable::VTableHook* IPanel;
    extern vtable::VTableHook* ClientModeShared;
    extern vtable::VTableHook* IVModelRender;
    extern vtable::VTableHook* SvCheats;
    extern vtable::VTableHook* IGameEventManager2;
    extern vtable::VTableHook* IVModelRender;

    typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
    typedef bool(__thiscall* CreateMoveFn)(void*, float flInputSampleTime, iface::CUserCmd* cmd);
    typedef void(__fastcall* FrameStageNotifyFn)(void* _this, int edx, iface::FrameStage stage);
    typedef bool(__thiscall* SvCheatsGetFn)(void* _this);
    typedef bool(__fastcall* FireEventsClientSideFn)(void* _this, int edx, iface::IGameEvent* event);
    typedef void(__thiscall* DrawModelExecuteFn)(void* _this, void* ctx, void* state, iface::ModelRenderInfo& info, iface::matrix3x4* customBoneToWorld);
    extern DrawModelExecuteFn oDrawModelExecute;

    void HookAll();
    void UnhookAll();
}