#pragma once
#include "victoria.hpp"
class Floating : public Value {
    public:
        double value;
        Floating(double value);
};
Package * floating_package();