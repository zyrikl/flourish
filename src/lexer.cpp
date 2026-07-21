#include "lexer.h"
#include "file.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <algorithm>

Token::Token(tt type, std::string literal) : type(type), literal(literal) {}

std::vector<Token> token_stream(std::string fn) {
    std::string contents = open_file(fn);
    std::vector<Token> tokens;
    std::vector<std::string> literals;

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
                    std::cerr << "Unexpected character.\n";
                    std::exit(1);
                }
                break;
            case '\n':
                tokens.push_back(Token(NEWLINE, std::string(1, c)));
                break;
            case ' ':
            case '\t':
                // ignore whitespace
                break;
            case '\'':
            case '"': {
                while (
                    (current >= contents.length()) ? '\0' : contents[current] != c && current < contents.length()
                ) {
                    current++;
                }

                if (current >= contents.length()) {
                    std::cerr << "Unterminated string.\n";
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
                    || static_cast<unsigned char>(
                        current >= contents.length() ? '\0' : contents[current]
                    ) == '-'
                    || static_cast<unsigned char>(
                        current >= contents.length() ? '\0' : contents[current]
                    ) == '_'
                ) current++;
                tokens.push_back(Token(CLASS, contents.substr(start+1, current - start - 1)));
                break;
            case '#':
                while (
                    std::isalnum(
                        static_cast<unsigned char>(
                            current >= contents.length() ? '\0' : contents[current]
                        )
                    ) 
                    || static_cast<unsigned char>(
                        current >= contents.length() ? '\0' : contents[current]
                    ) == '-'
                    || static_cast<unsigned char>(
                        current >= contents.length() ? '\0' : contents[current]
                    ) == '_'
                ) current++;
                tokens.push_back(Token(ID, contents.substr(start+1, current - start - 1)));
                break;
            case '=':
                if (
                    static_cast<unsigned char>(
                        current >= contents.length() ? '\0' : contents[current]
                    ) == '='
                ) { tokens.push_back(Token(EQUALS_EQUALS, "==")); } else { tokens.push_back(Token(EQUALS, "=")); }
                break;
            default: 
                if (std::isalpha(static_cast<unsigned char>(c))) {
                    while (std::isalnum(
                        static_cast<unsigned char>(
                            current >= contents.length()) ? '\0' : contents[current]
                        )
                    ) current++;

                    // Attribute detection
                    if (contents[current] == '=') {
                        tokens.push_back(Token(ATTRIBUTE, contents.substr(start, current - start)));
                        // Consume '=' sign
                        current++;
                    }
                    // echo macro
                    else if (contents.substr(start, current - start) == "echo") {
                        tokens.push_back(Token(ECHO, "echo"));
                    }
                    // let keyword
                    else if (contents.substr(start, current - start) == "let") {
                        tokens.push_back(Token(LET, "let"));
                    } 
                    // variable detection
                    else if (
                        // if the token right in front is the let keyword
                        (!tokens.empty() ? tokens.back().type : EOF_TOKEN) == LET
                    ) {
                        // look if variable is already defined
                        if (
                            std::find(
                                literals.begin(), literals.end(), contents.substr(start, current - start)
                            ) != literals.end()
                        ) {
                            std::cerr << "Variable "+contents.substr(start, current - start)+" already defined.\n";
                            std::exit(1);
                        } else {
                            tokens.push_back(Token(VARIABLE, contents.substr(start, current - start)));
                            literals.push_back(contents.substr(start, current - start));
                        }
                    }
                    // variable detection part 2: reevaluation and usage
                    else if (
                        // locate
                        std::find(
                            literals.begin(), literals.end(), contents.substr(start, current - start)
                        ) != literals.end()
                    ) {
                        tokens.push_back(Token(VARIABLE, contents.substr(start, current - start)));
                    }
                    // otherwise it's a tag
                    else {
                        // Assume it's a tag
                        tokens.push_back(Token(TAG, contents.substr(start, current - start)));
                    }
                } else if (c != '=') {
                    std::cerr << "Unexpected character "+std::string(1, c)+".\n";
                    std::exit(1);
                }
                break;
        }
    }

    tokens.push_back(Token(EOF_TOKEN, "eof"));
    return tokens;
}