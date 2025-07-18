#include "victoria.h"
#include "math.h"
Value * builtin_handle(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->error);
};
Value * builtin_if(Value * * arguments, Environment * environment) {
    if (arguments[0]->boolean) {
        return evaluate(arguments[1], environment);
    } else {
        return evaluate(arguments[2], environment);
    };
};
Value * builtin_define(Value * * arguments, Environment * environment) {
    return environment_set(environment, arguments[0]->symbol, arguments[1]);
};
Value * builtin_typeof(Value * * arguments, Environment * environment) {
    if (arguments[0]->type == BuiltinStreamType) return StreamType;
    else return arguments[0]->type;
};
Value * builtin_collect(Value * * arguments, Environment * environment) {
    collect();
};
Value * builtin_boolean_not(Value * * arguments, Environment * environment) {
    return new_boolean(!arguments[0]->boolean);
};
Value * builtin_boolean_and(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->boolean && arguments[1]->boolean);
};
Value * builtin_boolean_or(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->boolean || arguments[1]->boolean);
};
Value * builtin_boolean_xor(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->boolean ^ arguments[1]->boolean);
};
Value * builtin_integer_and(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer & arguments[1]->integer);
};
Value * builtin_integer_or(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer | arguments[1]->integer);
};
Value * builtin_integer_xor(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer ^ arguments[1]->integer);
};
Value * builtin_integer_not(Value * * arguments, Environment * environment) {
    return new_integer(~arguments[0]->integer);
};
Value * builtin_integer_add(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer + arguments[1]->integer);
};
Value * builtin_integer_subtract(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer - arguments[1]->integer);
};
Value * builtin_integer_multiply(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer * arguments[1]->integer);
};
Value * builtin_integer_divide(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer / arguments[1]->integer);
};
Value * builtin_integer_modulo(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer % arguments[1]->integer);
};
Value * builtin_integer_shift_left(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer << arguments[1]->integer);
};
Value * builtin_integer_shift_right(Value * * arguments, Environment * environment) {
    return new_integer(arguments[0]->integer >> arguments[1]->integer);
};
Value * builtin_integer_pow(Value * * arguments, Environment * environment) {
    int result = 1;
    if (arguments[1]->integer < 0) {
        return new_error(ArithmeticError);
    } else for (int i = 0; i < arguments[1]->integer; i++) {
        result *= arguments[0]->integer;
    };
    return new_integer(result);
};
Value * builtin_integer_equal(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->integer == arguments[1]->integer);
};
Value * builtin_integer_inequal(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->integer != arguments[1]->integer);
};
Value * builtin_integer_less(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->integer < arguments[1]->integer);
};
Value * builtin_integer_greater(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->integer > arguments[1]->integer);
};
Value * builtin_integer_less_equal(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->integer <= arguments[1]->integer);
};
Value * builtin_integer_greater_equal(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->integer >= arguments[1]->integer);
};
Value * builtin_floating_add(Value * * arguments, Environment * environment) {
    return new_floating(arguments[0]->floating + arguments[1]->floating);
};
Value * builtin_floating_subtract(Value * * arguments, Environment * environment) {
    return new_floating(arguments[0]->floating - arguments[1]->floating);
};
Value * builtin_floating_multiply(Value * * arguments, Environment * environment) {
    return new_floating(arguments[0]->floating * arguments[1]->floating);
};
Value * builtin_floating_divide(Value * * arguments, Environment * environment) {
    return new_floating(arguments[0]->floating / arguments[1]->floating);
};
Value * builtin_floating_equal(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->floating == arguments[1]->floating);
};
Value * builtin_floating_inequal(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->floating != arguments[1]->floating);
};
Value * builtin_floating_less(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->floating < arguments[1]->floating);
};
Value * builtin_floating_greater(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->floating > arguments[1]->floating);
};
Value * builtin_floating_less_equal(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->floating <= arguments[1]->floating);
};
Value * builtin_floating_greater_equal(Value * * arguments, Environment * environment) {
    return new_boolean(arguments[0]->floating >= arguments[1]->floating);
};
Value * builtin_floating_sin(Value * * arguments, Environment * environment) {
    return new_floating(sin(arguments[0]->floating));
};
Value * builtin_floating_cos(Value * * arguments, Environment * environment) {
    return new_floating(cos(arguments[0]->floating));
};
Value * builtin_floating_tan(Value * * arguments, Environment * environment) {
    return new_floating(tan(arguments[0]->floating));
};
Value * builtin_floating_asin(Value * * arguments, Environment * environment) {
    return new_floating(asin(arguments[0]->floating));
};
Value * builtin_floating_acos(Value * * arguments, Environment * environment) {
    return new_floating(acos(arguments[0]->floating));
};
Value * builtin_floating_atan(Value * * arguments, Environment * environment) {
    return new_floating(atan(arguments[0]->floating));
};
Value * builtin_floating_csc(Value * * arguments, Environment * environment) {
    return new_floating(1 / sin(arguments[0]->floating));
};
Value * builtin_floating_sec(Value * * arguments, Environment * environment) {
    return new_floating(1 / cos(arguments[0]->floating));
};
Value * builtin_floating_cot(Value * * arguments, Environment * environment) {
    return new_floating(1 / tan(arguments[0]->floating));
};
Value * builtin_floating_acsc(Value * * arguments, Environment * environment) {
    return new_floating(asin(1 / arguments[0]->floating));
};
Value * builtin_floating_asec(Value * * arguments, Environment * environment) {
    return new_floating(acos(1 / arguments[0]->floating));
};
Value * builtin_floating_acot(Value * * arguments, Environment * environment) {
    return new_floating(atan(1 / arguments[0]->floating));
};
Value * builtin_floating_sinh(Value * * arguments, Environment * environment) {
    return new_floating(sinh(arguments[0]->floating));
};
Value * builtin_floating_cosh(Value * * arguments, Environment * environment) {
    return new_floating(cosh(arguments[0]->floating));
};
Value * builtin_floating_tanh(Value * * arguments, Environment * environment) {
    return new_floating(tanh(arguments[0]->floating));
};
Value * builtin_floating_asinh(Value * * arguments, Environment * environment) {
    return new_floating(asinh(arguments[0]->floating));
};
Value * builtin_floating_acosh(Value * * arguments, Environment * environment) {
    return new_floating(acosh(arguments[0]->floating));
};
Value * builtin_floating_atanh(Value * * arguments, Environment * environment) {
    return new_floating(atanh(arguments[0]->floating));
};
Value * builtin_floating_csch(Value * * arguments, Environment * environment) {
    return new_floating(1 / sinh(arguments[0]->floating));
};
Value * builtin_floating_sech(Value * * arguments, Environment * environment) {
    return new_floating(1 / cosh(arguments[0]->floating));
};
Value * builtin_floating_coth(Value * * arguments, Environment * environment) {
    return new_floating(1 / tanh(arguments[0]->floating));
};
Value * builtin_floating_acsch(Value * * arguments, Environment * environment) {
    return new_floating(asinh(1 / arguments[0]->floating));
};
Value * builtin_floating_asech(Value * * arguments, Environment * environment) {
    return new_floating(acosh(1 / arguments[0]->floating));
};
Value * builtin_floating_acoth(Value * * arguments, Environment * environment) {
    return new_floating(atanh(1 / arguments[0]->floating));
};
Value * builtin_floating_atan2(Value * * arguments, Environment * environment) {
    return new_floating(atan2(arguments[0]->floating, arguments[1]->floating));
};
Value * builtin_floating_hypotenuse(Value * * arguments, Environment * environment) {
    return new_floating(hypot(arguments[0]->floating, arguments[1]->floating));
};
Value * builtin_floating_floor(Value * * arguments, Environment * environment) {
    return new_floating(floor(1 / arguments[0]->floating));
};
Value * builtin_floating_ceil(Value * * arguments, Environment * environment) {
    return new_floating(ceil(1 / arguments[0]->floating));
};
Value * builtin_floating_pow(Value * * arguments, Environment * environment) {
    return new_floating(pow(arguments[0]->floating, arguments[1]->floating));
};
Value * builtin_floating_gamma(Value * * arguments, Environment * environment) {
    return new_floating(tgamma(1 / arguments[0]->floating));
};
Value * builtin_floating_integer2floating(Value * * arguments, Environment * environment) {
    return new_floating(arguments[0]->integer);
};
Value * builtin_floating_string2floating(Value * * arguments, Environment * environment) {
    char * end_pointer = 0;
    double converted = strtod(arguments[0]->string.value, end_pointer);
    if (end_pointer == arguments[0]->string.value + arguments[0]->string.length) return new_floating(converted);
    else return new_error(ParseError);
};
Value * builtin_string_join(Value * * arguments, Environment * environment) {
    Value * list = arguments[0];
    char * result = strdup("");
    while (list && list->type == PairType) {
        char * new_result;
        asprintf(& new_result, "%s%s", result, list->pair.left->string.value);
        free(result);
        result = new_result;
        list = list->pair.right;
    };
    Value * value_result = new_string(result);
    free(result);
    return value_result;
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
    Value * list = arguments[0];
    long index = arguments[1]->integer;
    while (index-- > 0 && list->type == PairType) {
        list = list->pair.right;
    };
    if (list->type != PairType || index >= 0) return new_error(ManipulationError);
    return list->pair.left;
};
Value * builtin_list_set(Value * * arguments, Environment * environment) {

};
Value * builtin_list_length(Value * * arguments, Environment * environment) {
    Value * list = arguments[0];
    long count = 0;
    while (list->type == PairType) {
        count++;
        list = list->pair.right;
    };
    return new_integer(count);
};
Value * builtin_stream(Value * * arguments, Environment * environment) {
    return new_stream(arguments[0], arguments[1]);
};
Value * builtin_stream_read(Value * * arguments, Environment * environment) {
    if (arguments[0]->stream.is_builtin) return arguments[0]->stream.builtin.read(arguments[0]->stream.builtin.index, arguments[1]->integer, arguments[2]->integer);
    else evaluate(new_pair(arguments[0]))
};
Value * builtin_stream_write(Value * * arguments, Environment * environment) {
    if (arguments[0]->stream.is_builtin) return arguments[0]->stream.builtin.write(arguments[0]->stream.builtin.index, arguments[1]->integer, arguments[2]);
    else // send a message and return a result
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