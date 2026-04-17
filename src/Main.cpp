#include "../include/Parser.h"
#include "Commands.h"
#include <string>

#define ENDL "\n"

int main() {
    XMLParser   parser = XMLParser();

    std::string input;
    for (;;) {
        std::getline(cin, input);
        if (parser.parse(input) == UNKNOWN)
            continue;
    }

    return 0;
}