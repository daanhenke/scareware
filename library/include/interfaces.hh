#pragma once

#include <Windows.h>

#include "iface/IBaseClientDLL.hh"
#include "iface/ICvar.hh"
#include "iface/IPanel.hh"
#include "iface/ISurface.hh"
#include "iface/IInputSystem.hh"
#include "iface/ClientModeShared.hh"
#include "iface/IVEngineClient.hh"
#include "iface/IClientEntityList.hh"
#include "iface/CInput.hh"
#include "iface/CGlowObjectManager.hh"
#include "iface/IMaterialSystem.hh"
#include "iface/IMaterial.hh"
#include "iface/ConVar.hh"
#include "iface/IGameEventManager2.hh"
#include "iface/CGlobalVars.hh"
#include "iface/IVModelRender.hh"
#include "iface/IStudioRender.hh"
#include "iface/IVDebugOverlay.hh"
#include "iface/CPrediction.hh"
#include "iface/IGameMovement.hh"
#include "iface/IEngineTrace.hh"

namespace sw::interfaces
{
    bool FindInterfaces();

    extern iface::IBaseClientDLL* IBaseClientDLL;
    extern iface::ICvar* ICvar;
    extern iface::IPanel* IPanel;
    extern iface::ISurface* ISurface;
    extern iface::IInputSystem* IInputSystem;
    extern iface::ClientModeShared* ClientModeShared;
    extern iface::IVEngineClient* IVEngineClient;
    extern iface::IClientEntityList* IClientEntityList;
    extern iface::IClientEntity** LocalPlayer;
    extern iface::CInput* CInput;
    extern iface::CGlowObjectManager* CGlowObjectManager;
    extern iface::IMaterialSystem* IMaterialSystem;
    extern iface::IGameEventManager2* IGameEventManager2;
    extern iface::CGlobalVars* CGlobalVars;
    extern iface::IVModelRender* IVModelRender;
    extern iface::IStudioRender* IStudioRender;
    extern iface::IVDebugOverlay* IVDebugOverlay;
    extern iface::CPrediction* CPrediction;
    extern iface::IGameMovement* IGameMovement;
    extern iface::IEngineTrace* IEngineTrace;

    iface::IClientEntity* GetLocalPlayer();
}