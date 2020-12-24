#include "ManualMappedLibraryLoader.hh"
#include <iostream>

// We need the address of these functions in our relocation function, so define them as types
typedef HMODULE(__stdcall* pLoadLibraryA)(LPCSTR);
typedef FARPROC(__stdcall* pGetProcAddress)(HMODULE, LPCSTR);
typedef INT(__stdcall* pEntrypoint)(HMODULE, DWORD, LPVOID);

// Structure that has all the necessary information for relocation
typedef struct {
    LPVOID base;
    PIMAGE_BASE_RELOCATION relocation_block;
    PIMAGE_IMPORT_DESCRIPTOR import_descriptor;
    PIMAGE_NT_HEADERS nt_headers;

    pLoadLibraryA fnLoadLibrary;
    pGetProcAddress fnGetProcAddress;
    pEntrypoint fnEntrypoint;
} relocation_data_t;

// Function which will get copied into target process to relocate the injected dll & fix the import descriptors
extern "C" {

DWORD WINAPI relocation(LPVOID relocation_ptr) {
    // Get the data object our injector provided
    relocation_data_t* data = (relocation_data_t*) relocation_ptr;

    // Calculate the actual delta between the place where we loaded and where the DLL wants to be loaded
    DWORD delta = (DWORD) ((LPBYTE) data->base - data->nt_headers->OptionalHeader.ImageBase);

    // Get the first relocation block
    PIMAGE_BASE_RELOCATION relocationBlock = data->relocation_block;

    // For every valid block (last block will have 0 address)
    while (relocationBlock->VirtualAddress)
    {
        DWORD count = relocationBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION) / sizeof(WORD);
        PWORD list = (PWORD) (relocationBlock + 1);

        for (DWORD i = 0; i < count; i++)
        {
            if (list[i] != 0)
            {
                // Correct every symbol in the relocation block by adding the delta we calculated beforehand
                PDWORD ptr = (PDWORD) ((LPBYTE) data->base + (relocationBlock->VirtualAddress + (list[i] & 0xFFF)));
                *ptr += delta;
            }
        }

        // Get next block
        relocationBlock = (PIMAGE_BASE_RELOCATION) ((LPBYTE) relocationBlock + relocationBlock->SizeOfBlock);
    }

    // Get the first import descriptor
    PIMAGE_IMPORT_DESCRIPTOR importDescriptor = data->import_descriptor;

    // For every valid import descriptor (last one will have 0 characteristics)
    while (importDescriptor->Characteristics != 0)
    {
        PIMAGE_THUNK_DATA originalThunk = (PIMAGE_THUNK_DATA) ((LPBYTE) data->base + importDescriptor->OriginalFirstThunk);
        PIMAGE_THUNK_DATA newThunk = (PIMAGE_THUNK_DATA) ((LPBYTE) data->base + importDescriptor->FirstThunk);

        HMODULE moduleHandle = data->fnLoadLibrary((LPCSTR) data->base + importDescriptor->Name);

        // Check if the module got loaded correctly
        if (moduleHandle == nullptr)
        {
            // Oh no, something has pooped, will probably crash now
            return 0;
        }

        while (originalThunk->u1.AddressOfData)
        {
            // Import this symbol by ordinal
            if (originalThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
            {
                DWORD ptr = (DWORD) data->fnGetProcAddress(moduleHandle, (LPCSTR) (originalThunk->u1.Ordinal & 0xFFFF));

                if (ptr == 0)
                {
                    return 0;
                }

                newThunk->u1.Function = ptr;
            }
            // Import this symbol by name
            else
            {
                PIMAGE_IMPORT_BY_NAME byName = (PIMAGE_IMPORT_BY_NAME) ((LPBYTE) data->base + originalThunk->u1.AddressOfData);
                DWORD ptr = (DWORD) data->fnGetProcAddress(moduleHandle, (LPCSTR) byName->Name);

                if (ptr == 0)
                {
                    return 0;
                }

                newThunk->u1.Function = ptr;
            }

            originalThunk++;
            newThunk++;
        }

        importDescriptor++;
    }

    // Check if the library contains an entrypoint
    if (data->fnEntrypoint != 0)
    {
        data->fnEntrypoint((HMODULE) data->base, DLL_PROCESS_ATTACH, nullptr);
    }

    return 1;
}

// This function does not actually do anything, but (relocation_end - relocation) can now be used to calculate the size of the relocation function
DWORD WINAPI relocation_end()
{
    return 0;
}
}

