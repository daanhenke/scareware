#include "winutil.hh"

DWORD sw::injector::GetProcessId(std::string process_name)
{
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    DWORD targetPid = 0;

    // Create a snapshot of all current processes
    HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // This can fail
    if (snapshotHandle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    // Get the first process from the snapshot
    Process32First(snapshotHandle, &processEntry);
    
    // We use a do while loop because we already have the first entry by now
    do
    {
        // Compare returns 0 on success
        if (process_name.compare(processEntry.szExeFile) == 0)
        {
            targetPid = processEntry.th32ProcessID;
            break;
        }
    }
    // Repeat until there are no more entries
    while (Process32Next(snapshotHandle, &processEntry));

    // Either broke out of the loop or process not found, cleanup and return
    CloseHandle(snapshotHandle);
    return targetPid;
}