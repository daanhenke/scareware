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
}