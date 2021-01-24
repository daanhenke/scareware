#include "interfaces.hh"
#include "memory.hh"
#include "console.hh"
#include <map>
#include <string>

sw::iface::IBaseClientDLL*      sw::interfaces::IBaseClientDLL = nullptr;
sw::iface::ICvar*               sw::interfaces::ICvar = nullptr;
sw::iface::IPanel*              sw::interfaces::IPanel = nullptr;
sw::iface::ISurface*            sw::interfaces::ISurface = nullptr;
sw::iface::IInputSystem*        sw::interfaces::IInputSystem = nullptr;
sw::iface::ClientModeShared*    sw::interfaces::ClientModeShared = nullptr;
sw::iface::IVEngineClient*      sw::interfaces::IVEngineClient = nullptr;
sw::iface::IClientEntityList*   sw::interfaces::IClientEntityList = nullptr;
sw::iface::IClientEntity**      sw::interfaces::LocalPlayer = nullptr;
sw::iface::CInput*              sw::interfaces::CInput = nullptr;
sw::iface::CGlowObjectManager*  sw::interfaces::CGlowObjectManager = nullptr;
sw::iface::IMaterialSystem*     sw::interfaces::IMaterialSystem = nullptr;
sw::iface::IGameEventManager2*  sw::interfaces::IGameEventManager2 = nullptr;
sw::iface::CGlobalVars*         sw::interfaces::CGlobalVars = nullptr;

// Private map used for printing all the interface pointers
std::map<std::string, uintptr_t> _iface_ptr_map;

// Loads an interface the normal way
template<typename T>
inline T* GetInterface(const char* partial_interface_name, const char* module_name)
{
    T* result = nullptr;

    // Get the module & it's CreateInterface function
    DWORD moduleHandle = sw::memory::WaitForModule(module_name);
    sw::iface::CreateInterfaceFn func = (sw::iface::CreateInterfaceFn) GetProcAddress((HMODULE) moduleHandle, "CreateInterface");

    // Make sure we actually got our function pointer
    if (func == nullptr)
    {
        sw::console::WriteFormat("Couldn't find CreateInterfaceFn for module '%s'!\n", module_name);
        return nullptr;
    }

    // Get pointer to the full interface name (most interfaces have version numbers in their name making them prone to breaking on update. This is why we only use partial names)
    DWORD interfaceNameAddress = sw::memory::FindTextInModule(module_name, partial_interface_name);

    // Make sure we actually have our interface name
    if (interfaceNameAddress == 0)
    {
        sw::console::WriteFormat("Couldn't find full name for interface '%s'!\n", partial_interface_name);
        return nullptr;
    }

    // Call CreateInterface and retrieve the pointer
    T* retval = (T*) func((const char*) interfaceNameAddress, nullptr);

    // Store the pointer in our map & return it
    _iface_ptr_map[partial_interface_name] = (uintptr_t) retval;
    return retval;
}

// Prints all the interfaces to the console
void PrintInterfaces()
{
    sw::console::SetColors(FOREGROUND_RED | FOREGROUND_BLUE);
    sw::console::WriteFormat("Interfaces:\n");

    for (auto it = _iface_ptr_map.begin(); it != _iface_ptr_map.end(); it++)
    {
        sw::console::WriteColorFormat(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN, "\tFound %s: ", it->first.c_str());
        sw::console::WriteColorFormat(FOREGROUND_GREEN | FOREGROUND_RED, "%x\n", it->second);
    }
}

bool sw::interfaces::FindInterfaces()
{
    // Load all interfaces from their factory functions
    IBaseClientDLL = GetInterface<iface::IBaseClientDLL>("VClient0", "client");
    ICvar = GetInterface<iface::ICvar>("VEngineCvar", "vstdlib");
    IPanel = GetInterface<iface::IPanel>("VGUI_Panel", "vgui2");
    ISurface = GetInterface<iface::ISurface>("VGUI_Surface", "vguimatsurface");
    IInputSystem = GetInterface<iface::IInputSystem>("InputSystemVersion", "inputsystem");
    IVEngineClient = GetInterface<iface::IVEngineClient>("VEngineClient", "engine");
    IClientEntityList = GetInterface<iface::IClientEntityList>("VClientEntityList", "client");
    IMaterialSystem = GetInterface<iface::IMaterialSystem>("VMaterialSystem080", "materialsystem");
    IGameEventManager2 = GetInterface<iface::IGameEventManager2>("GAMEEVENTSMANAGER002", "engine");
    
    // These interfaces don't have one, get them using magic instead
    ClientModeShared = **reinterpret_cast<iface::ClientModeShared ***>( ( *reinterpret_cast<uintptr_t **>( IBaseClientDLL ) )[ 10 ] + 0x5 );
    _iface_ptr_map["ClientModeShared"] = (uintptr_t) ClientModeShared;

    LocalPlayer = *reinterpret_cast<iface::IClientEntity***>(memory::FindPattern("client", "\xA1????\x89\x45\xBC\x85\xC0") + 1);
    _iface_ptr_map["LocalPlayer"] = (uintptr_t)LocalPlayer;

    CInput = *reinterpret_cast<iface::CInput**>((*reinterpret_cast<uintptr_t**>(IBaseClientDLL))[16] + 1);
    _iface_ptr_map["CInput"] = (uintptr_t)CInput;

    CGlowObjectManager = *reinterpret_cast<iface::CGlowObjectManager**>(memory::FindPattern("client", "\x0F\x11\x05????\x83\xC8\x01") + 3);
    _iface_ptr_map["CGlowObjectManager"] = (uintptr_t)CGlowObjectManager;

    CGlobalVars = **reinterpret_cast<iface::CGlobalVars***>((*reinterpret_cast<uintptr_t**>(IBaseClientDLL))[11] + 10);
    _iface_ptr_map["CGlobalVars"] = (uintptr_t)CGlobalVars;
    
    PrintInterfaces();

    return true;
}

sw::iface::IClientEntity* sw::interfaces::GetLocalPlayer()
{
    //console::WriteColorFormat(FOREGROUND_RED, "Local Player idx: %x\n", IVEngineClient->GetLocalPlayer());
    return IClientEntityList->GetClientEntity(IVEngineClient->GetLocalPlayer());
}