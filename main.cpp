#include <iostream>
#include <memory>

#include "usvfs-cli/command_line_interface.h"
#include "usvfs-cli/usvfs_controller.h"

std::unique_ptr<UsvfsController> usvfsController = nullptr;

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
        std::cout << "Failed to parse options" << std::endl;
        return 1;
    }

    if (parsedOptions->webSocketServerOptions.runServer) {
        std::cout << "Running server on port " << parsedOptions->webSocketServerOptions.port << " at "
                  << parsedOptions->webSocketServerOptions.serverAddress << std::endl;
    } else {
        std::cout << "Not running server" << std::endl;
    }

    std::cout << "Parsed options" << std::endl;

    return 0;
}
