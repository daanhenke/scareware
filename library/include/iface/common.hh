#pragma once

namespace sw::iface
{
    typedef void* (__cdecl* CreateInterfaceFn)(const char*, int*);

    inline void**& GetVTable(void* instance, size_t offset = 0)
    {
        return *reinterpret_cast<void***>((size_t) instance + offset);
    }

    inline const void** GetVTable(const void* instance, size_t offset = 0)
    {
        return *reinterpret_cast<const void***>((size_t) instance + offset);
    }

    template<typename T>
    inline T GetVirtualFunction(const void* instance, size_t index, size_t offset = 0)
    {
        return reinterpret_cast<T>(GetVTable(instance, offset)[index]);
    }
}
