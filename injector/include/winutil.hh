#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <string>

namespace sw::injector
{
    DWORD GetProcessId(std::string process_name);
}
