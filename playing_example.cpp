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
        lpApplicationName.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, lpWorkingDirectory.c_str(), &si, &pi
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