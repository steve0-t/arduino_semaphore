#ifndef RSTR_H
#define RSTR_H

#include "../Abbreviations/Abbreviations.h"

struct rstr {
    char* _str;
    u64   len;

    void  clear_str() {
        if (_str == nullptr)
            return;
        for (u64 i = 0; i < len; i++)
            _str[i] = 0;
    }

    char front() const {
        return (_str != nullptr) ? _str[0] : 0;
    }

    char back() const {
        return (_str != nullptr && len != 0) ?
            _str[len - 1] :
            0;
    }
};

#endif