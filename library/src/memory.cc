#include "memory.hh"
#include <Psapi.h>

#include <map>
#include <string>

std::map<std::string, DWORD> _mod_handle_map;

// Waits for a module to be loaded and returns a handle to it once it is
DWORD sw::memory::WaitForModule(const char* module_name)
{
    // Check if the module isn't already in our internal map
    if (_mod_handle_map.find(module_name) == _mod_handle_map.end())
    {
        HANDLE handle = 0;

        // Wait until the handle is available
        while (handle == 0)
        {
            handle = LoadLibraryA(module_name);
            if (handle == 0)
            {
                Sleep(20);
            }
        }

        // Add the handle to our map
        _mod_handle_map[module_name] = (DWORD) handle;
    }

    return _mod_handle_map[module_name];
}

// Finds a pointer to a given string in a module
DWORD sw::memory::FindTextInModule(const char* module_name, const char* text)
{
    // Get the length of our string
    int textLength = strlen(text);

    // Get target module's address & size
    DWORD moduleAddress = WaitForModule(module_name);
    MODULEINFO moduleInfo;
    GetModuleInformation(GetCurrentProcess(), (HMODULE) moduleAddress, &moduleInfo, sizeof(MODULEINFO));
    DWORD moduleSize = moduleInfo.SizeOfImage;

    // Try all offsets possible incrementally
    for (DWORD offset = 0; offset < moduleSize - textLength; offset++)
    {
        // Compare the module with our string (memcmp returns 0 if buffers match)
        if (memcmp((void*)(moduleAddress + offset), text, textLength) == 0)
        {
            return moduleAddress + offset;
        }
    }


    return 0;
}
