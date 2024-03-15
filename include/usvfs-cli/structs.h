#pragma once

#include <string>
#include <vector>

struct WebSocketServerOptions {
    bool           runServer;
    unsigned short port;
    std::string    serverAddress;
};

struct LaunchProcess {
    std::string executable;  // TODO: rename to executable or executablePath or something better
    std::string arguments;
    std::string workingDirectory;
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
    WebSocketServerOptions     webSocketServerOptions;
    std::vector<LaunchProcess> processes{};
    std::vector<VirtualLink>   virtualLinks{};
};