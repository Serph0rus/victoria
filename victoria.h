#include <stdlib.h>
#include <string.h>
typedef enum ValueType {
    NullType,
    TypeType,
    ErrorType,
    BooleanType,
    IntegerType,
    FloatingType,
    StringType,
    PairType,
    FunctionType,
    BuiltinType,
    SymbolType,
} ValueType;
typedef enum Error {
    NullError, // a null pointer has been passed where a value pointer was expected
    SymbolError, // a symbol has been given to lookup() that does not exist in the environment heirarchy
    ApplicationError, // a value that is not a function or a builtin has been applied to a list
    ArityError, // a function or builtin has been called with an incorrect number of arguments
    TypeError, // a function or builtin has been called with arguments that do not match the types in its definition
    ManipulationError, // a malformed (i.e. not null-terminated) list has been passed to a function
} Error;
typedef int Boolean;
typedef long Integer;
typedef double Floating;
typedef struct String String;
typedef struct Pair Pair;
typedef struct Function Function;
typedef struct Builtin Builtin;
typedef unsigned long Symbol;
typedef struct Value Value;
typedef struct Environment Environment;
void notify(Value * value);
Value * new_boolean(Boolean boolean);
Value * new_integer(Integer integer);
Value * new_floating(Floating floating);
Value * new_string(const char * string);
Value * new_pair(Value * left, Value * right);
Value * new_function(unsigned long count, ValueType * types, Value * body, Environment * closure, unsigned long locals);
Value * new_builtin(unsigned long count, ValueType * types, Value * (* body)(Value * *));
Value * new_symbol(Symbol symbol);
void del(Value * value);
void mark(Value * value);
void collect();
Value * lookup(Environment * environment, Symbol symbol);
Value * evaluate(Value * value, Environment * environment);
Value * apply(Value * function, Value * * arguments);
Value * core_quote(Value * * arguments);
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
    Value * (* body)(Value * *, Environment *);
} Builtin;
typedef struct Value {
    ValueType type : 7;
    int marked : 1;
    union {
        Error error;
        Boolean boolean;
        Integer integer;
        Floating floating;
        String string;
        Pair pair;
        Function function;
        Builtin builtin;
        Symbol symbol;
        ValueType _type;
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