#include "victoria.h"
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
Value * new_builtin(unsigned long count, ValueType * types, Value * (* body)(Value * *)) {
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
    free(environment->data);
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
Value * lookup(Environment * environment, Symbol symbol) {
    while (environment) {
        if ((symbol >= environment->offset) && (symbol < environment->offset + environment->size)) {
            if (environment->data[symbol - environment->offset]) {
                return environment->data[symbol - environment->offset];
            } else return new_error(SymbolError);
        } else environment = environment->outer;
    };
    return new_error(SymbolError);
};
Value * evaluate(Value * value, Environment * environment) {
    if (value) {
        switch (value->type) {
            case SymbolType: return lookup(environment, value->symbol);
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
                    Value * value = evaluate(current_pair->pair.left, environment);
                    if (value->type == parameter_types[i]) {
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
                Value * result = apply(function, evaluated_arguments);
                free(evaluated_arguments);
                return result;
            } break;
            default: return value;
        };
    } else return 0;
};
Value * apply(Value * function, Value * * arguments) {
    if (function->type == FunctionType) {
        Environment * environment = new_environment(function->function.closure, function->function.count + function->function.locals);
        for (int i = 0; i < function->function.count; i++) {
            environment->data[i] = arguments[i];
        };
        return evaluate(function->function.body, environment);
    } else if (function->type == BuiltinType) {
        return function->builtin.body(arguments);
    } else return new_error(ApplicationError);
};

// builtins

Value * builtin_quote(Value * * arguments, Environment * environment) {
    return arguments[0];
};
Value * builtin_handle(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->error);
};
Value * builtin_if(Value * * arguments, Environment * environment) {
    if (arguments[0]->boolean) {
        return evaluate(arguments[1], environment);
    } else {
        return evaluate(arguments[2], environment);
    }
};
Value * builtin_mapcar(Value * * arguments, Environment * environment) {

};
Value * builtin_reduce(Value * * arguments, Environment * environment) {
    // evaluate end segments of the list in order
};
Value * builtin_collect(Value * * arguments, Environment * environment) {

};
Value * builtin_boolean_not(Value * * arguments, Environment * environment) {
    return new_boolean(!arguments[0]->boolean);
};
Value * builtin_boolean_and(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->boolean && arguments[1]->boolean);
};
Value * builtin_boolean_or(Value * * arguments, Environment * environment) {
    return arguments[0]->boolean || arguments[1]->boolean;
};
Value * builtin_boolean_xor(Value * * arguments, Environment * environment) {
    return arguments[0]->boolean ^ arguments[1]->boolean;
};
Value * builtin_integer_and(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_or(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_xor(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_not(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_add(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_subtract(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_multiply(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_divide(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_modulo(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_shift_left(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_shift_right(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_pow(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_equal(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_inequal(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_less(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_greater(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_less_equal(Value * * arguments, Environment * environment) {

};
Value * builtin_integer_greater_equal(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_add(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_subtract(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_multiply(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_divide(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_equal(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_inequal(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_less(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_greater(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_less_equal(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_greater_equal(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_sin(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_cos(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_tan(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_asin(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_acos(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_atan(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_csc(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_sec(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_cot(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_acsc(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_asec(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_acot(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_sinh(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_cosh(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_tanh(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_asinh(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_acosh(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_atanh(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_csch(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_sech(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_coth(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_acsch(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_asech(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_acoth(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_atan2(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_hypotenuse(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_floor(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_ceil(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_pow(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_gamma(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_integer2floating(Value * * arguments, Environment * environment) {

};
Value * builtin_floating_string2floating(Value * * arguments, Environment * environment) {

};
Value * builtin_string_join(Value * * arguments, Environment * environment) {

};
Value * builtin_string_split(Value * * arguments, Environment * environment) {

};
Value * builtin_string_tokenise(Value * * arguments, Environment * environment) {

};
Value * builtin_string_count(Value * * arguments, Environment * environment) {

};
Value * builtin_string_upper(Value * * arguments, Environment * environment) {

};
Value * builtin_string_lower(Value * * arguments, Environment * environment) {

};
Value * builtin_string_find(Value * * arguments, Environment * environment) {

};
Value * builtin_string_substring(Value * * arguments, Environment * environment) {

};
Value * builtin_string_set(Value * * arguments, Environment * environment) {

};
Value * builtin_string_isdigits(Value * * arguments, Environment * environment) {

};
Value * builtin_list_get(Value * * arguments, Environment * environment) {

};
Value * builtin_list_set(Value * * arguments, Environment * environment) {

};
Value * builtin_list_length(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};
Value * builtin_(Value * * arguments, Environment * environment) {

};