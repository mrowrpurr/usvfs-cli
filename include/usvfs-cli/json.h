#pragma once

#include <nlohmann/json.hpp>

#include "usvfs-cli/structs.h"

void from_json(const nlohmann::json& j, LaunchProcess& d) {
    if (j.contains("path") && j["path"].is_string()) j.at("path").get_to(d.executable);
    if (j.contains("args") && j["args"].is_string()) j.at("args").get_to(d.arguments);
    if (j.contains("cwd") && j["cwd"].is_string()) j.at("cwd").get_to(d.workingDirectory);
}

void from_json(const nlohmann::json& j, VirtualLink& d) {
    if (j.contains("source") && j["source"].is_string()) j.at("source").get_to(d.source);
    if (j.contains("target") && j["target"].is_string()) j.at("target").get_to(d.target);
}
