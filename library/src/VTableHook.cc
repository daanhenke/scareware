#include "VTableHook.hh"
#include "console.hh"

sw::vtable::VTableHook::VTableHook(DWORD* instance)
{
    m_instance = instance;
    m_table_old = (DWORD*) *instance;

    // Allocate a new vtable
    int method_count = MethodCount();
    int table_size = (method_count + 1) * sizeof(DWORD);
    console::WriteFormat("Found vtable with %d entries!\n", method_count);
    m_table_new = new DWORD[method_count + 1];

    // Copy over the old vtable
    CopyMemory(m_table_new, &m_table_old[-1], table_size);

    // Use the new vtable instead
    *m_instance = (DWORD) &m_table_new[1];
}

// Helper function to calculate the size of a vtable
int sw::vtable::VTableHook::MethodCount()
{
    int index = 0;

    while (! IsBadCodePtr((FARPROC) m_table_old[index]))
    {
        index++;
    }

    return index;
}

// Overwrite a function pointer inside the vtable
DWORD sw::vtable::VTableHook::HookMethod(DWORD new_address, int index)
{
    m_table_new[1 + index] = new_address;
    return m_table_old[index];
}

// Restore the old vtable
void sw::vtable::VTableHook::RestoreOld()
{
    *m_instance = (DWORD) m_table_old;
}