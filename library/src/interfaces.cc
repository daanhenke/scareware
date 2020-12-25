#include "interfaces.hh"
#include "memory.hh"
#include "console.hh"

sw::iface::IBaseClientDLL*      sw::interfaces::IBaseClientDLL = nullptr;
sw::iface::ICvar*               sw::interfaces::ICvar = nullptr;
sw::iface::IPanel*              sw::interfaces::IPanel = nullptr;
sw::iface::ISurface*            sw::interfaces::ISurface = nullptr;
sw::iface::IInputSystem*        sw::interfaces::IInputSystem = nullptr;

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

    // Get pointer to the full interface name
    DWORD interfaceNameAddress = sw::memory::FindTextInModule(module_name, partial_interface_name);

    // Make sure we actually have our interface name
    if (interfaceNameAddress == 0)
    {
        sw::console::WriteFormat("Couldn't find full name for interface '%s'!\n", partial_interface_name);
        return nullptr;
    }

    // Call CreateInterface and return the result
    return (T*) func((const char*) interfaceNameAddress, nullptr);
}

bool sw::interfaces::FindInterfaces()
{
    IBaseClientDLL = GetInterface<iface::IBaseClientDLL>("VClient", "client.dll");
    ICvar = GetInterface<iface::ICvar>("VEngineCvar", "vstdlib.dll");
    IPanel = GetInterface<iface::IPanel>("VGUI_Panel", "vgui2.dll");
    ISurface = GetInterface<iface::ISurface>("VGUI_Surface", "vguimatsurface.dll");
    IInputSystem = GetInterface<iface::IInputSystem>("InputSystemVersion", "inputsystem.dll");
    
    ICvar->ConsoleDPrintf("fakka nifje: %d\n", 1337);

    return true;
}