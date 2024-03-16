#pragma once

#include <string>
#include <vector>

struct LaunchProcess {
    std::string executable;
    std::string arguments;
    std::string workingDirectory;

    bool IsValid() const { return !executable.empty(); }
};

enum class VirtualLinkType {
    Directory,
    File,
    OnCreate,
};

struct VirtualLink {
    VirtualLinkType type;
    std::string     source;
    std::string     target;
};

struct CommandOptions {
    bool                       useStdinForProcesses = false;
    std::vector<LaunchProcess> processes{};
    std::vector<VirtualLink>   virtualLinks{};
};
