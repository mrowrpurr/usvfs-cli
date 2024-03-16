#pragma once

#include <windows.h>

#include <cstdint>  // IWYU pragma: keep
#include <iostream>

// ^-- include cstdint and windows.h first (for usvfs.h)

#include <usvfs.h>

#include "usvfs-cli/structs.h"

class UsvfsController {
    std::vector<HANDLE> _processHandles;

    std::wstring GetWideString(const std::string& text) {
        int wideSize = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
        if (wideSize == ERROR_NO_UNICODE_TRANSLATION) throw std::runtime_error("Invalid UTF-8 sequence.");
        if (wideSize == 0) throw std::runtime_error("Error in conversion.");

        std::wstring wideString(wideSize, L'\0');
        if (MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wideString[0], wideSize) == 0)
            throw std::runtime_error("Error in conversion.");

        return wideString;
    }

    void initializeUsvfs() {
        auto params = usvfsCreateParameters();
        usvfsSetInstanceName(params, "my_usvfs");
        usvfsSetDebugMode(params, false);  // No UI popup when this is false
        usvfsSetLogLevel(params, LogLevel::Debug);
        usvfsSetCrashDumpType(params, CrashDumpsType::Full);
        usvfsSetCrashDumpPath(params, "crash.log");
        usvfsSetProcessDelay(params, 1000);
        InitLogging(true);
        usvfsCreateVFS(params);
    }

    bool runProcess(const LaunchProcess& process) {
        auto executablePath   = GetWideString(process.executable);
        auto workingDirectory = GetWideString(process.workingDirectory);
        // auto args             = GetWideString(process.arguments);

        STARTUPINFOW si        = {};
        si.cb                  = sizeof(si);
        PROCESS_INFORMATION pi = {};

        auto lpApplicationName  = std::wstring{executablePath};
        auto lpWorkingDirectory = workingDirectory;

        auto success = CreateProcessHooked(
            lpApplicationName.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, lpWorkingDirectory.c_str(), &si, &pi
        );

        if (success) {
            _processHandles.push_back(pi.hProcess);
            CloseHandle(pi.hThread);
            return true;
        } else {
            std::cout << "Failed to create process" << std::endl;
            return false;
        }
    }

    void waitForProcesses() {
        WaitForMultipleObjects(_processHandles.size(), _processHandles.data(), TRUE, INFINITE);
        for (auto& handle : _processHandles) CloseHandle(handle);
        _processHandles.clear();
    }

    void runProcesses() {
        for (auto& process : _processes) runProcess(process);
    }

public:
    UsvfsController() { initializeUsvfs(); }
    ~UsvfsController() { DisconnectVFS(); }

    bool AddVirtualLinks(const std::vector<VirtualLink>& links) {
        auto success = true;
        for (auto& link : links) {
            auto sourcePath = GetWideString(link.source);
            auto targetPath = GetWideString(link.target);
            switch (link.type) {
                case VirtualLinkType::File:
                    if (!VirtualLinkFile(sourcePath.c_str(), targetPath.c_str(), {})) {
                        success = false;
                        std::cout << "Failed to create link for " << link.source << " -> " << link.target << std::endl;
                    }
                    break;
                case VirtualLinkType::Directory:
                    if (!VirtualLinkDirectoryStatic(sourcePath.c_str(), targetPath.c_str(), LINKFLAG_RECURSIVE)) {
                        success = false;
                        std::cout << "Failed to create link for " << link.source << " -> " << link.target << std::endl;
                    }
                    break;
                case VirtualLinkType::OnCreate:  // targetPath: the "overwrite folder"
                    if (!VirtualLinkDirectoryStatic(targetPath.c_str(), sourcePath.c_str(), LINKFLAG_CREATETARGET)) {
                        success = false;
                        std::cout << "Failed to create link for " << link.source << " -> " << link.target << std::endl;
                    }
                    break;
            }
        }
        return success;
    }

    bool LaunchProcess(const LaunchProcess& process) {
        auto executablePath   = GetWideString(process.executable);
        auto workingDirectory = GetWideString(process.workingDirectory);
        // auto args             = GetWideString(process.arguments);

        STARTUPINFOW si        = {};
        si.cb                  = sizeof(si);
        PROCESS_INFORMATION pi = {};

        auto lpApplicationName  = std::wstring{executablePath};
        auto lpWorkingDirectory = workingDirectory;

        auto success = CreateProcessHooked(
            lpApplicationName.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, lpWorkingDirectory.c_str(), &si, &pi
        );

        if (success) {
            _processHandles.push_back(pi.hProcess);
            CloseHandle(pi.hThread);
            return true;
        } else {
            std::cout << "Failed to create process: " << process.executable << std::endl;
            return false;
        }
    }

    void WaitForAllProcesses(unsigned int timeoutMs = 100) {
        DWORD waitTime = timeoutMs;

        while (!_processHandles.empty()) {
            DWORD waitResult = WaitForMultipleObjects(
                static_cast<DWORD>(_processHandles.size()), _processHandles.data(),
                FALSE,  // Wait for any
                waitTime
            );

            // A process finished, close its handle and remove it from the list
            if (waitResult >= WAIT_OBJECT_0 && waitResult < WAIT_OBJECT_0 + _processHandles.size()) {
                size_t finishedIndex = waitResult - WAIT_OBJECT_0;
                CloseHandle(_processHandles[finishedIndex]);
                _processHandles.erase(_processHandles.begin() + finishedIndex);
            }
        }
    }
};
