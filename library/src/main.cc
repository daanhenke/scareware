#include <Windows.h>
#include "interfaces.hh"
#include "console.hh"
#include "hooks.hh"
#include "logic.hh"

void Initialize()
{
    sw::console::Create();
    //sw::interfaces::FindInterfaces();
    //sw::hooks::HookAll();
    
    Sleep(2000);
    MessageBoxA(nullptr, "Unloading", "Yeet", MB_OK);
    sw::logic::UnloadSelf();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpBaseAddress)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            sw::logic::ModuleBase = (uintptr_t) lpBaseAddress;
            Initialize();
            break;
    }
    return 0;
}
