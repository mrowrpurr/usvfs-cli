#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include "usvfs-cli/structs.h"

// void from_json(const nlohmann::json& j, LaunchProcess& d) {
//     std::cout << "Parsing launch process" << std::endl;
//     std::cout << "JSON: " << j << std::endl;
//     j.at("path").get_to(d.executable);
//     // if (j.contains("args")) j.at("args").get_to(d.arguments);
//     // if (j.contains("cwd")) j.at("cwd").get_to(d.workingDirectory);
// }

void from_json(const nlohmann::json& j, LaunchProcess& d) {
    std::cout << "Parsing launch process" << std::endl;
    std::cout << "JSON: " << j << std::endl;
    // Make sure the JSON contains the "path" key before trying to assign it
    if (j.contains("path") && j["path"].is_string()) {
        j.at("path").get_to(d.executable);
    }
    if (j.contains("args") && j["args"].is_string()) {
        j.at("args").get_to(d.arguments);
    }
    if (j.contains("cwd") && j["cwd"].is_string()) {
        j.at("cwd").get_to(d.workingDirectory);
    }
}

void from_json(const nlohmann::json& j, VirtualLink& d) {
    std::cout << "Parsing virtual link" << std::endl;
    j.at("source").get_to(d.source);
    j.at("target").get_to(d.target);
}
