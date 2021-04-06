#include <Windows.h>
#include "logic.hh"
#include <stdint.h>


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpBaseAddress)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            sw::logic::ModuleBase = (uintptr_t) lpBaseAddress;
            sw::logic::Initialize();
            break;
    }
    return 0;
}
