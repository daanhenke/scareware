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

    // Create a pointer to the first section header
    PIMAGE_SECTION_HEADER sectionHeader = (PIMAGE_SECTION_HEADER) (ntHeaders + 1);

    // Copy all of the sections to our target process
    for (WORD i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
    {
        LPVOID target = (LPVOID) ((LPBYTE) remoteImage + sectionHeader[i].VirtualAddress);
        LPVOID source = (LPVOID) ((LPBYTE) dllFileBuffer + sectionHeader[i].PointerToRawData);
        DWORD size = sectionHeader[i].SizeOfRawData;
        WriteProcessMemory(processHandle, target, source, size, nullptr);
        std::cout << "\tCopying section:" << std::endl;
        std::cout << "\t\tFrom:\t" << std::hex << source << std::endl;
        std::cout << "\t\tTo:\t" << std::hex << target << std::endl;
        std::cout << "\t\tSize:\t" << std::hex << size << std::endl;
    }

    // Allocate memory for our stage1
    LPVOID remoteStage1 = VirtualAllocEx(processHandle, nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Deallocate stage 1 in target process
    VirtualFreeEx(processHandle, remoteStage1, 0, MEM_RELEASE);

    // Clean up handles
    CloseHandle(processHandle);
    CloseHandle(dllFileHandle);

    return true;
}