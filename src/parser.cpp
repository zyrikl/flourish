#include "parser.h"
#include "lexer.h"
#include "file.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_map>

/*      Base elements that are closed
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
        current_token.literal == "param")*/

Parser::Parser(std::string fn) : fn(fn) {
    html = "<!DOCTYPE html><html>";
    current = 0;
    tokens = token_stream(fn);
}

/*       In a regular stream:
 *                             current_token         <next_token>
 *                            (token we're on)       (token after)
 *
 *                             <token[current]>     token[current]
 *                                before ++             after ++
 *                             token[current-1]     token[current]*/

std::string Parser::mainloop() {
    std::string rva;
    bool con = true;
    while (con) {
        if (current < tokens.size()) con = false;
        current_token = tokens[current++];
        rva += handle_token();
    }
    return rva;
}

std::string Parser::handle_token() {
    std::string rval;
    bool con = true;
    while (con) {
        if (current >= tokens.size()) con = false;
        current_token = tokens[current++];

        switch (current_token.type) {
            case RIGHT_BRACE:
                con = false;
                // go back so the tag can consume the token
                current--;
                break;
            case ECHO:
                if (tokens[current].type != EOF_TOKEN) {
                    switch (tokens[current].type) {
                        case STRING:
                            rval += tokens[current].literal;
                            break;
                        case VARIABLE:
                            if (vars.contains(tokens[current].literal)) {
                                rval += vars[tokens[current].literal];
                            } else {
                                std::cerr << "Variable "+tokens[current].literal+" not defined to echo macro.\n";
                                std::exit(1);
                            }
                            break;
                    }
                    current++;
                }
                break;
            case STRING:
                rval += current_token.literal;
                break;
            case VARIABLE:
                if (tokens[current].type == EQUALS) {
                    switch (tokens[current+1].type) {
                        case EOF_TOKEN:
                            std::cerr << "Did not finish variable declaration.\n";
                            std::exit(1);
                            break;
                        case STRING:
                            vars[current_token.literal] = tokens[current+1].literal;
                            current += 2;
                            break;
                        case TAG:
                            current++; //skip equals
                            current_token = tokens[current++];
                            vars[current_token.literal] = tag_node();
                            break;
                        default:
                            break;
                    }
                } else {
                    if (vars.contains(current_token.literal)) {
                        rval += vars[current_token.literal];
                    } else {
                        vars[current_token.literal] = "";
                    }
                }
                break;
            case TAG: 
                rval += tag_node();
                break;
            case LET:
                break;
            case EQUALS:
                break;
            case EOF_TOKEN:
                con = false;
                return rval;
                break;
            default:
                std::cerr << "Invalid token.\n";
                std::exit(1);
                break;
        }
    }
    return rval;
}

