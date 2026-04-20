#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "../Abbreviations/Abbreviations.h"
#include "../Rstr/Rstr.h"
#include "../XMLFields/XMLFields.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <HardwareSerial.h>
#else
#include <cstring>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#endif

#define OPENING_BRACKET '<'
#define CLOSING_BRACKET '>'
#define CLOSING_DELIM   '/'
#define WHITESPACE      ' '
#define ASSIGNMENT      '='
#define NLN             '\n'

#define MAX_ATTRS         2
#define MAX_ATTR_NAME_LEN 32

#define SKIP_WHITESPACE(str)                                                   \
    do {                                                                       \
        if (*(str) != ' ')                                                     \
            break;                                                             \
        *(str)++;                                                              \
    } while (1)

enum Type {
    SET = 0,
    GET,
    PING,
    TCOUNT
};

static const char* types[TCOUNT] = {"set", "get", "ping"};

enum State {
    STOP = 0,
    READY,
    GO,
    CAUTION,
    OFF,
    SCOUNT
};

static const char* states[SCOUNT] = {"STOP", "READY", "GO", "CAUTION", "OFF"};

enum Retval {
    BAD_FORMAT  = 1,
    UNKNOWN_CMD = 2,
    BAD_STATE   = 3,
    VALID       = 33,
    COMMENT
};

static const char* responses[3] = {"BAD_FORMAT", "UNKNOWN_CMD", "BAD_STATE"};

typedef struct {
    rstr name;
    rstr value;
} xml_attr;

typedef struct {
    rstr*     name;
    rstr      content;
    xml_attr* attributes;
} xml_node;

class XMLParser {
  public:
    XMLParser();
    XMLParser(const rstr& buffer);
    XMLParser(const char* buffer);
#ifdef ARDUINO
    XMLParser(const String& buffer);
#else
    XMLParser(const std::string& buffer);
#endif
    ~XMLParser();

    // void      set_buffer(const rstr& buffer);

    void         reset();
    int8_t       is_xml_tag(const rstr& input);
    xml_node*    parse_node(const rstr& input);
    xml_attr*    get_attributes(char* input);
    rstr*        get_tag_name(const rstr& input);
    rstr*        rstr_cpy(const rstr* src);
    rstr*        str_cpy(const char* src);
    static char* xml_strtok_r(char* str, const char* delim, char** nextp);

  private:
    rstr m_Buffer;
};

#endif
