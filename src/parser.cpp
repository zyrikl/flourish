#include "parser.h"
#include "lexer.h"
#include "file.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_map>

std::string get_content(std::string fn) {
    if (fn.ends_with(".fsh")) {
        std::string html = "<!DOCTYPE html>\n<html>";

        int current = 0;
        std::vector<Token> tokens = token_stream(fn);
        std::vector<std::string> offenders;

        std::unordered_map<std::string, std::string> vars;
        
        while (current < tokens.size() - 1) {
            Token current_token = tokens[current++];
            
            switch (current_token.type) {
                case TAG: {
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
                                    std::cerr << "Unexpected opening brace!\n";
                                    std::exit(1);
                                    break;
                                case RIGHT_BRACE:
                                    // bring current back down
                                    current--;
                                    con = false;
                                    break;
                                case NEWLINE:
                                    html += " />";
                                    con = false;
                                    break;
                                case STRING:
                                    std::cerr << "Unexpected string!\n";
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
                                        std::cerr << "Attribute must have string after it.\n";
                                        std::exit(1);
                                    }
                                    break;
                                default:
                                    std::cerr << "Unexpected token after tag name.\n";
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
                                    html += "></"+current_token.literal+">";
                                    // bring current back down
                                    current--;
                                    con = false;
                                    break;
                                case NEWLINE:
                                    html += "></"+current_token.literal+">";
                                    con = false;
                                    break;
                                case STRING:
                                    html += ">"+next_token.literal+"</"+current_token.literal+">";
                                    con = false;
                                    break;
                                case VARIABLE:
                                    html += ">"+vars[next_token.literal]+"</"+current_token.literal+">";
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
                                        std::cerr << "Attribute must have string after it.\n";
                                        std::exit(1);
                                    }
                                    break;
                                default:
                                    std::cerr << "Unexpected token after tag name.\n";
                                    std::exit(1);
                                    break;
                            }
                        }
                    }
                    break;
                }
                case RIGHT_BRACE:
                    if (offenders.size() < 1) {
                        std::cerr << "Unexpected closing brace.\n";
                        std::exit(1);
                    } else {
                        html += "</"+offenders.back()+">";
                        offenders.pop_back();
                    }
                    break;
                case LEFT_BRACE:
                    std::cerr << "Opening brace needs tag name before it.\n";
                    std::exit(1);
                    break;
                case ECHO:
                    if (current >= tokens.size() - 1) {
                        std::cerr << "echo macro requires string argument.\n";
                        std::exit(1);
                    } else {
                        if (tokens[current+1].type == STRING) {
                            html += tokens[current++].literal;
                        } else if (tokens[current+1].type == VARIABLE) {
                            html += vars[tokens[current++].literal];
                        } else {
                            // Other variables to echo here
                        }
                    }
                    break;
                case STRING:
                    html += current_token.literal;
                    break;
                case VARIABLE:
                    if (tokens[current].type == EQUALS) {
                        if (current < tokens.size() - 2) {
                            vars[current_token.literal] = tokens[++current].literal;
                            current++;
                        } else {
                            std::cerr << "Unfinished variable declaration.\n";
                            std::exit(1);
                        }
                    } else {
                        //           -2                   -1                     0
                        //    [value we want]    [current_token's current]  [current now]
                        /*if (!tokens.empty() ? tokens[current - 2].type : EOF_TOKEN == LET) {
                            vars[current_token.literal] = "";
                        }*/
                        html += vars[current_token.literal];
                    }
                    break;
                case LET:
                    // handled in lexing
                    break;
                default:
                    break;
            }
        }

        html += "</html>";
        return html;
    } else {
        return open_file(fn);
    }
}