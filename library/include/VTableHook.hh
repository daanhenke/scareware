#pragma once

#include <Windows.h>

namespace sw::vtable
{
    class VTableHook
    {
    public:
        DWORD* m_instance;
        DWORD* m_table_old;
        DWORD* m_table_new;

        VTableHook(DWORD* instance);
        int MethodCount();
        DWORD HookMethod(DWORD new_address, int index);
        void RestoreOld();
    };
}