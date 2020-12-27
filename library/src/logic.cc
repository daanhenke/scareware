#include <Windows.h>
#include "logic.hh"
#include "hooks.hh"
#include "console.hh"

/*
    Unloads our code, since it isn't a real module anyway all we need to do is free the memory we use
    This is quite tricky since freeing it will mean the return instruction of VirtualFree will crash our game because the function it came from doesn't exist anymore
    We solve this by creating a new thread and manipulating it's stack so it calls VirtualFree & ExitThread without having to jump back to our code
*/
DWORD WINAPI HackyUnload(LPVOID lpParam)
{
    __asm {
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
    hooks::UnhookAll();
    console::Destroy();

    CreateThread(nullptr, 0, HackyUnload, nullptr, 0, nullptr);
}

uintptr_t sw::logic::ModuleBase;