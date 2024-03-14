#include <argparse/argparse.hpp>
#include <string>
#include <vector>

struct WebSocketServerOptions {
    bool           runServer     = false;
    unsigned short port          = 8080;
    std::string    serverAddress = "localhost";
};

struct LaunchProcess {
    std::string              command;
    std::vector<std::string> arguments;
    std::string              workingDirectory;
};

// TODO: first mock out some of what the CLI will look like
struct CommandOptions {
    WebSocketServerOptions     webSocketServerOptions;
    std::vector<LaunchProcess> processes;
};

inline CommandOptions ParseOptions(int argc, char* argv[]) {
    CommandOptions           options;
    argparse::ArgumentParser parser("usvfs-cli");

    try {
        parser.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
    }

    return options;
}
