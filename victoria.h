#pragma once
#include <stdlib.h>
#include <string.h>
typedef enum ValueType {
    NullType,
    ErrorType,
    TypeType,
    BooleanType,
    IntegerType,
    FloatingType,
    StringType,
    PairType,
    FunctionType,
    BuiltinType,
    SymbolType,
    BytesType,
    StreamType,
} ValueType;
typedef enum Error {
    NullError, // a null pointer has been passed where a value pointer was expected
    SuccessError, // a return value was saved from a function with no return
    SymbolError, // a symbol has been given to environment_get() that does not exist in the environment heirarchy
    ApplicationError, // a value that is not a function or a builtin has been applied to a list
    ArityError, // a function or builtin has been called with an incorrect number of arguments
    TypeError, // a function or builtin has been called with arguments that do not match the types in its definition
    ManipulationError, // a malformed (i.e. not null-terminated) list has been passed to a function
    ArithmeticError, // a value outside of an arithmetic function's domain has been passed to it
    ParseError, // an unexpected token exists in a string passed to a parse function
} Error;
typedef int Boolean;
typedef long Integer;
typedef double Floating;
typedef struct String String;
typedef struct Pair Pair;
typedef struct Function Function;
typedef struct Builtin Builtin;
typedef unsigned long Symbol;
typedef struct Bytes Bytes;
typedef struct Stream Stream;
typedef struct BuiltinStream BuiltinStream;
typedef struct Value Value;
typedef struct Environment Environment;
typedef Value * (* BuiltinFunctionPointer)(Value * *, Environment *);
typedef unsigned long (* BuiltinBytesReadPointer)(long index, unsigned long offset, unsigned long length);
typedef unsigned long (* BuiltinBytesWritePointer)(long index, unsigned long offset, Value * value);
void notify(Value * value);
Value * new_error(Error error);
Value * new_type(ValueType type);
Value * new_boolean(Boolean boolean);
Value * new_integer(Integer integer);
Value * new_floating(Floating floating);
Value * new_string(const char * string);
Value * new_pair(Value * left, Value * right);
Value * new_function(unsigned long count, ValueType * types, Value * body, Environment * closure, unsigned long locals);
Value * new_builtin(unsigned long count, ValueType * types, BuiltinFunctionPointer body);
Value * new_bytes(char * value, unsigned long length);
Value * new_stream(Value * read, Value * write);
Value * new_builtin_stream(BuiltinBytesReadPointer read, BuiltinBytesWritePointer write);
Value * new_symbol(Symbol symbol);
void del(Value * value);
void mark(Value * value);
void collect();
Value * environment_get(Environment * environment, Symbol symbol);
Value * environment_set(Environment * environment, Symbol symbol, Value * value);
Value * evaluate(Value * value, Environment * environment);
Value * apply(Value * function, Value * * arguments, Environment * environment);
Value * builtin_quote(Value * * arguments, Environment * environment);
typedef struct String {
    unsigned long length;
    char * value;
} String;
typedef struct Pair {
    Value * left;
    Value * right;
} Pair;
typedef struct Function {
    unsigned long count;
    ValueType * types;
    Environment * closure;
    unsigned long locals;
    Value * body;
} Function;
typedef struct Builtin {
    unsigned long count;
    ValueType * types;
    BuiltinFunctionPointer body;
} Builtin;
typedef struct Bytes {
    unsigned long length;
    char * value;
} Bytes;
typedef struct Stream {
    union {
        struct {
            Value * read;
            Value * write;
        } user;
        struct {
            BuiltinBytesReadPointer read;
            BuiltinBytesWritePointer write;
            long index;
        } builtin;
    };
    char is_builtin;
} Stream;
typedef struct Value {
    ValueType type : 7;
    int marked : 1;
    union {
        Error error;
        ValueType value_type;
        Boolean boolean;
        Integer integer;
        Floating floating;
        String string;
        Pair pair;
        Function function;
        Builtin builtin;
        Symbol symbol;
        Bytes bytes;
        Stream stream;
    };
} Value;
typedef struct Environment {
    Environment * outer;
    unsigned long offset;
    unsigned long size;
    Value * data[];
} Environment;
void notify(Value * value) {};
void mark(Value * value) {};
void collect() {};