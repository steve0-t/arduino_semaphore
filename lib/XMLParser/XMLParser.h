#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "../Abbreviations/Abbreviations.h"
#include "../Rstr/Rstr.h"

#define OPENING_BRACKET '<'
#define CLOSING_BRACKET '>'
#define CLOSING_DELIM   '/'

class XMLParser {
  public:
    XMLParser() {};
    ~XMLParser() {};

    i8   isXML(const rstr& input);
    void parse(const rstr* input);

  private:
};

#endif