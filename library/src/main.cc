#include <Windows.h>

INT DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpData)
{
    MessageBoxA(nullptr, "Yeet", "Yote", MB_OK);
    return 0;
}