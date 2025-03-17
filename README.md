# Memory.dll
**This is a memory editing/dll injection library**

## Memory.dll Library
The Memory.dll library provides functions for manipulating memory in other processes, such as reading and writing memory, as well as DLL injection and Mono injection. This library is intended for developers looking to create cheat trainers or similar applications.

# Features
\- Open a target process by its ID
\- Read memory from a target process
\- Write memory to a target process
\- Inject a specified DLL into a target process
\- Placeholder for Mono injection (requires further implementation)

# Getting Started
## Prerequisites
\- Visual Studio (or another C++ compiler)
\- Basic knowledge of C++ and Windows API
\- Remember to edit the files before compiling as you have to define which memory adress to load an so on for the other files
## Installation

### Clone the Repository or download the zip file
### also dont forget to compile it to x86 or x64 depending on which architecture the game/process is 
Bash:
```bash
git clone https://github.com/yourusername/Memory.dll.git
cd Memory.dll
```

### Build the Library

\- Open the solution in Visual Studio
\- **Ensure the project configuration is set to DLL**
\- Build the project to create memory.dll

### Include in Your Project

\- Copy memory.dll to your project directory
\- Include the library.h header file in your source files

## Usage
### Including the Library
To use the library in your project, include the header file at the top of your source files:
```cpp
#include "library.h"
```

### Example Usage
Here is an example on how to use the library for reading, writing memory, and injecting a DLL into a target process.

```cpp
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

        // Example writing memory
        int newValue = 9999; // New value to write
        if (WriteMemory(hProcess, (LPVOID)0x00FF1234, &newValue, sizeof(newValue))) {
            std::cout << "Value written: " << newValue << std::endl;
        }

        // Inject DLL
        if (InjectDLL(targetProcessId, dllPath)) {
            std::cout << "DLL injected successfully!" << std::endl;
        } else {
            std::cout << "DLL injection failed!" << std::endl;
        }

        // Close the handle
        CloseProcess(hProcess);
    }

    return 0;
}
```

## Function Reference
| Function Name| Description|
| OpenProcessById| Opens a target process by its ID|
| ReadMemory| Reads memory from a target process|
| WriteMemory| Writes memory to a target process|
| CloseProcess| Closes the handle to the target process|
| InjectDLL | Injects a specified DLL into a target process|
| InjectMono| Placeholder for Mono injection (not fully implemented)|

# Notes
\- Use the correct process ID and memory addresses relevant to your target application.
\- This is a very basic one so expand to your needs
