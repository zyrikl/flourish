#include <iostream>
#include <string>
#include "server.h"
#include "file.h"

int main(int argc, char* argv[]) {
    int port;

    if (argc < 2) {
        std::cerr << "Usage: flourish [directory] [port (optional, default=3000)]" << std::endl;
        return 1;
    } else if (argc == 2) {
        port = 8080;
    } else {
        port = std::stoi(argv[2]);
    }

    if (std::string(argv[1]) == std::string("run")) {
        create_server("./", port);
    } else {
        create_server(argv[1], port);
    }
    return 0;
}
