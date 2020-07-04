#pragma once
#include "windows.h"
#include "tlhelp32.h"
#include <vector>
#include <string>

class MemoryManager {
private:
    HANDLE m_hProcess; // The handle of the process we are attaching too
    DWORD m_dwProcessId; // The process id of the process we are attaching too
    std::vector<MODULEENTRY32> m_modules; // container for all the modules we want to grab from the process
public:
    bool Attach(const std::string&);
    bool GrabModule(const std::string&);

    MemoryManager(const std::string&);

    template <typename T>
    inline bool Read(DWORD dwAddress, T& value);

    template <typename T>
    inline bool Write(DWORD dwAddress, const T& Value);

    // Getters
    HANDLE GetHandle();
    DWORD GetProcID();
    std::vector<MODULEENTRY32> GetModules();
};
