#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "lexer.h"

class Parser {
    public:
        std::string fn;
        std::string html;
        std::vector<Token> tokens;

        int current;
        std::vector<std::string> offenders;
        std::unordered_map<std::string, std::string> vars;

        Token current_token;

        Parser(std::string fn);
        std::string tag_node(Token t);
        std::string handle_token();
        std::string mainloop();
        std::string get_content();
};