#include "file.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>

void write_file(std::string fn, std::string message) {
    std::ofstream file(fn);

    if (!file.is_open()) {
        std::cerr << "Could not open file for writing.";
        std::exit(1);
    }

    file << message;
    file.close();
}

void append_file(std::string fn, std::string message) {
    std::ofstream file(fn, std::ios_base::app);

    if (!file.is_open()) {
        std::cerr << "Could not open file for writing.";
        std::exit(1);
    }

    file << message;
    file.close();
}

std::string open_file(std::string fn) {
    std::ifstream file(fn);

    if (file.is_open()) {
        std::string file_contents(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        file.close();
        return file_contents;
    } else {
        std::cerr << "Could not open file.";
        std::exit(1);
    }
    return "";
}

std::vector<std::string> open_file_by_line(std::string fn) {
    std::vector<std::string> lines;
    std::ifstream file(fn);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        return lines;
    } else {
        std::cerr << "Could not open file.";
        std::exit(1);
    }
    return {""};
}