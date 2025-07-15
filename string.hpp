#pragma once
#include "victoria.hpp"
class String : public Value {
    public:
        char * value;
        long length;
        String(const char * value);
};