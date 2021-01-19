#pragma once

#include <Windows.h>
#include <string>

namespace sw::memory
{
    DWORD WaitForModule(const char* module_name);
    DWORD FindTextInModule(const char* module_name, const char* text);
    DWORD FindPattern(const char* module_name, std::string pattern);
}