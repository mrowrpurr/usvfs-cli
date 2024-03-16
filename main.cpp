#include <csignal>
#include <iostream>
#include <memory>

#include "usvfs-cli/command_line_interface.h"
#include "usvfs-cli/usvfs_controller.h"

std::unique_ptr<UsvfsController> usvfsController = nullptr;

volatile std::sig_atomic_t _sigint = 0;

void signalHandler(int signal) {
    if (signal == SIGINT) _sigint = 1;
}

std::optional<LaunchProcess> ParseStandardInput(const std::string& input) {
    try {
        auto json = nlohmann::json::parse(input);
        return json.template get<LaunchProcess>();
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        return std::nullopt;
    }
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signalHandler);

    auto parsedOptions = ParseOptions(argc, argv);
    if (!parsedOptions) {
        std::cout << "Failed to parse options" << std::endl;
        return 1;
    }
    if (parsedOptions->virtualLinks.empty()) {
        std::cout << "No virtual links to create" << std::endl;
        return 1;
    }
    if (!parsedOptions->useStdinForProcesses && parsedOptions->processes.empty()) {
        std::cout << "No processes to launch" << std::endl;
        return 1;
    }

    usvfsController = std::make_unique<UsvfsController>();

    if (!usvfsController->AddVirtualLinks(parsedOptions->virtualLinks)) {
        std::cout << "Some virtual links failed" << std::endl;
    }

    if (!parsedOptions->processes.empty()) {
        for (const auto& process : parsedOptions->processes)
            if (!usvfsController->LaunchProcess(process)) std::cout << "Failed to launch process" << std::endl;
    }

    if (parsedOptions->useStdinForProcesses) {
        std::cout << "Reading processes from stdin" << std::endl;
        std::cout << "Press CTRL-C to exit" << std::endl;
        std::cout << "Enter JSON for new process to launch:" << std::endl;
        std::string line;
        while (std::getline(std::cin, line)) {
            if (!line.empty()) {
                // TODO: allow also providing link files/folders via STDIN (?)
                auto process = ParseStandardInput(line);
                if (process && process->IsValid()) {
                    if (!usvfsController->LaunchProcess(*process)) std::cout << "Failed to launch process" << std::endl;
                } else {
                    std::cout << "Invalid input" << std::endl;
                }
            }
            if (_sigint) {
                std::cout << "SIGINT received, stopping all processes..." << std::endl;
                usvfsController->KillAllProcesses();
                break;  // Exit the loop if SIGINT is received
            }
        }
    } else if (parsedOptions->processes.empty()) {
        std::cout << "No processes to launch" << std::endl;
        return 1;
    }

    usvfsController->WaitForAllProcesses();
    std::cout << "All processes have exited" << std::endl;

    return 0;
}

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