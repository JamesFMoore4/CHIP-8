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
  memory->program = PROGRAM_START_ADDR;

  // Initialize font here
  
  return memory;  
}

void memory_free(memory_s* memory)
{
  free(memory->addrspace);
  free(memory);
}

instr memory_read_instruction(memory_s* memory, ptr addr)
{
  instr* fetch;
  
  check_addr(addr, "error: attempted to read from invalid address.");
  if (addr % 2)
    error("error: invalid instruction address.", 1);

  fetch = (instr*) &memory->addrspace[addr];
  
  return *fetch;
}

byte memory_read_byte(memory_s* memory, ptr addr)
{
  check_addr(addr, "error: attempted to read from invalid address.");

  return memory->addrspace[addr];
}

void memory_write(memory_s* memory, ptr addr, byte val)
{
  check_addr(addr, "error: attempted to write to invalid address.");

  memory->addrspace[addr] = val;
}

void memory_map_file(memory_s* memory, FILE* bin)
{
  byte val;
  ptr addr;

  addr = memory->program;
  while ((val = getc(bin)) != EOF && addr < MAX_RAM)
    memory->addrspace[addr++] = val;
}
