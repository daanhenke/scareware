#include <Windows.h>
#include "interfaces.hh"
#include "console.hh"
#include "hooks.hh"
#include "logic.hh"
#include "netvars.hh"
#include "memory.hh"
#include "events.hh"
#include "hacks/chams.hh"

void Initialize()
{
    sw::console::Create();
    sw::interfaces::FindInterfaces();
    sw::memory::FindRandomPtrs();
    sw::netvars::manager = new sw::netvars::NetvarManager();
    sw::hooks::HookAll();
    sw::hacks::chams::Initialize();
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
