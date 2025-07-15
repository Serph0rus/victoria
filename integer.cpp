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

#include "integer.hpp"
Integer::Integer(long value) : Value(ValueType::IntegerType) {
    this->value = value;
};
Value * plus(List * list) {
    long sum = 0;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            sum += dynamic_cast<Integer *>(list->value)->value;
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(sum);
};
Value * minus(List * list) {
    long difference = 0;
    bool past_first = false;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            if (past_first) {
                difference -= dynamic_cast<Integer *>(list->value)->value;
            } else {
                difference = dynamic_cast<Integer *>(list->value)->value;
                past_first = true;
            };
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(difference);
};
Value * times(List * list) {
    long product = 0;
    bool past_first = false;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            if (past_first) {
                product *= dynamic_cast<Integer *>(list->value)->value;
            } else {
                product = dynamic_cast<Integer *>(list->value)->value;
                past_first = true;
            };
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(product);
};
Value * divide(List * list) {
    long quotient = 0;
    bool past_first = false;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            if (past_first) {
                quotient /= dynamic_cast<Integer *>(list->value)->value;
            } else {
                quotient = dynamic_cast<Integer *>(list->value)->value;
                past_first = true;
            };
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(quotient);
};
Value * modulo(List * list) {
    long modulus = 0;
    bool past_first = false;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            if (past_first) {
                modulus %= dynamic_cast<Integer *>(list->value)->value;
            } else {
                modulus = dynamic_cast<Integer *>(list->value)->value;
                past_first = true;
            };
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(modulus);
};
Value * bitwise_and(List * list) {
    long result = 0;
    bool past_first = false;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            if (past_first) {
                result &= dynamic_cast<Integer *>(list->value)->value;
            } else {
                result = dynamic_cast<Integer *>(list->value)->value;
                past_first = true;
            };
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(result);
};
Value * bitwise_or(List * list) {
    long result = 0;
    bool past_first = false;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            if (past_first) {
                result |= dynamic_cast<Integer *>(list->value)->value;
            } else {
                result = dynamic_cast<Integer *>(list->value)->value;
                past_first = true;
            };
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(result);
};
Value * bitwise_xor(List * list) {
    long result = 0;
    bool past_first = false;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            if (past_first) {
                result ^= dynamic_cast<Integer *>(list->value)->value;
            } else {
                result = dynamic_cast<Integer *>(list->value)->value;
                past_first = true;
            };
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(result);
};
Value * bitwise_not(List * list) {
    long result = 0;
    if (list) {
        if (list->value->type == ValueType::IntegerType) {
            result = ~dynamic_cast<Integer *>(list->value)->value;
            if (list->next) {
                return new Error(Error::Arity);
            } else {
                return new Integer(result);
            };
        } else {
            return new Error(Error::Type);
        };
    };
};
Value * plus(List * list) {
    long first = 0;
    while (list) {
        if (list->value->type == ValueType::IntegerType) {
            sum += dynamic_cast<Integer *>(list->value)->value;
        } else {
            return new Error(Error::Type);
        };
        list = list->next;
    };
    return new Integer(sum);
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
Package * integer_package() {
    return integer_package_value;
};