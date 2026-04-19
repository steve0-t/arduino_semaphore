#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "../Abbreviations/Abbreviations.h"
#include "../Rstr/Rstr.h"
#include "../XMLFields/XMLFields.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstring>
#endif

#define OPENING_BRACKET '<'
#define CLOSING_BRACKET '>'
#define CLOSING_DELIM   '/'
#define WHITESPACE      ' '
#define ASSIGNMENT      '='

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

class XMLParser {
  public:
    XMLParser();
    ~XMLParser();

    void   reset();
    i8     isXML(const rstr& input);
    Retval parse(const rstr& input);

    Type   getCmpType() const {
        return m_cmdType;
    }

    State getState() const {
        return m_state;
    }

    XMLFields getFields() const {
        return m_xmlFields;
    }

  private:
    Type      m_cmdType;
    State     m_state;
    XMLFields m_xmlFields;
};

#endif