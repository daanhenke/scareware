#include <Windows.h>

INT DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpData)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            MessageBoxA(nullptr, "Yeet", "Yote", MB_OK);
            break;
    }
    return 0;
}