std::string Parser::tag_node() {
    std::string rval = "<"+current_token.literal;
    std::vector<std::string> classes;
    bool con = true;
    bool done_id = false;
    //d
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
        while (con) {
            Token next_token = tokens[current++];
            switch (next_token.type) {
                case ATTRIBUTE:
                    if (tokens[current].type == STRING) {
                        rval += " "+next_token.literal+"=\""+tokens[current].literal+"\"";
                        current++;
                    } else if (tokens[current].type == VARIABLE) {
                        if (vars.contains(tokens[current].literal)) {
                            rval += " "+next_token.literal+"=\""+vars[tokens[current].literal]+"\"";
                        } else {
                            std::cerr << "Variable needs value to be attribute value.\n";
                            std::exit(1);
                        }
                        current++;
                    } else {
                        std::cerr << "Invalid type after attribute.\n";
                        std::exit(1);
                    }
                    break;
                case ID:
                    if (!done_id) {
                        rval += " id=\""+next_token.literal+"\"";
                        done_id = true;
                    } else {
                        std::cerr << "Cannot have more than 1 (one) id.\n";
                        std::exit(1);
                    }
                    break;
                case CLASS:
                    classes.push_back(next_token.literal);
                    break;
                case EOF_TOKEN:
                    con = false;
                    //handle_token();
                    break;
                default:
                    if (!classes.empty()) {
                        rval += " class=\"";
                        for (auto cls : classes) {
                            rval += cls+" ";
                        }
                        classes.clear();
                        classes.shrink_to_fit();
                        rval.pop_back();
                        rval += "\"";
                    }
                    rval += " />";
                    con = false;
                    current--; // let handle_token() handle it
                    break;
            }
        }
    } else {
        while (con) {
            Token next_token = tokens[current++];
            switch (next_token.type) {
                case ATTRIBUTE:
                    if (tokens[current].type == STRING) {
                        rval += " "+next_token.literal+"=\""+tokens[current].literal+"\"";
                        current++;
                    } else if (tokens[current].type == VARIABLE) {
                        if (vars.contains(tokens[current].literal)) {
                            rval += " "+next_token.literal+"=\""+vars[tokens[current].literal]+"\"";
                        } else {
                            std::cerr << "Variable needs value to be attribute value.\n";
                            std::exit(1);
                        }
                        current++;
                    } else {
                        std::cerr << "Invalid type after attribute.\n";
                        std::exit(1);
                    }
                    break;
                case ID:
                    if (!done_id) {
                        rval += " id=\""+next_token.literal+"\"";
                        done_id = true;
                    } else {
                        std::cerr << "Cannot have more than 1 (one) id.\n";
                        std::exit(1);
                    }
                    break;
                case CLASS:
                    classes.push_back(next_token.literal);
                    break;
                case STRING:
                    if (!classes.empty()) {
                        rval += " class=\"";
                        for (auto cls : classes) {
                            rval += cls+" ";
                        }
                        classes.clear();
                        classes.shrink_to_fit();
                        rval.pop_back();
                        rval += "\"";
                    }

                    rval += ">"+next_token.literal+"</"+current_token.literal+">";

                    con = false;
                    break;
                case VARIABLE:
                    if (!classes.empty()) {
                        rval += " class=\"";
                        for (auto cls : classes) {
                            rval += cls+" ";
                        }
                        classes.clear();
                        classes.shrink_to_fit();
                        rval.pop_back();
                        rval += "\"";
                    }

                    if (vars.contains(next_token.literal)) {
                        rval += ">"+vars[next_token.literal]+"</"+current_token.literal+">";
                    } else {
                        std::cerr << "Variable "+next_token.literal+" not assigned value.\n";
                        std::exit(1);
                    }

                    con = false;
                    break;
                case LEFT_BRACE:
                    if (!classes.empty()) {
                        rval += " class=\"";
                        for (auto cls : classes) {
                            rval += cls+" ";
                        }
                        classes.clear();
                        classes.shrink_to_fit();
                        rval.pop_back();
                        rval += "\"";
                    }
                    rval += ">";

                    offenders.push_back(current_token.literal);
                    rval += handle_token();
                    break;
                case RIGHT_BRACE:
                    if (!offenders.empty()) {
                        rval += "</"+offenders.back()+">";
                    } else {
                        std::cerr << "Unexpected closing brace.\n";
                        std::exit(1);
                    }

                    con = false;
                    //handle_token();
                    break;
                case EOF_TOKEN:
                    con = false;
                    //handle_token();
                    break;
                default:
                    std::cerr << "Warning: tokens after tag node.\n";
                    break;
            }
        }
    }
    return rval;
}

std::string Parser::get_content() {
    if (fn.ends_with(".fsh")) {
        for (Token token : tokens) {
            switch (token.type) {
                case TAG:
                    std::cout << "TAG:"+token.literal+", ";
                    break;
                case VARIABLE:
                    std::cout << "VARIABLE:"+token.literal+", ";
                    break;
                case LET:
                    std::cout << "LET:"+token.literal+", ";
                    break;
                case STRING:
                    std::cout << "STRING:\""+token.literal+"\", ";
                    break;
                case EQUALS:
                    std::cout << "EQUALS:"+token.literal+", ";
                    break;
                case ATTRIBUTE:
                    std::cout << "ATTRIBUTE:"+token.literal+", ";
                    break;
                case ID:
                    std::cout << "ID:"+token.literal+", ";
                    break;
                case CLASS:
                    std::cout << "CLASS:"+token.literal+", ";
                    break;
            }
        }
        std::cout << std::endl;
        html += handle_token();
        html += "</html>";
        return html;
    } else {
        return open_file(fn);
    }
}