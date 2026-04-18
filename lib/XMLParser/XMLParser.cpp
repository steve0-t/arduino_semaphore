#include "XMLParser.h"

XMLParser::XMLParser() : m_cmdType(TCOUNT), m_state(SCOUNT) {}

Retval XMLParser::parse(const rstr& input) {
    if (!isXML(input))
        return BAD_FORMAT;
    char *beg, *cpy, *curr_char, *prev_char;
    char  buf[256] = {0};
    beg = cpy = input._str;
    u8 idx = 0, buf_idx = 0;
    i8 specification = 0;

    // Serial.println(cpy);
    while ((curr_char = cpy++) != nullptr) {
        // Serial.print(*curr_char);
        // Serial.println(buf);
        // Serial.println(specification);
        if (idx == 0 && *curr_char == '!')
            return COMMENT;

        if (*curr_char == CLOSING_BRACKET) {
            if (buf[buf_idx - 1] != CLOSING_DELIM)
                return BAD_FORMAT;
            break;
        }

        if (*curr_char == WHITESPACE) {
            if (idx == 0) {
                if (strcmp(buf, "cmd") != 0)
                    return BAD_FORMAT;
            }
            if (buf_idx != 0) {
                memset((void*)buf, 0, sizeof(buf));
                buf_idx = 0;
                idx++;
            }
            continue;
        }

        if (*curr_char == OPENING_BRACKET)
            continue;

        if (*curr_char == '"' && buf[buf_idx - 1] == ASSIGNMENT) {
            specification = 1;
            if (strcmp(buf, "type=") == 0 || strcmp(buf, "state=") == 0) {
                memset((void*)buf, 0, sizeof(buf));
                buf_idx = 0;
                continue;
            } else
                return BAD_FORMAT;
        } else if (*curr_char == '"' && specification == 0)
            return BAD_FORMAT;
        else if (*curr_char == '"' && specification == 1) {
            specification = 0;
            if (idx == 1) {
                for (i8 i = 0; i < TCOUNT; i++) {
                    if (strcmp(buf, types[i]) == 0)
                        m_cmdType = (Type)i;
                }
                if (m_cmdType == TCOUNT)
                    return UNKNOWN_CMD;
                m_xmlFields.type = types[m_cmdType];
            }

            if (idx == 2) {
                for (i8 i = 0; i < SCOUNT; i++) {
                    if (strcmp(buf, states[i]) == 0)
                        m_state = (State)i;
                }
                if (m_state == SCOUNT)
                    return BAD_STATE;
                // strcpy(m_xmlFields.state, types[m_cmdType]);
                m_xmlFields.state = states[m_state];
            }
            continue;
        }

        // prev_char      = curr_char;
        buf[buf_idx++] = *curr_char;
    }
    return VALID;
}

void XMLParser::reset() {
    m_cmdType   = TCOUNT;
    m_state     = SCOUNT;
    m_xmlFields = XMLFields();
}

i8 XMLParser::isXML(const rstr& input) {
    if (input._str == nullptr)
        return -1;
    // Serial.println(input._str[0]);
    // Serial.println(input._str[input.len - 1]);
    return input.front() == OPENING_BRACKET && input.back() == CLOSING_BRACKET;
}