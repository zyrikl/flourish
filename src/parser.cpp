#include "parser.h"
#include "lexer.h"
#include "file.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

std::string get_content(std::string fn) {
    if (fn.ends_with(".fsh")) {
        std::string html = "<!DOCTYPE html>\n<html>";

        int current = 0;
        std::vector<Token> tokens = token_stream(fn);
        std::vector<std::string> offenders;
        
        while (current < tokens.size() - 1) {
            Token current_token = tokens[current++];
            if (current_token.type == TAG) {
                html += "<"+current_token.literal;

                // Closed elements -- must be hard-coded
                if (
                    current_token.literal == "area" ||
                    current_token.literal == "base" ||
                    current_token.literal == "br" ||
                    current_token.literal == "col" ||
                    current_token.literal == "embed" ||
                    current_token.literal == "hr" ||
                    current_token.literal == "img" ||
                    current_token.literal == "input" ||
                    current_token.literal == "link" ||
                    current_token.literal == "meta" ||
                    current_token.literal == "source" ||
                    current_token.literal == "track" ||
                    current_token.literal == "wbr" ||
                    current_token.literal == "param"
                ) {
                    bool con = true;
                    while (con) {
                        Token next_token = tokens[current++];

                        switch (next_token.type) {
                            case LEFT_BRACE:
                                std::cerr << "Unexpected opening brace!";
                                std::exit(1);
                                break;
                            case RIGHT_BRACE:
                                html += " />";
                                con = false;
                                break;
                            case NEWLINE:
                                html += " />";
                                con = false;
                                break;
                            case STRING:
                                std::cerr << "Unexpected string!";
                                std::exit(1);
                                break;
                            case ID:
                                html += " id=\""+next_token.literal+"\"";
                                break;
                            case CLASS:
                                html += " class=\""+next_token.literal+"\"";
                                break;
                            case ATTRIBUTE:
                                if (current >= tokens.size() ? EOF_TOKEN : tokens[current].type == STRING) {
                                    html += " "+next_token.literal+"=\""+tokens[current].literal+"\"";
                                    current++;
                                } else {
                                    std::cerr << "Attribute must have string after it.";
                                    std::exit(1);
                                }
                                break;
                            default:
                                std::cerr << "Unexpected token after tag name.";
                                std::exit(1);
                                break;
                        }
                    }
                    if (!html.ends_with(" />")) {
                        html += " />";
                    }
                } else {
                    // Open elements (everything else is assumed to be open)
                    bool con = true;
                    while (con) {
                        Token next_token = tokens[current++];

                        switch (next_token.type) {
                            case LEFT_BRACE:
                                html += ">";
                                offenders.push_back(current_token.literal);
                                con = false;
                                break;
                            case RIGHT_BRACE:
                                html += " />";
                                con = false;
                                break;
                            case NEWLINE:
                                html += " />";
                                con = false;
                                break;
                            case STRING:
                                html += ">"+next_token.literal+"</"+current_token.literal+">";
                                con = false;
                                break;
                            case ID:
                                html += " id=\""+next_token.literal+"\"";
                                break;
                            case CLASS:
                                html += " class=\""+next_token.literal+"\"";
                                break;
                            case ATTRIBUTE:
                                if (current >= tokens.size() ? EOF_TOKEN : tokens[current].type == STRING) {
                                    html += " "+next_token.literal+"=\""+tokens[current].literal+"\"";
                                    current++;
                                } else {
                                    std::cerr << "Attribute must have string after it.";
                                    std::exit(1);
                                }
                                break;
                            default:
                                std::cerr << "Unexpected token after tag name.";
                                std::exit(1);
                                break;
                        }
                    }
                }
            } else if (current_token.type == RIGHT_BRACE) {
                if (offenders.size() < 1) {
                    std::cerr << "Unexpected closing brace.";
                    std::exit(1);
                } else {
                    html += "</"+offenders.back()+">";
                }
            } else if (current_token.type == LEFT_BRACE) {
                std::cerr << "Opening brace needs tag name before it.";
                std::exit(1);
            } else if (current_token.type == ECHO) {
                html += current_token.literal;
            }
        }

        html += "</html>";
        return html;
    } else {
        return open_file(fn);
    }
}