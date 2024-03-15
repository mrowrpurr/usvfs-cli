#pragma once

#include <argumentum/argparse.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include "usvfs-cli/json.h"  // IWYU pragma: keep
#include "usvfs-cli/structs.h"

inline std::optional<CommandOptions> ParseOptions(int argc, char* argv[]) {
    bool                     runServer;
    unsigned int             port;
    std::string              serverAddress;
    std::vector<std::string> processesJson;
    std::vector<std::string> virtualFileLinksJson;
    std::vector<std::string> virtualDirectoryLinksJson;
    std::vector<std::string> virtualOnCreateLinksJson;

    argumentum::argument_parser parser;
    auto                        params = parser.params();

    parser.config().program("usvfs-cli").description("usvfs command line interface");

    params.add_parameter(runServer, "--run-server", "-w").help("Run the web socket server");
    params.add_parameter(port, "--port", "-p").default_value(8080).help("Port for the web socket server");
    params.add_parameter(serverAddress, "--server-address", "-h")
        .default_value("localhost")
        .help("Address for the web socket server");
    params.add_parameter(processesJson, "--process", "-c").minargs(1).help("Processes to launch");
    params.add_parameter(virtualFileLinksJson, "--link-file", "-f").minargs(1).help("Virtual file links");
    params.add_parameter(virtualDirectoryLinksJson, "--link-directory", "-d")
        .minargs(1)
        .help("Virtual directory links");
    params.add_parameter(virtualOnCreateLinksJson, "--link-on-create", "-c")
        .help("Virtual on create links (overwrite folders)");

    if (!parser.parse_args(argc, argv, 1)) return std::nullopt;

    CommandOptions options;

    options.webSocketServerOptions.runServer     = runServer;
    options.webSocketServerOptions.port          = port;
    options.webSocketServerOptions.serverAddress = serverAddress;
    if (runServer) return options;

    for (auto& processJson : processesJson) options.processes.push_back(nlohmann::json::parse(processJson));
    for (auto& linkJson : virtualFileLinksJson)
        options.virtualLinks.push_back({VirtualLinkType::File, nlohmann::json::parse(linkJson)});
    for (auto& linkJson : virtualDirectoryLinksJson)
        options.virtualLinks.push_back({VirtualLinkType::Directory, nlohmann::json::parse(linkJson)});
    for (auto& linkJson : virtualOnCreateLinksJson)
        options.virtualLinks.push_back({VirtualLinkType::OnCreate, nlohmann::json::parse(linkJson)});
    return options;
}