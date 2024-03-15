#pragma once

#include <windows.h>

#include <cstdint>  // IWYU pragma: keep
#include <iostream>

// ^-- include cstdint and windows.h first (for usvfs.h)

#include <usvfs.h>

#include "usvfs-cli/structs.h"

class UsvfsController {
    std::vector<VirtualLink>   _links;
    std::vector<LaunchProcess> _processes;
    std::vector<HANDLE>        _processHandles;

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

    void setupLinks() {
        for (auto& link : _links) {
            auto sourcePath = GetWideString(link.source);
            auto targetPath = GetWideString(link.target);
            switch (link.type) {
                case VirtualLinkType::File:
                    VirtualLinkFile(sourcePath.c_str(), targetPath.c_str(), {});
                    break;
                case VirtualLinkType::Directory:
                    VirtualLinkDirectoryStatic(sourcePath.c_str(), targetPath.c_str(), LINKFLAG_RECURSIVE);
                    break;
                case VirtualLinkType::OnCreate:
                    // targetPath: the "overwrite folder"
                    VirtualLinkDirectoryStatic(targetPath.c_str(), sourcePath.c_str(), LINKFLAG_CREATETARGET);
                    break;
            }
        }
    }

    bool runProcess(const LaunchProcess& process) {
        auto executablePath = GetWideString(process.executable);
        std::cout << "Executable path: " << process.executable << std::endl;
        auto workingDirectory = GetWideString(process.workingDirectory);
        std::cout << "Working directory: " << process.workingDirectory << std::endl;
        // auto args             = GetWideString(process.arguments);

        STARTUPINFOW si        = {};
        si.cb                  = sizeof(si);
        PROCESS_INFORMATION pi = {};

        auto lpApplicationName  = std::wstring{executablePath};
        auto lpWorkingDirectory = workingDirectory;

        std::cout << "Creating process" << std::endl;
        auto success = CreateProcessHooked(
            lpApplicationName.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, lpWorkingDirectory.c_str(), &si, &pi
        );
        std::cout << "Process created" << std::endl;

        if (success) {
            std::cout << "Process created successfully" << std::endl;
            _processHandles.push_back(pi.hProcess);
            std::cout << "Process handle added" << std::endl;
            CloseHandle(pi.hThread);
            std::cout << "Thread handle closed" << std::endl;
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
    UsvfsController(std::vector<VirtualLink> links, std::vector<LaunchProcess> processes)
        : _links(links), _processes(processes) {}

    void run_blocking() {
        std::cout << "Initializing usvfs" << std::endl;
        initializeUsvfs();
        std::cout << "Setting up links" << std::endl;
        setupLinks();
        std::cout << "Running processes" << std::endl;
        runProcesses();
        std::cout << "Waiting for processes" << std::endl;
        waitForProcesses();
        DisconnectVFS();
    }
};
