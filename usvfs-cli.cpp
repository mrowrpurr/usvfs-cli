#include <windows.h>

#include <cstdint>

//

#include <usvfs.h>

#include <boost/filesystem.hpp>
#include <codecvt>
#include <iostream>
#include <locale>

auto GetHappyPath(const std::string& path) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(path);
}

bool RunProgram(const std::string& program) {
    STARTUPINFOW si = {};
    si.cb           = sizeof(si);

    PROCESS_INFORMATION pi      = {};
    BOOL                success = FALSE;

    auto lpCommandLine      = std::wstring{GetHappyPath(program)};
    auto lpWorkingDirectory = L"C:/";

    std::cout << "Creating process..." << std::endl;

    auto result = CreateProcessHooked(
        nullptr, &lpCommandLine[0], nullptr, nullptr, FALSE, 0, nullptr, lpWorkingDirectory, &si,
        &pi
    );

    if (result) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    } else {
        std::cout << "Failed to create process" << std::endl;
        return false;
    }
}

int main() {
    auto params = usvfsCreateParameters();
    usvfsSetInstanceName(params, "my_usvfs");
    usvfsSetDebugMode(params, true);
    usvfsSetLogLevel(params, LogLevel::Debug);
    usvfsSetCrashDumpType(params, CrashDumpsType::Full);
    usvfsSetCrashDumpPath(params, "crash.log");
    usvfsSetProcessDelay(params, 1000);
    InitLogging(true);

    usvfsCreateVFS(params);

    auto sourceFolderPath = GetHappyPath("C:/Source");
    auto destFolderPath   = GetHappyPath("C:/Dest");

    auto linkedOK = VirtualLinkDirectoryStatic(
        sourceFolderPath.c_str(), destFolderPath.c_str(), LINKFLAG_CREATETARGET | LINKFLAG_RECURSIVE
    );

    if (!linkedOK) std::cout << "Failed to link directory" << std::endl;
    else std::cout << "Linked directory" << std::endl;

    std::cout << "Running program..." << std::endl;
    if (RunProgram("C:/Program Files/Notepad++/notepad++.exe")) {
        std::cout << "Program finished" << std::endl;
    } else {
        std::cout << "Failed to run program" << std::endl;
    }

    DisconnectVFS();
    // ClearVirtualMappings();

    return 0;
}
