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

    iface::IClientEntity* GetLocalPlayer();
}