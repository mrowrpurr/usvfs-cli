#include <argparse/argparse.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;

struct WebSocketServerOptions {
    bool           runServer     = false;
    unsigned short port          = 8080;
    string         serverAddress = "localhost";
};

struct LaunchProcess {
    string         command;
    vector<string> arguments;
    string         workingDirectory;
};

void from_json(const nlohmann::json& j, LaunchProcess& d) {
    j.at("path").get_to(d.command);
    if (j.contains("args")) j.at("args").get_to(d.arguments);
    if (j.contains("cwd")) j.at("cwd").get_to(d.workingDirectory);
}

enum class VirtualLinkType {
    Directory,
    File,
    OnCreate,
};

struct VirtualLink {
    VirtualLinkType type;
    string          source;
    string          target;
};

void from_json(const nlohmann::json& j, VirtualLink& d) {
    j.at("source").get_to(d.source);
    j.at("target").get_to(d.target);
}

struct CommandOptions {
    WebSocketServerOptions webSocketServerOptions;
    vector<LaunchProcess>  processes;
    vector<VirtualLink>    virtualLinks;
};

inline CommandOptions ParseOptions(int argc, char* argv[]) {
    CommandOptions           options;
    argparse::ArgumentParser parser("usvfs-cli");

    // usvfs.exe \
    //    --process '{"path":"C:/path/to/program.exe"}' \
    //    --link-on-create '{"source":"C:/actual/path","overwrite":"C:/overwrite-folder"}' \
    //    --link-dir '{"source":"C:/actual/path","target":"C:/virtual/path"}' \
    //    --link-file '{"source":"C:/actual/file.txt","target":"C:/virtual/file.txt"}'

    parser.add_argument("--run-server")
        .help("Run the WebSocket server")
        .default_value(false)
        .implicit_value(true);

    parser.add_argument("--port")
        .help("Port for the WebSocket server")
        .default_value(8080)
        .action([](const std::string& value) { return std::stoi(value); });

    parser.add_argument("--server-address")
        .help("Address for the WebSocket server")
        .default_value("localhost");

    parser.add_argument("--process")
        .help("Process to launch")
        .nargs(1)
        .action([&options](const std::string& value) {
            options.processes.push_back(nlohmann::json::parse(value));
        });

    try {
        parser.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
    }

    return options;
}
