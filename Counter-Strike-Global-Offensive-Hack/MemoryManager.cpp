#include "MemoryManager.h"

// returns true on success, false on failure
bool MemoryManager::Attach(const std::string& strProcessName) {
    // creates a snapshot of all currently running processes
    // TH32CS_SNAPPROCESS -> capture all processes (you use Process32Next to get the next one)
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    
    // check if the snapshot is valid
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &procEntry)) {
        do {
            if (std::wstring(procEntry.szExeFile) == std::wstring(strProcessName.begin(), strProcessName.end())) {
                // If we have reached this point then we have found the process in the snapshot 
                // which has the same name as the application we are trying to attach to!

                CloseHandle(hSnapshot); // close the snapshot as we don't need it now that we've found our process

                this->m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry.th32ProcessID);
                this->m_dwProcessId = procEntry.th32ProcessID;

                return true;
            }
        } while (Process32Next(hSnapshot, &procEntry));
    } else {
        CloseHandle(hSnapshot);
        return false;
    }
}

bool MemoryManager::GrabModule(const std::string& strModuleName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);

    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    MODULEENTRY32 modEntry;

    modEntry.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &modEntry)) {
        do {
            if (std::wstring(modEntry.szModule) == std::wstring(strModuleName.begin(), strModuleName.end())) {
                CloseHandle(hSnapshot);

                m_modules.push_back(modEntry);

                return true;
            }
        } while (Module32Next(hSnapshot, &modEntry));
    } else {
        CloseHandle(hSnapshot);
        return false;
    }
}

MemoryManager::MemoryManager(const std::string& strProcessName = "csgo.exe") : m_hProcess(INVALID_HANDLE_VALUE), m_dwProcessId(0) {
    if (!Attach(strProcessName)) {
        throw;
    }
}

template<typename T>
inline bool MemoryManager::Read(DWORD dwAddress, T& value) {
    return ReadProcessMemory(m_hProcess, reinterpret_cast<LPVOID>(dwAddress), value, sizeof(T), NULL) == TRUE;
}

template<typename T>
inline bool MemoryManager::Write(DWORD dwAddress, const T& value) {
    return WriteProcessMemory(m_hProcess, reinterpret_cast<LPVOID>(dwAddress), value, sizeof(T), NULL) == TRUE;
}

HANDLE MemoryManager::GetHandle() {
    return HANDLE();
}

DWORD MemoryManager::GetProcID(){
    return 0;
}

std::vector<MODULEENTRY32> MemoryManager::GetModules() {
    return std::vector<MODULEENTRY32>();
}
