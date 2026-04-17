#ifndef COMMANDS_H
#define COMMANDS_H

#include <array>
#include <cstddef>
#include <string>
#include <iostream>

#define UNKNOWN           -1
#define OPENING_BRACKET   "<"
#define CLOSING_BRACKET   ">"
#define CLOSING_STATEMENT "/"
#define ASSIGNMENT        "="
#define DELIM             " "

using std::cout;
using std::cin;

enum class Type {
    SET = 0,
    GET,
    COUNT
};

static const std::array<std::string,
                        static_cast<size_t>(Type::COUNT)>
    types = {"set", "get"};

enum Commands {
    STOP = 0,
    READY,
    GO,
    CAUTION,
    OFF,
    COUNT
};

static const std::array<std::string, Commands::COUNT>
    commands = {"STOP", "READY", "GO", "CAUTION", "OFF"};

#endif