#pragma once

#include <string>
#include <vector>

void write_file(std::string fn, std::string message);
void append_file(std::string fn, std::string message);

std::string open_file(std::string fn);
std::vector<std::string> open_file_by_line(std::string fn);