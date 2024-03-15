#include <iostream>
#include <memory>

#include "usvfs-cli/command_line_interface.h"
#include "usvfs-cli/usvfs_controller.h"

std::unique_ptr<UsvfsController> usvfsController = nullptr;

int main(int argc, char* argv[]) {
    std::cout << "Parsing options..." << std::endl;

    // std::vector<std::string> fakeArguments = {
    //     "program.exe", "--process", "{\"path\":\"C:\\\\Program Files\\\\Notepad++\\\\notepad++.exe\"}"
    // };

    // usvfs-cli --process '{"path":"C:/Program Files/Notepad++/notepad++.exe"}' --link-directory
    // '{"source":"C:/Code/mrowrpurr/usvfs-cli/fixtures/source_folder","target":"C:/Code/mrowrpurr/usvfs-cli/fixtures/destination_folder"}'

    // std::vector<std::string> fakeArguments = {
    //     "program.exe", "--process", "{\"path\":\"C:/Program Files/Notepad++/notepad++.exe\"}", "--link-directory",
    //     "{\"source\":\"C:/Code/mrowrpurr/usvfs-cli/fixtures/source_folder\",\"target\":\"C:/Code/mrowrpurr/usvfs-cli/"
    //     "fixtures/destination_folder\"}"
    // };

    // int    fakeArgc = fakeArguments.size();
    // char** fakeArgv = new char*[fakeArgc];
    // for (int i = 0; i < fakeArgc; i++) {
    //     fakeArgv[i] = const_cast<char*>(fakeArguments[i].c_str());
    // }
    // auto parsedOptions = ParseOptions(fakeArgc, fakeArgv);

    auto parsedOptions = ParseOptions(argc, argv);
    std::cout << "Parsed options" << std::endl;

    if (!parsedOptions) {
        std::cout << "Failed to parse options" << std::endl;
        return 1;
    }

    if (parsedOptions->webSocketServerOptions.runServer) {
        std::cout << "[TODO] Running server on port " << parsedOptions->webSocketServerOptions.port << " at "
                  << parsedOptions->webSocketServerOptions.serverAddress << std::endl;
    } else {
        std::cout << "Link files/folders and run processes..." << std::endl;
        usvfsController = std::make_unique<UsvfsController>(parsedOptions->virtualLinks, parsedOptions->processes);
        std::cout << "Starting usvfs controller..." << std::endl;
        usvfsController->run_blocking();
        std::cout << "usvfs controller finished" << std::endl;
    }

    std::cout << "Parsed options" << std::endl;

    return 0;
}
