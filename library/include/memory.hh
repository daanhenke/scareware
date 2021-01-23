#pragma once

#include <Windows.h>
#include <string>

namespace sw::memory
{
    template<typename T>
    constexpr T ToAbsolute(uintptr_t address)
    {
        return reinterpret_cast<T>(address + 4 + *reinterpret_cast<std::int32_t*>(address));
    }

    DWORD WaitForModule(const char* module_name);
    DWORD FindTextInModule(const char* module_name, const char* text);
    DWORD FindPattern(const char* module_name, std::string pattern);

    void FindRandomPtrs();
    extern uintptr_t CameraThinkPtr;
    extern std::add_pointer_t<void __fastcall(const char*)> LoadSky;
}