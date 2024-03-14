#include <argumentum/argparse.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

using namespace std;

struct WebSocketServerOptions {
    bool           runServer;
    unsigned short port;
    string         serverAddress;
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
    bool           runServer     = false;
    unsigned int   port          = 8080;
    string         serverAddress = "localhost";
    vector<string> processesJson;
    vector<string> virtualFileLinksJson;
    vector<string> virtualDirectoryLinksJson;
    vector<string> virtualOnCreateLinksJson;

    argumentum::argument_parser parser;
    auto                        params = parser.params();

    parser.config().program("usvfs-cli").description("usvfs command line interface");

    params.add_parameter(runServer, "--run-server", "-w").help("Run the web socket server");
    params.add_parameter(port, "--port", "-p").help("Port for the web socket server");
    params.add_parameter(serverAddress, "--server-address", "-h").help("Address for the web socket server");
    params.add_parameter(processesJson, "--process", "-c").minargs(1).help("Processes to launch");
    params.add_parameter(virtualFileLinksJson, "--link-file", "-f").minargs(1).help("Virtual file links");
    params.add_parameter(virtualDirectoryLinksJson, "--link-directory", "-d")
        .minargs(1)
        .help("Virtual directory links");
    params.add_parameter(virtualOnCreateLinksJson, "--link-on-create", "-c")
        .help("Virtual on create links (overwrite folders)");

    if (!parser.parse_args(argc, argv, 1)) return nullopt;

    CommandOptions options;

    options.webSocketServerOptions.runServer     = runServer;
    options.webSocketServerOptions.port          = port;
    options.webSocketServerOptions.serverAddress = serverAddress;

    for (auto& processJson : processesJson) options.processes.push_back(nlohmann::json::parse(processJson));
    for (auto& linkJson : virtualFileLinksJson)
        options.virtualLinks.push_back({VirtualLinkType::File, nlohmann::json::parse(linkJson)});
    for (auto& linkJson : virtualDirectoryLinksJson)
        options.virtualLinks.push_back({VirtualLinkType::Directory, nlohmann::json::parse(linkJson)});
    for (auto& linkJson : virtualOnCreateLinksJson)
        options.virtualLinks.push_back({VirtualLinkType::OnCreate, nlohmann::json::parse(linkJson)});

    return options;
}