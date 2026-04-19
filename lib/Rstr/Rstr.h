#ifndef RSTR_H
#define RSTR_H

#include "../Abbreviations/Abbreviations.h"

struct rstr {
    char* data;
    u16   len;

    void  clear_str() {
        if (data == nullptr)
            return;
        for (u16 i = 0; i < len; i++)
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