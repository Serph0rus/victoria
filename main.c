#include "victoria.h"
#include "builtins.h"
#include "main.h"
#include <stdio.h>
Value * read_display(long index, unsigned long offset, unsigned long length) {};
Value * write_display(long index, unsigned long offset, Value * value) {};
unsigned long display_pitch(long index) {};
unsigned long display_length(long index) {};
Value * read_console(long index, unsigned long offset, unsigned long length) {
    Value * value = new_bytes(0, length);
    fread(value->bytes.value, sizeof(char), length, stdin);
    return value;
};
Value * write_console(long index, unsigned long offset, Value * value) {
    return new_integer(fwrite(value->bytes.value, sizeof(char), value->bytes.value, stdout));
};
unsigned long console_pitch(long index) {};
Value * read_storage(long index, unsigned long offset, unsigned long length) {};
Value * write_storage(long index, unsigned long offset, Value * value) {};
int main(int argc, char * * argv) {
    Value * expression = new
}