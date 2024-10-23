#include "pch.h"
#include <iostream>
#include "library.h" 
int main() {
    DWORD targetProcessId = 1234; // Replace with the actual process ID
    const char* dllPath = "C:\\path\\to\\your\\dll.dll"; // Replace with the path to the DLL you want to inject

    // Open process
    HANDLE hProcess = OpenProcessById(targetProcessId);
    if (hProcess != NULL) {
        int value = 0;

        // Example reading memory
        if (ReadMemory(hProcess, (LPCVOID)0x00FF1234, &value, sizeof(value))) {
            std::cout << "Value read: " << value << std::endl;
        }
        else {
            std::cerr << "Failed to read memory." << std::endl;
        }

        // Example writing memory
        int newValue = 9999; // New value to write
        if (WriteMemory(hProcess, (LPVOID)0x00FF1234, &newValue, sizeof(newValue))) {
            std::cout << "Value written: " << newValue << std::endl;
        }
        else {
            std::cerr << "Failed to write memory." << std::endl;
        }

        // Inject DLL
        if (InjectDLL(targetProcessId, dllPath)) {
            std::cout << "DLL injected successfully!" << std::endl;
        }
        else {
            std::cerr << "DLL injection failed!" << std::endl;
        }

        // Close the handle
        CloseProcess(hProcess);
    }
    else {
        std::cerr << "Failed to open process." << std::endl;
    }

    return 0;
}