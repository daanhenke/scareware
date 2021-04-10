#include "memory.hh"
#include <Psapi.h>

#include <map>
#include <string>
#include <array>

std::map<std::string, DWORD> _mod_handle_map;

uintptr_t sw::memory::CameraThinkPtr = 0;
std::add_pointer_t<void __fastcall(const char*)> sw::memory::LoadSky = nullptr;
uintptr_t sw::memory::KeyValuesFromString;
sw::iface::KeyValues* (__thiscall* sw::memory::KeyValuesFindKey)(sw::iface::KeyValues* keyValues, const char* keyName, bool create);
void(__thiscall* sw::memory::KeyValuesSetString)(sw::iface::KeyValues* keyValues, const char* value);
sw::iface::IViewRenderBeams* sw::memory::IViewRenderBeams = nullptr;
sw::iface::IMoveHelper* sw::memory::IMoveHelper = nullptr;
int* sw::memory::predictSeed = nullptr;
sw::iface::CMoveData* sw::memory::CMoveData = nullptr;
IDirect3DDevice9* sw::memory::D3DDevice;

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

[[nodiscard]] static auto generateBadCharTable(std::string_view pattern) noexcept
{
    std::array<std::size_t, (std::numeric_limits<std::uint8_t>::max)() + 1> table;

    auto lastWildcard = pattern.rfind('?');
    if (lastWildcard == std::string_view::npos)
        lastWildcard = 0;

    const auto defaultShift = (std::max)(std::size_t(1), pattern.length() - 1 - lastWildcard);
    table.fill(defaultShift);

    for (auto i = lastWildcard; i < pattern.length() - 1; ++i)
        table[static_cast<std::uint8_t>(pattern[i])] = pattern.length() - 1 - i;

    return table;
}

DWORD sw::memory::FindPattern(const char* module_name, std::string pattern)
{
    static auto id = 0;
    id++;

    HMODULE handle = GetModuleHandleA(module_name);
    MODULEINFO moduleInfo;
    BOOL res = GetModuleInformation(GetCurrentProcess(), handle, &moduleInfo, sizeof(MODULEINFO));

    if (!res) return 0;

    const auto lastIndex = pattern.length() - 1;
    const auto badCharTable = generateBadCharTable(pattern);

    const char* start = static_cast<const char*>(moduleInfo.lpBaseOfDll);
    const char* end = start + moduleInfo.SizeOfImage - pattern.length();

    while (start <= end)
    {
        int i = lastIndex;

        while (i >= 0 && (pattern[i] == '?' || start[i] == pattern[i])) i--;

        if (i < 0) return reinterpret_cast<DWORD>(start);

        start += badCharTable[static_cast<std::uint8_t>(start[lastIndex])];
    }

    return 0;
}

void sw::memory::FindRandomPtrs()
{
    CameraThinkPtr = FindPattern("client", "\x85\xC0\x75\x30\x38\x86");
    LoadSky = ToAbsolute<decltype(LoadSky)>(FindPattern("engine", "\xE8????\x84\xC0\x74\x2D\xA1") + 1);

    KeyValuesFromString = ToAbsolute<decltype(KeyValuesFromString)>(FindPattern("client", "\xE8????\x83\xC4\x04\x89\x45\xD8") + 1);
    KeyValuesFindKey = ToAbsolute<decltype(KeyValuesFindKey)>(FindPattern("client", "\xE8????\xF7\x45") + 1);
    KeyValuesSetString = ToAbsolute<decltype(KeyValuesSetString)>(FindPattern("client", "\xE8????\x89\x77\x38") + 1);

    IViewRenderBeams = *reinterpret_cast<iface::IViewRenderBeams**>(FindPattern("client", "\xB9????\x0F\x11\x44\x24?\xC7\x44\x24?????\xF3\x0F\x10\x84\x24") + 1);
    IMoveHelper = **reinterpret_cast<iface::IMoveHelper***>(FindPattern("client", "\x8B\x0D????\x8B\x45?\x51\x8B\xD4\x89\x02\x8B\x01") + 2);

    predictSeed = *reinterpret_cast<int**>(FindPattern("client", "\x8B\x0D????\xBA????\xE8????\x83\xC4\x04") + 2);
    CMoveData = **reinterpret_cast<iface::CMoveData***>(FindPattern("client", "\xA1????\xF3\x0F\x59\xCD") + 1);

    D3DDevice = **reinterpret_cast<IDirect3DDevice9***>(FindPattern("shaderapidx9", "\xa1????\x50\x8b\x08\xff\x51\x0c") + 1);

    console::WriteFormat("d3d: %x\n", D3DDevice);
}