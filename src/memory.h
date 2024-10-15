#ifndef _MEMORY_H
#define _MEMORY_H

#include <inttypes.h>

#include "error.h"
#include "macros.h"

typedef struct memory_s memory_s;

memory_s* memory_init();
void memory_free(memory_s* memory);

ptr memory_read_instruction(memory_s* memory, ptr addr);
byte memory_read_byte(memory_s* memory, ptr addr);

void memory_write(memory_s* memory, ptr addr, byte val);
void memory_load_file(memory_s* memory, FILE* bin);

#endif
