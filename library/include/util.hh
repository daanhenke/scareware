#pragma once

#include "iface/Vector2D.hh"
#include "iface/Vector.hh"
#include "iface/VMatrix.hh"
#include "interfaces.hh"
#include "iface/Color.hh"
#include <Windows.h>
#include <locale>
#include <codecvt>

#include <intrin.h>
#pragma intrinsic(_ReturnAddress)
#define RETURN_ADDR() reinterpret_cast<uintptr_t>(_ReturnAddress());

namespace sw::util
{
    bool WorldToScreen(iface::Vector& point, iface::Vector& screen);

    void VectorAngles(iface::Vector& forward, iface::Vector& angles);
    iface::Vector CalcAngle(iface::Vector source, iface::Vector destination);
    iface::Vector ClampAngle(sw::iface::Vector angle);

    iface::Color  MixColors(iface::Color a, iface::Color b, float mix);

    extern std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wstring_converter;
}