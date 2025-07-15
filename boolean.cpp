//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  An interpreter for Victoria, a list-based programming language          //
//  Copyright (C) 2025 Tanika Claire Mellifont-Young                        //
//                                                                          //
//  This program is free software: you can redistribute it and/or modify    //
//  it under the terms of the GNU General Public License as published by    //
//  the Free Software Foundation, either version 3 of the License, or       //
//  (at your option) any later version.                                     //
//                                                                          //
//  This program is distributed in the hope that it will be useful,         //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of          //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
//  GNU General Public License for more details.                            //
//                                                                          //
//  You should have received a copy of the GNU General Public License       //
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include "boolean.hpp"
Boolean::Boolean(bool value) : Value(ValueType::BooleanType) {
    this->value = value;
};
Value * boolean_and(List * list) {
    bool result = true;
    while (list) {
        if (list->value->type == ValueType::BooleanType) {
            result &= dynamic_cast<Boolean *>(list->value)->value;
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Boolean(result);
};
Value * boolean_or(List * list) {
    bool result = true;
    while (list) {
        if (list->value->type == ValueType::BooleanType) {
            result |= dynamic_cast<Boolean *>(list->value)->value;
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Boolean(result);
};
Value * boolean_xor(List * list) {
    bool result = true;
    while (list) {
        if (list->value->type == ValueType::BooleanType) {
            result ^= dynamic_cast<Boolean *>(list->value)->value;
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Boolean(result);
};
Value * boolean_not(List * list) {
    bool result = true;
    if (list) {
        if (list->value->type == ValueType::BooleanType) {
            result = !dynamic_cast<Boolean *>(list->value)->value;
            if (list->next) {
                return new Error(Error::Arity);
            } else {
                return new Boolean(result);
            };
        } else {
            return new Error(Error::Type);
        };
    };
};
Value * equal(List * list) {
    bool first = false;
    bool past_first = false;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            if (past_first) {
                if (first != dynamic_cast<Boolean *>(list->value)->value) {
                    return new Boolean(false);
                };
            } else {
                first = dynamic_cast<Boolean *>(list->value)->value;
                past_first = true;
            };
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Boolean(true);
};
Value * not_equal(List * list) {
    bool first = false;
    bool past_first = false;
    if (list) {
        if (list->value->type == ValueType::BooleanType) {
            if (list->next->type == ValueType::ListType) {
                if (list->next->next == 0) { // 3 booleans must have at least 1 equal to another
                    if (list->next->value->type)
                } else {
                    if (list->next->next->value->type == ValueType::BooleanType) {
                        return new Boolean(false);
                    } else {
                        return new Error(Error::Type);
                    };
                };
            } else {
                return new Boolean(true);
            };
        } else {
            return new Error(Error::Type);
        };
    };
};
Package * integer_package_value = new Package((Package *) 0, std::map<unsigned long, Value *>({
    {hash("integer::+"), new Builtin(plus)},
    {hash("integer::-"), new Builtin(minus)},
    {hash("integer::*"), new Builtin(times)},
    {hash("integer::/"), new Builtin(divide)},
    {hash("integer::%"), new Builtin(modulo)},
    {hash("integer::&"), new Builtin(bitwise_and)},
    {hash("integer::|"), new Builtin(bitwise_or)},
    {hash("integer::^"), new Builtin(bitwise_xor)},
    {hash("integer::!"), new Builtin(bitwise_not)},
}));