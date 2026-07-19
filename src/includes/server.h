#pragma once

std::string get_mime_type(const std::string& path);
void handle_client(int client_sock, std::string dir);
void create_server(const std::string& dir, int port);