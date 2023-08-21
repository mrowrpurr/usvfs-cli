#include <windows.h>

#include <cstdint>

//

#include <usvfs.h>

#include <boost/filesystem.hpp>
#include <codecvt>
#include <iostream>
#include <locale>

// TODO: get it working with different (non-deprecated) functions
auto GetHappyPath(const std::string& path) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(path);
}

bool RunProgram(const std::string& program, const std::string& workingDirectory) {
    STARTUPINFOW si = {};
    si.cb           = sizeof(si);

    PROCESS_INFORMATION pi      = {};
    BOOL                success = FALSE;

    auto lpCommandLine      = std::wstring{GetHappyPath(program)};
    auto lpWorkingDirectory = std::wstring{GetHappyPath(workingDirectory)};

    std::cout << "Creating process..." << std::endl;

    auto result = CreateProcessHooked(
        nullptr, &lpCommandLine[0], nullptr, nullptr, FALSE, 0, nullptr, lpWorkingDirectory.c_str(),
        &si, &pi
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

    auto workingDirectory = "C:/Code/mrowrpurr/usvfs-cli/fixtures";

    auto sourceAFolderPath =
        GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/source_folder/SourceA");
    auto sourceBFolderPath =
        GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/source_folder/SourceB");
    auto sourceCFolderPath =
        GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/source_folder/SourceC");

    auto destFolderPath = GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/destination_folder");

    VirtualLinkDirectoryStatic(
        sourceAFolderPath.c_str(), destFolderPath.c_str(), LINKFLAG_RECURSIVE
    );
    VirtualLinkDirectoryStatic(
        sourceBFolderPath.c_str(), destFolderPath.c_str(), LINKFLAG_RECURSIVE
    );
    VirtualLinkDirectoryStatic(
        sourceCFolderPath.c_str(), destFolderPath.c_str(), LINKFLAG_RECURSIVE
    );

    std::cout << "Running program..." << std::endl;
    if (RunProgram("C:/Program Files/Notepad++/notepad++.exe", workingDirectory)) {
        std::cout << "Program finished" << std::endl;
    } else {
        std::cout << "Failed to run program" << std::endl;
    }

    DisconnectVFS();
    // ClearVirtualMappings();

    return 0;
}
