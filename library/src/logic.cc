#include <Windows.h>
#include "logic.hh"
#include "hooks.hh"
#include "console.hh"
#include "interfaces.hh"
#include "netvars.hh"
#include "memory.hh"
#include "config.hh"
#include "hacks/chams.hh"
#include "draw.hh"

/*
    Unloads our code, since it isn't a real module anyway all we need to do is free the memory we use
    This is quite tricky since freeing it will mean the return instruction of VirtualFree will crash our game because the function it came from doesn't exist anymore
    We solve this by creating a new thread and manipulating it's stack so it calls VirtualFree & ExitThread without having to jump back to our code
*/
DWORD WINAPI HackyUnload(LPVOID lpParam)
{
    __asm {
        // Call sleep so we don't unload our cheat while a hook is still running
        push 2000
        call Sleep

        // Put the argument for ExitThread on the stack (status code, so we'll keep it at 0)
        push 0

        // Put each of VirtualFree's arguments onto the stack, VirtualFree uses __stdcall so args go from right to left
        push MEM_RELEASE
        push 0
        mov eax, sw::logic::ModuleBase
        push eax
        
        // Put the address of ExitThread on the stack so VirtualFree will return to it
        mov eax, ExitThread
        push eax

        // Instead of calling VirtualFree we jump to it,
        // meaning VirtualFree will actually return to ExitThread instead since it should be on the top of the stack
        jmp VirtualFree
    }
}

void sw::logic::UnloadSelf()
{
    console::WriteFormat("Goodbye!\n");
    auto crosshair = interfaces::ICvar->FindVar("crosshair");
    if (crosshair) crosshair->SetValue(1);
    hooks::UnhookAll();
    console::Destroy();

    CreateThread(nullptr, 0, HackyUnload, nullptr, 0, nullptr);
}

void sw::logic::Initialize()
{
    console::Create();
    config::UseConfig("~/vaporware.yml");
    interfaces::FindInterfaces();
    memory::FindRandomPtrs();
    netvars::manager = new sw::netvars::NetvarManager();
    draw::Initialize();
    hooks::HookAll();
    hacks::chams::Initialize();
}

uintptr_t sw::logic::ModuleBase;