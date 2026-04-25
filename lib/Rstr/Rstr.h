#ifndef RSTR_H
#define RSTR_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

struct rstr {
    char*    data;
    uint16_t len;

    char*    str_view(uint16_t pos) const {
        if (pos < len && !empty() && pos >= 0)
            return data + pos;
        return nullptr;
    }

    void remove_data() {
        len  = 0;
        data = nullptr;
    }

    void clear_str() {
        if (data == nullptr)
            return;
        for (uint16_t i = 0; i < len; i++)
            data[i] = 0;
        len = 0;
    }

    int8_t empty() const {
        return data == nullptr || len == 0;
    }

    char at_pos(uint16_t i) const {
        return (!empty() && i >= 0 && i < len) ? data[i] : 0;
    }

    char front() const {
        return !empty() ? data[0] : 0;
    }

    // returns last character of data, taking whitespace into consideration
    char back() const {
        if (!empty()) {
            for (int16_t i = len; i >= 0; i--) {
                if (data[i] != ' ')
                    return data[i];
            }
        }
        return 0;
    }

    void trim() {
        if (empty())
            return;

        uint16_t start = 0;
        while (start < len && data[start] == ' ')
            start++;

        uint16_t end = len;
        while (end > start && data[end - 1] == ' ')
            end--;

        data = data + start;
        len  = end - start;
    }
};

#endif