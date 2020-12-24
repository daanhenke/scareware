#pragma once

#include <filesystem>
#include <Windows.h>

namespace sw::injector
{
    class ILibraryLoader
    {
    public:
        std::filesystem::path m_dll_path;

        ILibraryLoader(std::filesystem::path dll_path)
        {
            m_dll_path = dll_path;
        }
        
        virtual bool AttemptInjection(DWORD process_id) = 0;
    };
}