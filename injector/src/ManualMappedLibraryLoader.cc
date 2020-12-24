#include "ManualMappedLibraryLoader.hh"
#include <iostream>

sw::injector::ManualMappedLibraryLoader::ManualMappedLibraryLoader(std::filesystem::path dll_path) : ILibraryLoader(dll_path)
{

}

// TODO: CLEAN UP ALL OUR OBJECTS
// TODO: ERROR DETECTION
bool sw::injector::ManualMappedLibraryLoader::AttemptInjection(DWORD process_id)
{
    std::cout << "Manual Mapper:" << std::endl;

    // Open the target process
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    std::cout << "\tProcess Handle:\t" << std::hex << processHandle << std::endl;

    // Open the dll file
    HANDLE dllFileHandle = CreateFile(m_dll_path.string().c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    std::cout << "\tFile Handle:\t" << std::hex << dllFileHandle << std::endl;
    
    // Check for errors
    if (dllFileHandle == INVALID_HANDLE_VALUE)
    {
        CloseHandle(processHandle);
        return false;
    }

    // Obtain it's size
    DWORD dllFileSize = GetFileSize(dllFileHandle, 0);

    // Create a buffer which we can copy the file to & copy it
    LPVOID dllFileBuffer = VirtualAlloc(0, dllFileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    ReadFile(dllFileHandle, dllFileBuffer, dllFileSize, nullptr, nullptr);

    // Create pointers to the dll headers
    // The DOS header starts at byte 0 in a PE file, and the more interesting NT headers can be found using it
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER) dllFileBuffer;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS) ((LPBYTE) dllFileBuffer + dosHeader->e_lfanew);
    std::cout << "\tDOS Header:\t" << std::hex << dosHeader << std::endl;
    std::cout << "\tNT Headers:\t" << std::hex << ntHeaders << std::endl;

    // Allocate memory in our target process for the dll and make it writable
    // TODO: MAKE IT R + X after done writing to this
    LPVOID remoteImage = VirtualAllocEx(processHandle, 0, ntHeaders->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    std::cout << "\tRemote Image:\t" << std::hex << remoteImage << std::endl;

    // Copy the headers to our target process
    // TODO: MAYBE NOT DO THIS? TO AVOID THEM SHOWING IN RAM
    WriteProcessMemory(processHandle, remoteImage, dllFileBuffer, ntHeaders->OptionalHeader.SizeOfHeaders, nullptr);

    return true;
}