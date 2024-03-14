#include <iostream>

#include "usvfs-cli/command_line_interface.h"

using namespace std;

int main(int argc, char* argv[]) {
    // vector<string> fakeArguments = {
    //     "program.exe", "--process",
    //     "{\"path\":\"C:\\\\Program Files\\\\Notepad++\\\\notepad++.exe\"}"
    // };
    // int    fakeArgc = fakeArguments.size();
    // char** fakeArgv = new char*[fakeArgc];
    // for (int i = 0; i < fakeArgc; i++) {
    //     fakeArgv[i] = const_cast<char*>(fakeArguments[i].c_str());
    // }
    // auto parsedOptions = ParseOptions(fakeArgc, fakeArgv);

    auto parsedOptions = ParseOptions(argc, argv);

    if (!parsedOptions) {
        cout << "Failed to parse options" << endl;
        return 1;
    }

    auto  options       = *parsedOptions;
    auto& serverOptions = options.webSocketServerOptions;
    // ..

    auto& processes = options.processes;
    cout << "Processes: " << processes.size() << endl;
    for (auto& process : processes) {
        cout << "Process: " << process.command << endl;
        for (auto& arg : process.arguments) {
            cout << "  Arg: " << arg << endl;
        }
        cout << "  CWD: " << process.workingDirectory << endl;
    }

    return 0;
}
