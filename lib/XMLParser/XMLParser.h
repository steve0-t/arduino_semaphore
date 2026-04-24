#ifndef XML_PARSER_H
#define XML_PARSER_H

// inspired by: https://github.com/ooxi/xml.c
// thank you <3

#include "../Rstr/Rstr.h"

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
#define TAG_NAME_MAX_LEN  16

class XMLParser {
  public:
    XMLParser();
    XMLParser(const char** accepted_commands, const char** accepted_states);
    ~XMLParser();

    uint16_t                  skip_whitespace(const rstr& buf);
    [[nodiscard]] int8_t      set_buffer(const rstr& input);
    void                      reset();
    uint8_t                   is_xml_tag(const rstr& input);
    uint8_t                   is_comment(const rstr& input);
    [[nodiscard]] const char* get_attribute(const char* attr);
    const char*               get_value(uint16_t offset);
    static uint16_t           str_tok_r(const char* str, const char* delim,
                                        uint16_t& nextp);
    uint16_t                  str_str(const rstr& haystack, const char* needle);
    // uint16_t        str_chr(const rstr& s, int c);
    uint16_t str_chr(const char* s, int c);

  private:
    const char** m_AcceptedCommands;
    const char** m_AcceptedStates;
    rstr         m_Buffer;
};

#endif
