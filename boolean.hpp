#pragma once
#include "victoria.hpp"
class Boolean : public Value {
    public:
        bool value;
        Boolean(bool value);
};
Package * boolean_package();