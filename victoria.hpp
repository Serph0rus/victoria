#pragma once
#include <cstring>
#include <map>
unsigned long hash(const char * string, unsigned int length = -1);
enum ValueType {
    ErrorType,
    PackageType,
    BooleanType,
    IntegerType,
    FloatingType,
    StringType,
    ListType,
    FunctionType,
    BuiltinType,
    SymbolType,
};
class Value;
class Error;
class Package;
class Integer;
class Floating;
class String;
class List;
class Function;
class Builtin;
class Symbol;
class Value {
    public:
        ValueType type;
        Value(ValueType type);
        virtual ~Value();
        virtual Value * evaluate(Package * package);
        virtual Value * apply(List * list, Package * package);
};
class Error : public Value {
    public:
        long code;
        Error(long code);
        const static long Undefined = 1000,
                          Allocation = 2000,
                          Arity = 3000,
                          Type = 4000;
};
class Package : Value {
    public:
        Package * outer;
        std::map<unsigned long, Value *> entries;
        Package(Package * outer, std::map<unsigned long, Value *> entries);
};
class List : public Value {
    public:
        Value * value;
        List * next;
        List(Value * value, List * next);
        Value * evaluate(Package * package);
};
class Function : public Value {
    public:
        unsigned long * parameters;
        unsigned long count;
        Value * body;
        Package * closure;
        Function(unsigned long * parameters, unsigned long count, Value * body, Package * definition_package);
        ~Function();
        Value * apply(List * list, Package * package);
};
class Builtin : public Value {
    public:
        Value * (* pointer)(List *);
        Builtin(Value * (* pointer)(List *));
        Value * apply(List * list, Package * package);
};
class Symbol : public Value {
    public:
        unsigned long descriptor;
        Symbol(unsigned long descriptor);
        Value * evaluate(Package * package);
};