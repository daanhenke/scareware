#include <iostream>
#include <filesystem>

#include "winutil.hh"
#include "ManualMappedLibraryLoader.hh"

void usage(std::string name)
{
    std::cout << "no hablo espagnol" << std::endl
    << std::endl
    << name << " process.exe path/to/library.dll" << std::endl;
}

int main(int argc, const char** argv)
{
    // Get current filename for usage prompt
    auto injector_path = std::filesystem::path(argv[0]);
    std::string injector_filename = injector_path.filename().string();

    // Check if we have enough arguments (2 + our program name)
    if (argc != 3)
    {
        usage(injector_filename);
        return 1;
    }

    // Extract our arguments
    std::string process_name = argv[1];
    std::string dll_path = argv[2];

    // Try to get process id from process name
    DWORD target_pid = sw::injector::GetProcessId(process_name);

    // Print out some information
    std::cout  << "Frontend:" << std::endl
                << "\tProcess Name:\t" << process_name << std::endl
                << "\tLibrary Path:\t" << dll_path << std::endl
                << "\tProcess Id:\t" << std::hex << target_pid << std::endl;

    sw::injector::ILibraryLoader* loader = new sw::injector::ManualMappedLibraryLoader(dll_path);
    loader->AttemptInjection(target_pid);

    return 0;
}