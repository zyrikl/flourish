#include "lexer.h"
#include "file.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cctype>

Token::Token(tt type, std::string literal) : type(type), literal(literal) {}

std::vector<Token> token_stream(std::string fn) {
    std::string contents = open_file(fn);
    std::vector<Token> tokens;

    int start = 0;
    int current = 0;

    while (current < contents.length()) {
        start = current;
        char c = contents[current++];

        switch (c) {
            case '{':
                tokens.push_back(Token(LEFT_BRACE, std::string(1, c)));
                break;
            case '}':
                tokens.push_back(Token(RIGHT_BRACE, std::string(1, c)));
                break;
            case '/':
                if (contents[current] == '/') {
                    while (current >= contents.length() ? '\0' : contents[current] != '\n') current++;
                } /*else if (contents[current] == '*') {
                    while ()
                }*/ else {
                    std::cerr << "Unexpected character.";
                    std::exit(1);
                }
                break;
            case '\n':
                tokens.push_back(Token(NEWLINE, std::string(1, c)));
                break;
            case ' ':
            case '\t':
                break;
            case '\'':
            case '"': {
                while (
                    (current >= contents.length()) ? '\0' : contents[current] != c && current < contents.length()
                ) {
                    current++;
                }

                if (current >= contents.length()) {
                    std::cerr << "Unterminated string.";
                    std::exit(1);
                    //return;
                }

                current++;

                std::string value = contents.substr(start + 1, current - start - 2);
                tokens.push_back(Token(STRING, value));
                break;
            }
            case '.':
                while (
                    std::isalnum(
                        static_cast<unsigned char>(
                            current >= contents.length() ? '\0' : contents[current]
                        )
                    )
                        || current >= contents.length() ? '\0' : contents[current] == '-'
                        || current >= contents.length() ? '\0' : contents[current] == '_'
                ) {
                    current++;
                }
                tokens.push_back(Token(CLASS, contents.substr(start, current - start)));
                break;
            case '#':
                while (
                    std::isalnum(
                        static_cast<unsigned char>(
                            current >= contents.length() ? '\0' : contents[current]
                        )
                    )
                        || current >= contents.length() ? '\0' : contents[current] == '-'
                        || current >= contents.length() ? '\0' : contents[current] == '_'
                ) {
                    current++;
                }
                tokens.push_back(Token(ID, contents.substr(start, current - start)));
                break;
            default: 
                if (std::isalpha(static_cast<unsigned char>(c))) {
                    while (std::isalnum(
                        static_cast<unsigned char>(
                            current >= contents.length()) ? '\0' : contents[current]
                        )
                    ) current++;

                    if (contents[current] == '=') {
                        tokens.push_back(Token(ATTRIBUTE, contents.substr(start, current - start)));
                    } else if (contents.substr(start, current - start) == "echo") {
                        tokens.push_back(Token(ECHO, "echo"));
                    } else {
                        tokens.push_back(Token(TAG, contents.substr(start, current - start)));
                    }
                } else if (c != '=') {
                    std::cerr << "Unexpected character.";
                    std::exit(1);
                }
                break;
        }
    }

    tokens.push_back(Token(EOF_TOKEN, "eof"));
    return tokens;
}