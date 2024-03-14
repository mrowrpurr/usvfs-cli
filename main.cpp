#include <iostream>

#include "usvfs-cli/command_line_interface.h"

using namespace std;

int main(int argc, char* argv[]) {
    // vector<string> fakeArguments = {
    //     "program.exe", "--process", "{\"path\":\"C:\\\\Program Files\\\\Notepad++\\\\notepad++.exe\"}"
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

    if (parsedOptions->webSocketServerOptions.runServer) {
        cout << "Running server on port " << parsedOptions->webSocketServerOptions.port << " at "
             << parsedOptions->webSocketServerOptions.serverAddress << endl;
    } else {
        cout << "Not running server" << endl;
    }

    cout << "Parsed options" << endl;

    return 0;
}
