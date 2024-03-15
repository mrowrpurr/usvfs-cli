#pragma once

#include <argumentum/argparse.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include "usvfs-cli/json.h"  // IWYU pragma: keep
#include "usvfs-cli/structs.h"

// TODO: add -h/--help

inline std::optional<CommandOptions> ParseOptions(int argc, char* argv[]) {
    bool                     runServer;
    unsigned int             port;
    std::string              serverAddress;
    std::vector<std::string> processesJson;
    std::vector<std::string> virtualFileLinksJson;
    std::vector<std::string> virtualDirectoryLinksJson;
    std::vector<std::string> virtualOnCreateLinksJson;

    // Print out the arguments that are being parsed:
    std::cout << "Arguments size: " << argc << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }

    argumentum::argument_parser parser;
    auto                        params = parser.params();

    parser.config().program("usvfs-cli").description("usvfs command line interface");

    std::cout << "Adding parameters" << std::endl;
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
    params.add_parameter(virtualOnCreateLinksJson, "--link-on-create", "-o")
        .minargs(1)
        .help("Virtual on create links (overwrite folders)");

    try {
        std::cout << "Parsing arguments" << std::endl;
        if (!parser.parse_args(argc, argv, 1)) return std::nullopt;
        std::cout << "Parsed arguments" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    }

    std::cout << "Creating options" << std::endl;

    CommandOptions options;

    options.webSocketServerOptions.runServer     = runServer;
    options.webSocketServerOptions.port          = port;
    options.webSocketServerOptions.serverAddress = serverAddress;
    if (runServer) return options;

    std::cout << "Parsing json" << std::endl;

    try {
        // for (auto& processJson : processesJson) options.processes.push_back(nlohmann::json::parse(processJson));
        // for (auto& linkJson : virtualFileLinksJson)
        //     options.virtualLinks.push_back({VirtualLinkType::File, nlohmann::json::parse(linkJson)});
        // for (auto& linkJson : virtualDirectoryLinksJson)
        //     options.virtualLinks.push_back({VirtualLinkType::Directory, nlohmann::json::parse(linkJson)});
        // for (auto& linkJson : virtualOnCreateLinksJson)
        //     options.virtualLinks.push_back({VirtualLinkType::OnCreate, nlohmann::json::parse(linkJson)});

        for (auto& processJson : processesJson) {
            auto json = nlohmann::json::parse(processJson);
            options.processes.push_back(json.get<LaunchProcess>());
        }
        for (auto& linkJson : virtualFileLinksJson) {
            auto json = nlohmann::json::parse(linkJson);
            auto link = json.get<VirtualLink>();
            link.type = VirtualLinkType::File;
            options.virtualLinks.push_back(link);
        }
        for (auto& linkJson : virtualDirectoryLinksJson) {
            auto json = nlohmann::json::parse(linkJson);
            auto link = json.get<VirtualLink>();
            link.type = VirtualLinkType::Directory;
            options.virtualLinks.push_back(link);
        }
        for (auto& linkJson : virtualOnCreateLinksJson) {
            auto json = nlohmann::json::parse(linkJson);
            auto link = json.get<VirtualLink>();
            link.type = VirtualLinkType::OnCreate;
            options.virtualLinks.push_back(link);
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    }

    // Print the processes size and infos:
    std::cout << "Processes size: " << options.processes.size() << std::endl;
    for (auto& process : options.processes) {
        std::cout << "Process: " << process.executable << std::endl;
    }

    // Print the linked directories size and infos:
    std::cout << "Linked directories size: " << options.virtualLinks.size() << std::endl;
    for (auto& link : options.virtualLinks) {
        std::cout << "Link source: " << link.source << std::endl;
        std::cout << "Link target: " << link.target << std::endl;
    }

    return options;
}