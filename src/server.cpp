#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"
#include "parser.h"

namespace fs = std::filesystem;

// We need to take these two as arguments
// const std::string PUBLIC_DIR = "./src/tests/hello-world";
// const int PORT = 8080;

std::string get_mime_type(const std::string& path) {
    if (path.ends_with(".html")) return "text/html";
    if (path.ends_with(".css")) return "text/css";
    if (path.ends_with(".js")) return "application/javascript";
    if (path.ends_with(".jpg") || path.ends_with(".jpeg")) return "image/jpeg";
    if (path.ends_with(".png")) return "image/png";
    if (path.ends_with(".fsh")) return "text/html";
    return "text/plain";
}

void handle_client(int client_sock, std::string dir) {
    char buffer[4096] = {0};
    read(client_sock, buffer, sizeof(buffer));

    std::istringstream request(buffer);
    std::string method, path;
    request >> method >> path;

    if (method != "GET") {
        std::string response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
        write(client_sock, response.c_str(), response.size());
        close(client_sock);
        return;
    }

    // fallback
    if (path == "/") path = "/main.fsh";

    fs::path file_path = dir + path;
    
    if (fs::exists(file_path) && !fs::is_directory(file_path)) {
        std::string mime_type = get_mime_type(file_path.string());
        std::string content = get_content(file_path);
        
        std::ostringstream response_header;
        response_header << "HTTP/1.1 200 OK\r\n"
                        << "Content-Type: " << mime_type << "\r\n"
                        << "Content-Length: " << content.size() << "\r\n"
                        << "Connection: close\r\n\r\n";

        write(client_sock, response_header.str().c_str(), response_header.str().size());
        write(client_sock, content.c_str(), content.size());
    } else {
        std::string response = "HTTP/1.1 404 Not Found\r\n\r\n<h1>404 Not Found</h1>";
        write(client_sock, response.c_str(), response.size());
    }

    close(client_sock);
}

void create_server(const std::string& dir, int port) {
    const std::string PUBLIC_DIR = dir;
    const int PORT = port;

    fs::create_directory(PUBLIC_DIR); // backup in case dir doesn't exist
    std::cout << "Serving files from: " << fs::absolute(PUBLIC_DIR) << "\n";

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_sock, (struct sockaddr*)&address, sizeof(address));
    listen(server_sock, 10);

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_address;
        socklen_t addr_len = sizeof(client_address);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_address, &addr_len);
        
        if (client_sock >= 0) {
            handle_client(client_sock, PUBLIC_DIR);
        }
    }

    close(server_sock);
}