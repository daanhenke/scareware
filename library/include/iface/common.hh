#pragma once

#include "netvars.hh"

namespace sw::iface
{
    typedef void* (__cdecl* CreateInterfaceFn)(const char*, int*);

    template<typename T>
    __forceinline static T GetVirtualFunction(const void* instance, size_t index)
    {
        int* vtablePointer = *(int**) instance;
        int address = vtablePointer[index];
        return (T) (address);
    }

    template<typename T, std::size_t index, typename... ArgsT>
    constexpr T CallVirtualFunction(void* instance, ArgsT... args)
    {
        return (*reinterpret_cast<T (__thiscall***)(void*, ArgsT...)>(instance))[index](instance, args...);
    }
}

#define DEFINE_MEMBER(name, returnType, ...) typedef returnType (name)(void*, __VA_ARGS__ )
#define CALL_MEMBER(name, instance, index, ...) sw::iface::GetVirtualFunction<name>(instance, index)(instance, __VA_ARGS__)

#define LAZY_MEMBER(name, returnType, funcArgs, index, innerArgs) \
returnType name funcArgs \
{\
    return sw::iface::CallVirtualFunction<returnType, index> innerArgs; \
}

#define DEFINE_MEMBER_GETTER(name, returnType) typedef returnType (name)(void*)
#define CALL_MEMBER_GETTER(name, instance, index) sw::iface::GetVirtualFunction<name>(instance, index)(instance)