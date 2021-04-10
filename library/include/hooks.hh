#pragma once

#include "VTableHook.hh"
#include "iface/CUserCmd.hh"
#include "iface/FrameStage.hh"
#include "iface/IGameEvent.hh"
#include "iface/ModelRender.hh"

#include <d3d9.h>

namespace sw::hooks
{
    extern vtable::VTableHook* IBaseClientDLL;
    extern vtable::VTableHook* IPanel;
    extern vtable::VTableHook* ClientModeShared;
    extern vtable::VTableHook* IVModelRender;
    extern vtable::VTableHook* SvCheats;
    extern vtable::VTableHook* IGameEventManager2;
    extern vtable::VTableHook* IVModelRender;
    extern vtable::VTableHook* IDirect3DDevice9;

    typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
    typedef bool(__thiscall* CreateMoveFn)(void*, float flInputSampleTime, iface::CUserCmd* cmd);
    typedef void(__fastcall* FrameStageNotifyFn)(void* _this, int edx, iface::FrameStage stage);
    typedef bool(__thiscall* SvCheatsGetFn)(void* _this);
    typedef bool(__fastcall* FireEventsClientSideFn)(void* _this, int edx, iface::IGameEvent* event);
    typedef void(__thiscall* DrawModelExecuteFn)(void* _this, void* ctx, void* state, iface::ModelRenderInfo& info, iface::matrix3x4* customBoneToWorld);
    typedef long(__stdcall* EndSceneFn)(::IDirect3DDevice9* device);
    typedef long(__stdcall* ResetFn)(::IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
    extern DrawModelExecuteFn oDrawModelExecute;

    void HookAll();
    void UnhookAll();
}