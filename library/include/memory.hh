#pragma once

#include <Windows.h>

namespace sw::memory
{
    DWORD WaitForModule(const char* module_name);
    DWORD FindTextInModule(const char* module_name, const char* text);
}