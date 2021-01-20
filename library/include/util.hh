#pragma once

#include "iface/Vector2D.hh"
#include "iface/Vector.hh"
#include "iface/VMatrix.hh"
#include "interfaces.hh"
#include <Windows.h>

#include <intrin.h>
#pragma intrinsic(_ReturnAddress)
#define RETURN_ADDR() reinterpret_cast<uintptr_t>(_ReturnAddress());

namespace sw::util
{
    iface::Vector2D WorldToScreen(iface::Vector worldVector);
}