#include "XMLParser.h"
#include <cstdint>

XMLParser::XMLParser() {
    m_Buffer = (rstr){.data = nullptr, .len = 0};
}

XMLParser::XMLParser(const rstr& buffer) {
    m_Buffer = buffer;
}

XMLParser::XMLParser(const char* buffer) {
    m_Buffer.len  = strlen(buffer);
    m_Buffer.data = (char*)malloc(m_Buffer.len);
    memcpy((void*)m_Buffer.data, (void*)buffer, m_Buffer.len);
}

#ifdef ARDUINO
XMLParser::XMLParser(const String& buffer) {
    m_Buffer.len  = buffer.length();
    m_Buffer.data = (char*)malloc(m_Buffer.len);
    memcpy((void*)m_Buffer.data, (void*)buffer.c_str(), m_Buffer.len);
}
#else
XMLParser::XMLParser(const std::string& buffer) {
    m_Buffer.len  = buffer.size();
    m_Buffer.data = (char*)malloc(m_Buffer.len);
    memcpy((void*)m_Buffer.data, (void*)buffer.c_str(), m_Buffer.len);
}
#endif

XMLParser::~XMLParser() {
    free(m_Buffer.data);
}

xml_node* XMLParser::parse_node(const rstr& input) {
    if (!is_xml_tag(input))
        return nullptr;

    rstr* input_cpy = rstr_cpy(&input);

    SKIP_WHITESPACE(input_cpy->data);
    if (input_cpy->front() != OPENING_BRACKET)
        return nullptr;

    char *save_ptr, *tok;

    tok = xml_strtok_r(input_cpy->data, " ", &save_ptr);
    if (tok == nullptr)
        return nullptr;

    rstr* tag_name = (rstr*)malloc(sizeof(rstr));
    tag_name       = str_cpy(tok);

    xml_attr* attrs = get_attributes(tok);

    xml_node* ret   = (xml_node*)malloc(sizeof(xml_node));
    ret->attributes = attrs;
    ret->name       = tag_name;
    return ret;
}

xml_attr* XMLParser::get_attributes(char* input) {
    xml_attr* ret = (xml_attr*)malloc(sizeof(xml_attr) * MAX_ATTRS);
    char *    tok, *save_ptr;

    u8        idx = 0;
    while ((tok = strtok_r(nullptr, " ", &save_ptr)) != nullptr) {
        // std::cout << tok << NLN;
        if (idx >= MAX_ATTRS)
            return nullptr;

        ret[idx].name.data = (char*)malloc(MAX_ATTR_NAME_LEN);
        ret[idx].name.len  = strlen(tok);

        if (ret[idx].name.len > MAX_ATTR_NAME_LEN - 1) {
#ifdef ARDUINO
            if (Serial.available()) {
                Serial.print("Invalid length of attribute. Originated at: ");
                Serial.println(tok);
            }
#else
            std::cout << "Invalid length of attribute. Originated at: " << tok
                      << NLN;
#endif
            goto cleanup;
        }

        memcpy((void*)ret[idx++].name.data, (void*)tok, MAX_ATTR_NAME_LEN - 1);
    }

    return ret;

cleanup:
    for (int8_t i = 0; i < MAX_ATTRS; i++) {
        if (ret[i].name.data != nullptr)
            free(ret[i].name.data);
    }
    return nullptr;
}

rstr* XMLParser::rstr_cpy(const rstr* src) {
    if (src == nullptr)
        return nullptr;

    rstr* ret = (rstr*)malloc(sizeof(rstr));
    ret->len  = src->len;
    ret->data = (char*)malloc(ret->len);
    memcpy((void*)ret->data, (void*)src->data, src->len);

    return ret;
}

rstr* XMLParser::str_cpy(const char* src) {
    if (src == nullptr)
        return nullptr;

    uint16_t len = (uint16_t)strlen(src);
    rstr*    ret = (rstr*)malloc(sizeof(rstr));
    ret->data    = (char*)malloc(len);
    ret->len     = len;
    memcpy((void*)ret, (void*)src, len);

    return ret;
}

void   XMLParser::reset() {}

int8_t XMLParser::is_xml_tag(const rstr& input) {
    if (input.data == nullptr || input.len == 0)
        return -1;
    char* tmp = input.data;
    SKIP_WHITESPACE(tmp);
    if (*tmp == OPENING_BRACKET) {
        while (tmp && *tmp != NLN && *tmp != '\0') {
            tmp++;
            if (*tmp == CLOSING_BRACKET)
                return 1;
        }
    }
    return 0;
}

rstr* XMLParser::get_tag_name(const rstr& input) {
    rstr* ret = rstr_cpy(&input);

    SKIP_WHITESPACE(ret->data);

    if (ret && ret->front() == OPENING_BRACKET)
        ret->consume(1);
    ret->data += strspn(ret->data, " ");

    return ret;
}

char* XMLParser::xml_strtok_r(char* str, const char* delim, char** nextp) {
    char* ret;

    if (str == NULL)
        str = *nextp;

    str += strspn(str, delim);

    if (*str == '\0')
        return NULL;

    ret = str;

    str += strcspn(str, delim);

    if (*str)
        *str++ = '\0';

    *nextp = str;

    return ret;
}