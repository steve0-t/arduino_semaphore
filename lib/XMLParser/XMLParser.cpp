#include "XMLParser.h"
#include "Arduino.h"

i8 XMLParser::isXML(const rstr& input) {
    if (input._str == nullptr)
        return -1;
    // Serial.println(input._str[0]);
    // Serial.println(input._str[input.len - 1]);
    return input.front() == OPENING_BRACKET &&
        input.back() == CLOSING_BRACKET;
}