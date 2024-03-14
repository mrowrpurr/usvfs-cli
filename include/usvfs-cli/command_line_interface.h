#include <argumentum/argparse.h>

#include <nlohmann/json.hpp>
#include <optional>
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
    vector<LaunchProcess>  processes{};
    vector<VirtualLink>    virtualLinks{};
};

inline optional<CommandOptions> ParseOptions(int argc, char* argv[]) {
    vector<string> processesJson;

    argumentum::argument_parser parser;
    auto                        params = parser.params();

    parser.config().program("usvfs-cli").description("usvfs command line interface");

    params.add_parameter(processesJson, "--process", "-p").minargs(1).help("Processes to launch");

    if (!parser.parse_args(argc, argv, 1)) return nullopt;

    CommandOptions options;

    for (auto& processJson : processesJson) {
        options.processes.push_back(nlohmann::json::parse(processJson));
    }

    return options;
}