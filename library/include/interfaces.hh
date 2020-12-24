#pragma once

#include <Windows.h>

#include "iface/IBaseClientDLL.hh"
#include "iface/ICvar.hh"
#include "iface/IPanel.hh"

namespace sw::interfaces
{
    bool FindInterfaces();

    extern iface::IBaseClientDLL* IBaseClientDLL;
    extern iface::ICvar* ICvar;
    extern iface::IPanel* IPanel;
}