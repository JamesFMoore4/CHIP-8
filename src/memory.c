#include "memory.h"

typedef struct memory_s
{
  byte* addrspace;
  ptr first_byte;
  ptr last_byte;
  ptr program;
} memory_s;

memory_s* memory_init()
{
  memory_s* memory;

  memory = Malloc(sizeof(*memory));
  memory->addrspace = Calloc(MAX_RAM, sizeof(byte));
  memory->first_byte = 0;
  memory->last_byte = MAX_RAM - 1;
  memory->program = (ptr) PROGRAM_START_ADDR;
  
  return memory;  
}

void memory_free(memory_s* memory)
{
  free(memory->addrspace);
  free(memory);
}

ptr memory_read_instruction(memory_s* memory, ptr addr);
byte memory_read_byte(memory_s* memory, ptr addr);

void memory_write(memory_s* memory, byte byte);
void memory_map_file(memory_s* memory, FILE* bin);
