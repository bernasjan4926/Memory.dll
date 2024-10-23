#ifndef LIBRARY_H
#define LIBRARY_H

#include "pch.h"
#include <windows.h>
#include <iostream>
// Define a macro for export/import based on whether you're building or using the library
#ifdef LIBRARY_EXPORTS
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif

// Function prototypes
extern "C" {
    LIBRARY_API HANDLE OpenProcessById(DWORD processId);
    LIBRARY_API BOOL ReadMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize);
    LIBRARY_API BOOL WriteMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize);
    LIBRARY_API void CloseProcess(HANDLE hProcess);

    // DLL injection functions
    LIBRARY_API BOOL InjectDLL(DWORD processId, const char* dllPath);

    // Mono injection functions
    LIBRARY_API BOOL InjectMono(DWORD processId, const char* monoDllPath);
}

#endif // LIBRARY_H
