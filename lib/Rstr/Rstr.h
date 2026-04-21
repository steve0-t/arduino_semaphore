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

    void     clear_str() {
        if (data == nullptr)
            return;
        for (uint16_t i = 0; i < len; i++)
            data[i] = 0;
    }

    char front() const {
        return (data != nullptr) ? data[0] : 0;
    }

    char back() const {
        return (data != nullptr && len != 0) ? data[len - 1] : 0;
    }
};

#endif