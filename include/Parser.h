#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "Commands.h"
#include <strings.h>

class XMLParser {
  public:
    XMLParser() {}
    ~XMLParser() {}

    int parse(std::string command);

  private:
    char m_currentToken;
};

#endif