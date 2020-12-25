#include <Windows.h>
#include "interfaces.hh"
#include "console.hh"
#include "hooks.hh"

void Initialize()
{
    sw::console::Create();
    sw::interfaces::FindInterfaces();
    sw::hooks::HookAll();
    
    sw::console::Destroy();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpData)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            Initialize();
            break;
    }
    return 0;
}