sw::injector::ManualMappedLibraryLoader::ManualMappedLibraryLoader(std::filesystem::path dll_path) : ILibraryLoader(dll_path)
{

}

// Main function which is responsible for copying the dll & relocation function and executing the relocation function
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
    LPVOID remoteImage = VirtualAllocEx(processHandle, 0, ntHeaders->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
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

    // Allocate memory for our relocation function
    LPVOID remoteRelocator = VirtualAllocEx(processHandle, nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    std::cout << "\tRelocation Data:\t" << std::hex << remoteRelocator << std::endl;

    // Create all the necessary data for our relocation function
    relocation_data_t relocation_data;
    ZeroMemory(&relocation_data, sizeof(relocation_data_t));
    relocation_data.base = remoteImage;
    relocation_data.nt_headers = (PIMAGE_NT_HEADERS) ((LPBYTE) remoteImage + dosHeader->e_lfanew);
    relocation_data.relocation_block = (PIMAGE_BASE_RELOCATION) ((LPBYTE) remoteImage + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
    relocation_data.import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR) ((LPBYTE) remoteImage + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    relocation_data.fnGetProcAddress = GetProcAddress;
    relocation_data.fnLoadLibrary = LoadLibraryA;

    // Check if the module contains an entrypoint
    if (ntHeaders->OptionalHeader.AddressOfEntryPoint)
    {
        relocation_data.fnEntrypoint = (pEntrypoint) ((LPBYTE) remoteImage + ntHeaders->OptionalHeader.AddressOfEntryPoint);
    }
    else
    {
        relocation_data.fnEntrypoint = 0;
    }

    std::cout << "\tRelocation Function:\t" << std::hex << ((relocation_data_t*) remoteRelocator + 1) << std::endl;
    std::cout << "\tRelocation Function Size:\t" << std::hex << ((DWORD) relocation_end - (DWORD) relocation) << std::endl;
    std::cout << "\tLibrary Entrypoint:\t" << std::hex << relocation_data.fnEntrypoint << std::endl;
    std::cout << "\tLoadLibraryA:\t" << std::hex << relocation_data.fnLoadLibrary << std::endl;
    std::cout << "\tGetProcAddress:\t" << std::hex << relocation_data.fnGetProcAddress << std::endl;

    // Copy data to target process
    WriteProcessMemory(processHandle, remoteRelocator, &relocation_data, sizeof(relocation_data_t), nullptr);

    // Copy relocation function to target process
    WriteProcessMemory(processHandle, (LPVOID) ((relocation_data_t*) remoteRelocator + 1), relocation, (DWORD) relocation_end - (DWORD) relocation, nullptr);

    // Run relocation function
    HANDLE threadHandle = CreateRemoteThread(processHandle, nullptr, 0, (LPTHREAD_START_ROUTINE) ((relocation_data_t*) remoteRelocator + 1), remoteRelocator, 0, nullptr);

    // Wait for the relocation thread to finish
    WaitForSingleObject(threadHandle, INFINITE);
    
    std::cout << "Done! Cleaning up..." << std::endl;

    // Deallocate relocation func in target process
    VirtualFreeEx(processHandle, remoteRelocator, 0, MEM_RELEASE);

    // Clean up handles
    CloseHandle(processHandle);
    CloseHandle(dllFileHandle);

    return true;
}