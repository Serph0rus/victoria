#include "victoria.h"
#include "main.h"
char * strdup(const char * string) {
    unsigned long length = strlen(string);
    char * new_string = malloc(length);
    memcpy(new_string, string, length);
    return new_string;
};
Value * new_null() {
    Value * value = malloc(sizeof(Value));
    value->type = NullType;
    notify(value);
    return value;
};
Value * new_error(Error error) {
    Value * value = malloc(sizeof(Value));
    value->type = ErrorType;
    value->error = error;
    notify(value);
    return value;
};
Value * new_type(ValueType type) {
    Value * value = malloc(sizeof(Value));
    value->type = TypeType;
    value->value_type = type;
    notify(value);
    return value;
};
Value * new_boolean(Boolean boolean) {
    Value * value = malloc(sizeof(Value));
    value->type = BooleanType;
    value->boolean = boolean;
    notify(value);
    return value;
};
Value * new_integer(Integer integer) {
    Value * value = malloc(sizeof(Value));
    value->type = IntegerType;
    value->integer = integer;
    notify(value);
    return value;
};
Value * new_floating(Floating floating) {
    Value * value = malloc(sizeof(Value));
    value->type = FloatingType;
    value->floating = floating;
    notify(value);
    return value;
};
Value * new_string(const char * string) {
    Value * value = malloc(sizeof(Value));
    value->type = StringType;
    value->string.length = strlen(string);
    value->string.value = strdup(string);
    notify(value);
    return value;
};
Value * new_pair(Value * left, Value * right) {
    Value * value = malloc(sizeof(Value));
    value->type = PairType;
    value->pair.left = left;
    value->pair.right = right;
    notify(value);
    return value;
};
Value * new_function(unsigned long count, ValueType * types, Value * body, Environment * closure, unsigned long locals) {
    Value * value = malloc(sizeof(Value));
    value->type = FunctionType;
    value->function.count = count;
    value->function.types = types;
    value->function.closure = closure;
    value->function.locals = locals;
    notify(value);
    return value;
};
Value * new_builtin(unsigned long count, ValueType * types, BuiltinFunctionPointer body) {
    Value * value = malloc(sizeof(Value));
    value->type = BuiltinType;
    value->builtin.count = count;
    value->builtin.types = types;
    value->builtin.body = body;
    notify(value);
    return value;
};
Value * new_symbol(Symbol symbol) {
    Value * value = malloc(sizeof(Value));
    value->type = SymbolType;
    value->symbol = symbol;
    notify(value);
    return value;
};
Value * new_bytes(char * data, unsigned long length) {
    Value * value = malloc(sizeof(Value));
    value->type = BytesType;
    value->bytes.length = length;
    value->bytes.value = malloc(length);
    if (data) memcpy(value->bytes.value, data, length);
    notify(value);
    return value;
};
Value * new_stream(Value * read, Value * write) {
    Value * value = malloc(sizeof(Value));
    value->type = StreamType;
    value->stream.is_builtin = 0;
    value->stream.user.read = read;
    value->stream.user.write = write;
    notify(value);
    return value;
};
Value * new_builtin_stream(BuiltinBytesReadPointer read, BuiltinBytesWritePointer write, long index) {
    Value * value = malloc(sizeof(Value));
    value->type = StreamType;
    value->stream.is_builtin = 1;
    value->stream.builtin.read = read;
    value->stream.builtin.write = write;
    value->stream.builtin.index = index;
    notify(value);
    return value;
};
Environment * new_environment(Environment * outer, unsigned long size) {
    Environment * environment = malloc(sizeof(Environment) + size * sizeof(Value *));
    environment->outer = outer;
    environment->offset = outer ? outer->offset + outer->size : 0;
    environment->size = size;
    for (int i = 0; i < size; i++) {
        environment->data[i] = 0;
    };
    return environment;
};
void del_environment(Environment * environment) {
    free(environment);
};
void del(Value * value) {
    if (value) {
        switch (value->type) {
            case StringType: {
                free(value->string.value);
            } break;
            case FunctionType: {
                free(value->function.types);
            } break;
            case BuiltinType: {
                free(value->builtin.types);
            } break;
        };
        free(value);
    };
};
Value * environment_get(Environment * environment, Symbol symbol) {
    while (environment) {
        if ((symbol >= environment->offset) && (symbol < environment->offset + environment->size)) {
            if (environment->data[symbol - environment->offset]) {
                return environment->data[symbol - environment->offset];
            } else return new_error(SymbolError);
        } else environment = environment->outer;
    };
    return new_error(SymbolError);
};
Value * environment_set(Environment * environment, Symbol symbol, Value * value) {
    while (environment) {
        if ((symbol >= environment->offset) && (symbol < environment->offset + environment->size)) {
            environment->data[symbol - environment->offset] = value;
            return new_error(SuccessError);
        } else environment = environment->outer;
    };
    return new_error(SymbolError);
};
Value * evaluate(Value * value, Environment * environment) {
    if (value) {
        switch (value->type) {
            case SymbolType: return environment_get(environment, value->symbol);
            case PairType: {
                Value * function = evaluate(value->pair.left, environment);
                ValueType * parameter_types = 0;
                unsigned long parameter_count = 0;
                switch (function->type) {
                    case FunctionType: {
                        parameter_types = function->function.types;
                        parameter_count = function->function.count;
                    } break;
                    case BuiltinType: {
                        parameter_types = function->builtin.types;
                        parameter_count = function->builtin.count;
                    } break;
                    case ErrorType: {
                        return function;
                    } break;
                    default: {
                        return new_error(ApplicationError);
                    } break;
                };
                Value * current_pair = value->pair.right;
                Value * * evaluated_arguments = malloc(parameter_count * sizeof(Value *));
                for (int i = 0; i < parameter_count; i++) {
                    if (!current_pair) return new_error(ArityError);
                    if (current_pair->type != PairType) {
                        free(evaluated_arguments);
                        return new_error(ManipulationError);
                    };
                    Value * value = ((function->type == BuiltinType) && (function->builtin.body == builtin_quote)) ? current_pair->pair.left : evaluate(current_pair->pair.left, environment);
                    if ((value->type == parameter_types[i]) || (parameter_types[i] == NullType)) {
                        evaluated_arguments[i] = value;
                        current_pair = current_pair->pair.right;
                    } else {
                        free(evaluated_arguments);
                        if (value->type == ErrorType) return value;
                        else return new_error(TypeError);
                    };
                };
                if (current_pair) {
                    free(evaluated_arguments);
                    return new_error(ArityError);
                };
                Value * result = apply(function, evaluated_arguments, environment);
                free(evaluated_arguments);
                return result;
            } break;
            default: return value;
        };
    } else return 0;
};
Value * apply(Value * function, Value * * arguments, Environment * caller_environment) {
    if (function->type == FunctionType) {
        Environment * environment = new_environment(function->function.closure, function->function.count + function->function.locals);
        for (int i = 0; i < function->function.count; i++) {
            environment->data[i] = arguments[i];
        };
        return evaluate(function->function.body, environment);
    } else if (function->type == BuiltinType) {
        return function->builtin.body(arguments, caller_environment);
    } else return new_error(ApplicationError);
};
Value * builtin_quote(Value * * arguments, Environment * environment) {
    return arguments[0];
};
