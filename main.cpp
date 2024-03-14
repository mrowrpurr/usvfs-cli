#include <iostream>

#include "usvfs-cli/command_line_interface.h"

using namespace std;

int main(int argc, char* argv[]) {
    auto options = ParseOptions(argc, argv);

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
