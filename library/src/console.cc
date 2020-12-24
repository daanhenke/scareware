#include "console.hh"
#include <Windows.h>
#include <stdio.h>

void sw::console::Create()
{
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
}

void sw::console::WriteFormat(const char* format_str, ...)
{
    va_list list;
    
    va_start(list, format_str);
    vprintf(format_str, list);
    va_end(list);
}

void sw::console::Destroy()
{
    FreeConsole();
}