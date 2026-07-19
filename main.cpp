#include <iostream>
#include <string>
#include "server.h"
#include "file.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: flourish [directory] [port (optional, default=3000)]";
        return 1;
    } else if (argc == 2) {
        create_server(argv[1], 3000);
        return 0;
    } else {
        create_server(argv[1], std::stoi(argv[2]));
        return 0;
    }
}
