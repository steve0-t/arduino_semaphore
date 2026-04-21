#include "XMLParser.h"

// inspired by: https://github.com/ooxi/xml.c
// thank you <3

XMLParser::XMLParser(const char** accepted_commands,
                     const char** accepted_states) {
    m_AcceptedCommands = accepted_commands;
    m_AcceptedStates   = accepted_states;
}

XMLParser::~XMLParser() {}

int8_t XMLParser::set_buffer(const rstr& input) {
    if (!is_xml_tag(input))
        return 0;
    m_Buffer = input;
    // std::cout << "ORIGINAL LEN: " << m_Buffer.len << NLN;
    m_Buffer.trim();
    // std::cout << "AFTER TRIMMING LEN: " << m_Buffer.len << NLN;
    return 1;
}

const char* XMLParser::get_attribute(const char* attr) {
    if (attr == nullptr)
        return nullptr;
    // std::cout << m_Buffer.data << NLN;
    uint16_t pos = str_str(m_Buffer, attr);
    // std::cout << m_Buffer.str_view(pos) << NLN;

    // std::cout << "CHECK WHETHER IT IS A SELF CLOSING TAG: "
    //           << m_Buffer.at_pos(m_Buffer.len - 2) << NLN;
    if (m_Buffer.at_pos((uint16_t)(m_Buffer.len - 2)) != '/')
        return nullptr;

    // std::cout << "POS: " << pos << NLN;
    return (pos != UINT16_MAX) ? get_value(pos) : nullptr;
}

const char* XMLParser::get_value(uint16_t offset) {
    uint16_t next_ptr = 0;
    uint16_t ret =
        str_tok_r(m_Buffer.str_view(offset), "\"", next_ptr) + offset + 1;

    // std::cout << NLN << "GET VALUE METHOD" << NLN;
    // std::cout << m_Buffer.str_view(ret) << NLN << NLN;

    // std::cout << m_Buffer.at_pos(ret - 2) << NLN;
    if (m_Buffer.at_pos(ret - 2) != '=')
        return nullptr;

    return m_Buffer.str_view(ret);
}

void    XMLParser::reset() {}

uint8_t XMLParser::is_xml_tag(const rstr& input) {
    if (input.data == nullptr || input.len == 0)
        return 0;

    uint16_t tmp = skip_whitespace(input);
    if (input.data[tmp] == OPENING_BRACKET) {
        while (tmp < input.len && input.data[tmp] != NLN &&
               input.data[tmp] != '\0') {
            tmp++;
            if (input.data[tmp] == CLOSING_BRACKET)
                return 1;
        }
    }
    return 0;
}

uint16_t XMLParser::str_tok_r(const char* str, const char* delim,
                              uint16_t& nextp) {
    uint16_t ret = 0;

    if (str == nullptr)
        ret = nextp;

    ret += strcspn(str, delim);

    nextp = ret;

    return ret;
}

uint16_t XMLParser::skip_whitespace(const rstr& buf) {
    uint16_t ret = 0;
    if (buf.data != nullptr && buf.len > 0) {
        while (ret < buf.len && buf.data[ret] == WHITESPACE)
            ret++;
    }
    return ret;
}

uint16_t XMLParser::str_chr(const char* s, int c) {
    uint16_t ret = 0;
    while (s && *s != 0) {
        if (*s + ret == c)
            break;
        ret++;
    }
    return ret;
}

uint16_t XMLParser::str_str(const rstr& haystack, const char* needle) {
    const size_t len = strlen(needle);
    if (len == 0 || haystack.empty())
        return UINT16_MAX;

    uint16_t ret = 0;
    while (ret < haystack.len) {
        // std::cout << needle << NLN;
        char* c = haystack.str_view(ret);
        // if (c)
        //     std::cout << c << NLN;
        if (c && strncmp(c, needle, len) == 0)
            break;
        ret++;
    }
    return ret;
}
