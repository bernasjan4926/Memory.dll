#include "pch.h"
#include "library.h"
#include <iostream>
#include <tlhelp32.h>
#include <string>

using namespace std; 

/**
 * Open a target process with a given process ID
 */
HANDLE OpenProcessById(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        cerr << "Failed to open process. Error: " << GetLastError() << endl;
    }
    return hProcess;
}

/**
 * Read memory from the target process
 */
BOOL ReadMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize) {
    SIZE_T bytesRead;
    BOOL success = ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, &bytesRead);
    if (!success) {
        cerr << "Failed to read memory. Error: " << GetLastError() << endl;
    }
    return success;
}

/**
 * Write memory to the target process
 */
BOOL WriteMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize) {
    SIZE_T bytesWritten;
    BOOL success = WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, &bytesWritten);
    if (!success) {
        cerr << "Failed to write memory. Error: " << GetLastError() << endl;
    }
    return success;
}

/**
 * Close the handle to the target process
 */
void CloseProcess(HANDLE hProcess) {
    if (hProcess != NULL) {
        CloseHandle(hProcess);
    }
}

/**
 * Inject a DLL into a target process
 */
BOOL InjectDLL(DWORD processId, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        return FALSE;
    }

    // Allocate memory in the target process for the DLL path
    LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pDllPath == NULL) {
        CloseProcess(hProcess);
        return FALSE;
    }

    // Write the DLL path into the allocated memory
    if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, strlen(dllPath) + 1, NULL)) {
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseProcess(hProcess);
        return FALSE;
    }

    // Get the address of LoadLibraryA
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC pLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");

    // Create a remote thread to load the DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, pDllPath, 0, NULL);
    if (hThread == NULL) {
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseProcess(hProcess);
        return FALSE;
    }

    // Wait for the thread to finish and clean up
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseProcess(hProcess);
    return TRUE;
}

/**
 * Simple Mono injection function (This is a placeholder and you need to implement it)
 */
BOOL InjectMono(DWORD processId, const char* monoDllPath) {
    // For Mono injection, you would typically have to find the Mono API functions
    // and perform similar memory allocation and remote thread creation.
    // This is left as a placeholder for specific implementation as needed.
    cerr << "put whatever you want here" << endl;
    return FALSE;
}
