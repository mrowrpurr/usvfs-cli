#pragma once

#include <argumentum/argparse.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include "usvfs-cli/json.h"  // IWYU pragma: keep
#include "usvfs-cli/structs.h"

inline std::optional<CommandOptions> ParseOptions(int argc, char* argv[]) {
    std::vector<std::string> processesJson;
    std::vector<std::string> virtualFileLinksJson;
    std::vector<std::string> virtualDirectoryLinksJson;
    std::vector<std::string> virtualOnCreateLinksJson;

    argumentum::argument_parser parser;
    auto                        params = parser.params();

    parser.config().program("usvfs-cli").description("usvfs command line interface");

    params.add_parameter(processesJson, "--process", "-c").minargs(1).help("Processes to launch");
    params.add_parameter(virtualFileLinksJson, "--link-file", "-f").minargs(1).help("Virtual file links");
    params.add_parameter(virtualDirectoryLinksJson, "--link-directory", "-d")
        .minargs(1)
        .help("Virtual directory links");
    params.add_parameter(virtualOnCreateLinksJson, "--link-on-create", "-o")
        .minargs(1)
        .help("Virtual on create links (overwrite folders)");

    try {
        if (!parser.parse_args(argc, argv, 1)) return std::nullopt;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    }

    CommandOptions options;
    try {
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
    return options;
}