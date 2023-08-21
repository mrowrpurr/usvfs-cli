#include <windows.h>

#include <cstdint>

//

#include <usvfs.h>

#include <boost/filesystem.hpp>
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

// Le sigh.
std::wstring GetHappyPath(const std::string& path) {
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, NULL, 0);
    if (wideSize == ERROR_NO_UNICODE_TRANSLATION)
        throw std::runtime_error("Invalid UTF-8 sequence.");
    if (wideSize == 0) throw std::runtime_error("Error in conversion.");

    std::wstring wideString(wideSize, L'\0');
    if (MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, &wideString[0], wideSize) == 0)
        throw std::runtime_error("Error in conversion.");

    return wideString;
}

bool RunProgram(const std::string& program, const std::wstring& workingDirectory) {
    STARTUPINFOW si = {};
    si.cb           = sizeof(si);

    PROCESS_INFORMATION pi      = {};
    BOOL                success = FALSE;

    auto lpApplicationName = std::wstring{GetHappyPath(program)};
    // auto lpCommandLine      = std::wstring{GetHappyPath(program)};
    auto lpWorkingDirectory = workingDirectory;

    std::cout << "Creating process..." << std::endl;

    auto result = CreateProcessHooked(
        lpApplicationName.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, lpWorkingDirectory.c_str(),
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
    usvfsSetDebugMode(params, false);  // No UI popup when this is false
    usvfsSetLogLevel(params, LogLevel::Debug);
    usvfsSetCrashDumpType(params, CrashDumpsType::Full);
    usvfsSetCrashDumpPath(params, "crash.log");
    usvfsSetProcessDelay(params, 1000);
    InitLogging(true);

    usvfsCreateVFS(params);

    auto workingDirectory = L"C:/Code/mrowrpurr";

    auto sourceAFolderPath =
        GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/source_folder/SourceA");
    auto sourceBFolderPath =
        GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/source_folder/SourceB");
    auto sourceCFolderPath =
        GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/source_folder/SourceC");

    auto overwriteFolderPath = GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/overwrite");

    auto destFolderPath = GetHappyPath("C:/Code/mrowrpurr/usvfs-cli/fixtures/destination_folder");

    VirtualLinkDirectoryStatic(
        sourceAFolderPath.c_str(), destFolderPath.c_str(), LINKFLAG_RECURSIVE
    );
    // VirtualLinkDirectoryStatic(
    //     sourceBFolderPath.c_str(), destFolderPath.c_str(), LINKFLAG_RECURSIVE
    // );
    VirtualLinkDirectoryStatic(
        sourceCFolderPath.c_str(), destFolderPath.c_str(), LINKFLAG_RECURSIVE
    );
    VirtualLinkDirectoryStatic(
        overwriteFolderPath.c_str(), destFolderPath.c_str(), LINKFLAG_CREATETARGET
    );

    std::cout << "Running program..." << std::endl;
    // if (RunProgram("C:/Program Files/Notepad++/notepad++.exe", workingDirectory)) {
    if (RunProgram("c:/Program Files/Kate/bin/kate.exe", workingDirectory)) {
        std::cout << "Program finished" << std::endl;
    } else {
        std::cout << "Failed to run program" << std::endl;
    }

    DisconnectVFS();
    // ClearVirtualMappings();

    return 0;
}
