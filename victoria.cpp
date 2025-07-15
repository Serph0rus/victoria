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

#include "victoria.hpp"
unsigned long hash(const char * string, unsigned int length) {
    const unsigned long offset = 0xcbf29ce484222325;
    const unsigned long prime = 0x100000001b3;
    unsigned long hash = offset;
    if (length == -1) {
        length = strlen(string);
    };
    for (int i = 0; (i < length) && (string[i]); i++) {
        hash ^= (unsigned long) string[i];
        hash *= prime;
    }
    return hash;
};
Value::Value(ValueType type) {
    this->type = type;
};
Value::~Value() {
};
Value * Value::evaluate(Package * package) {
    return this;
};
Value * Value::apply(List * list, Package * package) {
    return new Error(Error::Type);
};
Error::Error(long code) : Value(ValueType::ErrorType) {
    this->code = code;
};
Package::Package(Package * outer, std::map<unsigned long, Value *> entries) : Value(ValueType::PackageType) {
    this->outer = outer;
    this->entries = entries;
};
List::List(Value * value, List * next) : Value(ValueType::ListType) {
    this->value = value;
    this->next = next;
};
Value * List::evaluate(Package * package) {
    return this->value->evaluate(package)->apply(this->next, package);
};
Function::Function(unsigned long * parameters, unsigned long count, Value * body, Package * definition_package) : Value(ValueType::FunctionType) {
    this->parameters = parameters;
    this->count = count;
    this->body = body;
    this->closure = new Package(definition_package, std::map<unsigned long, Value *>());
    for (int i = 0; i < count; i++) {
        (* this->closure).entries[parameters[i]] = 0;
    };
};
Function::~Function() {
    delete this->closure;
};
Value * Function::apply(List * list, Package * package) {
    for (int i = 0; i < this->count; i++) {
        if (list) {
            this->closure->entries[this->parameters[i]] = list->value->evaluate(package);
        } else {
            return new Error(Error::Arity);
        };
        list = list->next;
    };
    return this->evaluate(this->closure);
};
Builtin::Builtin(Value * (* pointer)(List *)) : Value(ValueType::BuiltinType) {
    this->pointer = pointer;
};
Value * Builtin::apply(List * list, Package * package) {
    List * new_list = 0;
    List * * new_list_iterator = & new_list;
    while (list) {
        if (dynamic_cast<Error *>(list->value)) {
            return list->value;
        };
        * new_list_iterator = new List(list->value->evaluate(package), 0);
        new_list_iterator = & (* new_list_iterator)->next;
        list = list->next;
    };
    return this->pointer(new_list);
};
Symbol::Symbol(unsigned long descriptor) : Value(ValueType::SymbolType) {
    this->descriptor = descriptor;
};
Value * Symbol::evaluate(Package * package) {
    std::map<unsigned long, Value *>::iterator iterator = package->entries.find(this->descriptor);
    if (iterator == package->entries.end()) {
        if (package->outer) {
            return this->evaluate(package->outer);
        } else {
            return new Error(Error::Undefined);
        };
    } else {
        return iterator->second;
    };
};
// 14/07/2025@21:41 first successful run! added 3 and 5 to get 8.
// 14/07/2025@22:09 first successful symbol evaluation! evaluated "+" to plus().
// 14/07/2025@22:31 first successful run with first-class packages, same as 22:09 run.
// 15/07/2025@00:06 added basic integer operators (can't remember when, realised while making equality operators that boolean is needed)