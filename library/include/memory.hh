#pragma once

#include <Windows.h>
#include <string>
#include "iface/IViewRenderBeams.hh"
#include "iface/IMoveHelper.hh"
#include "iface/CMoveData.hh"
#include <d3d9.h>

namespace sw::iface
{
    class KeyValues;
}

namespace sw::memory
{
    template<typename T>
    constexpr T ToAbsolute(uintptr_t address)
    {
        return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
    }

    DWORD WaitForModule(const char* module_name);
    DWORD FindTextInModule(const char* module_name, const char* text);
    DWORD FindPattern(const char* module_name, std::string pattern);

    void FindRandomPtrs();
    extern uintptr_t CameraThinkPtr;
    extern std::add_pointer_t<void __fastcall(const char*)> LoadSky;

    extern uintptr_t KeyValuesFromString;
    extern iface::KeyValues* (__thiscall* KeyValuesFindKey)(iface::KeyValues* keyValues, const char* keyName, bool create);
    extern void (__thiscall* KeyValuesSetString)(iface::KeyValues* keyValues, const char* value);

    extern iface::IViewRenderBeams* IViewRenderBeams;
    extern iface::IMoveHelper* IMoveHelper;

    extern int* predictSeed;
    extern iface::CMoveData* CMoveData;

    extern IDirect3DDevice9* D3DDevice;
}