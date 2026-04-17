#include "../include/Parser.h"
#include "Commands.h"
#include <cstring>

int XMLParser::parse(std::string input) {
    if (input.empty())
        return UNKNOWN;

    // for (int i = 0; i < Commands::COUNT; i++) {
    //     if (strcasecmp(input.c_str(),
    //                    commands[i].c_str()) == 0) {
    //         return i;
    //     }
    // }
    return UNKNOWN;
}
