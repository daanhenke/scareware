#pragma once
#include <Windows.h>

namespace sw::console
{
    void Create();
    void WriteFormat(const char* format_str, ...);
    void SetColors(WORD colors);
    void ResetColors();
    void WriteColorFormat(WORD colors, const char* format_str, ...);
    void Destroy();
}