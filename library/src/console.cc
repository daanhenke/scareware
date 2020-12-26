#include "console.hh"
#include <stdio.h>

HANDLE _conHandle;

void sw::console::Create()
{
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    _conHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void sw::console::WriteFormat(const char* format_str, ...)
{
    va_list list;
    
    va_start(list, format_str);
    vprintf(format_str, list);
    va_end(list);
}

void sw::console::SetColors(WORD colors)
{
    SetConsoleTextAttribute(_conHandle, colors);
}

void sw::console::ResetColors()
{
    SetConsoleTextAttribute(_conHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void sw::console::WriteColorFormat(WORD colors, const char* format_str, ...)
{
    SetColors(colors);

    va_list list;
    
    va_start(list, format_str);
    vprintf(format_str, list);
    va_end(list);

    ResetColors();
}

void sw::console::Destroy()
{
    FreeConsole();
}