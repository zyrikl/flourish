#pragma once

#include <string>
#include <vector>

enum tt {
    // Basic elements of HTML
    TAG, ID, CLASS, ATTRIBUTE,
    STRING,

    // Important symbols
    LEFT_BRACE,
    RIGHT_BRACE,

    // Variable constructors
    LET, EQUALS, EQUALS_EQUALS,
    VARIABLE,

    // Other necessary constructors
    ECHO, NEWLINE, EOF_TOKEN
};

struct Token {
    tt type;
    std::string literal;

    Token(tt type = EOF_TOKEN, std::string literal = "eof");
};

std::vector<Token> token_stream(std::string fn);