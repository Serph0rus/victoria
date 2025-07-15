#pragma once
#include "victoria.hpp"
class Integer : public Value {
    public:
        long value;
        Integer(long value);
};
Package * integer_package();