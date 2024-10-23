// injection.cpp
#include "pch.h"
#include "injection.h"
#include <iostream>

BOOL InjectDLL(DWORD processId, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process: " << GetLastError() << std::endl;
        return FALSE;
    }

    // Allocate memory for the DLL path in the target process
    LPVOID pRemotePath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (pRemotePath == NULL) {
        std::cerr << "Failed to allocate memory in target process: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return FALSE;
    }

    // Write the DLL path into the allocated memory
    if (!WriteProcessMemory(hProcess, pRemotePath, dllPath, strlen(dllPath) + 1, NULL)) {
        std::cerr << "Failed to write DLL path to target process: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    // Get the address of LoadLibraryA from kernel32.dll
    LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryA");
    if (pLoadLibrary == NULL) {
        std::cerr << "Failed to get address of LoadLibraryA: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    // Create a remote thread in the target process to load the DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemotePath, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Failed to create remote thread: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    // Wait for the thread to complete
    WaitForSingleObject(hThread, INFINITE);

    // Clean up
    VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return TRUE;
}
