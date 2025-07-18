#pragma once
Value * read_display(long index, unsigned long offset, unsigned long length);
Value * write_display(long index, unsigned long offset, Value * value);
unsigned long display_pitch(long index);
unsigned long display_length(long index);
Value * read_console(long index, unsigned long offset, unsigned long length);
Value * write_console(long index, unsigned long offset, Value * value);
Value * read_storage(long index, unsigned long offset, unsigned long length);
Value * write_storage(long index, unsigned long offset, Value * value);