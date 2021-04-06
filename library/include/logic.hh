#pragma once

#include <stdint.h>

namespace sw::logic
{
    void Initialize();
    void UnloadSelf();

    extern uintptr_t ModuleBase;
}