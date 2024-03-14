#include <argparse/argparse.hpp>
#include <string>

struct WebSocketServerOptions {
    bool           runServer     = false;
    unsigned short port          = 8080;
    std::string    serverAddress = "localhost";
};

struct CommandOptions {
    WebSocketServerOptions webSocketServerOptions;
};
