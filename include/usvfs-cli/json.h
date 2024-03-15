#pragma once

#include <nlohmann/json.hpp>

#include "usvfs-cli/structs.h"

void from_json(const nlohmann::json& j, LaunchProcess& d) {
    j.at("path").get_to(d.executable);
    if (j.contains("args")) j.at("args").get_to(d.arguments);
    if (j.contains("cwd")) j.at("cwd").get_to(d.workingDirectory);
}

void from_json(const nlohmann::json& j, VirtualLink& d) {
    j.at("source").get_to(d.source);
    j.at("target").get_to(d.target);
}