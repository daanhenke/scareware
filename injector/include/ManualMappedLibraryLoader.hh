#pragma once

#include "ILibraryLoader.hh"

namespace sw::injector
{
    class ManualMappedLibraryLoader : public ILibraryLoader
    {
    public:
        ManualMappedLibraryLoader(std::filesystem::path dll_path);
        bool AttemptInjection(DWORD process_id);
    };
}