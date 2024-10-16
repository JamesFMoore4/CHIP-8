#include "memory.h"

// Font data for characters 0-F, each on a separate row
static byte font_data[] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0,
  0x20, 0x60, 0x20, 0x20, 0x70,
  0xF0, 0x10, 0xF0, 0x80, 0xF0, 
  0xF0, 0x10, 0xF0, 0x10, 0xF0, 
  0x90, 0x90, 0xF0, 0x10, 0x10, 
  0xF0, 0x80, 0xF0, 0x10, 0xF0, 
  0xF0, 0x80, 0xF0, 0x90, 0xF0, 
  0xF0, 0x10, 0x20, 0x40, 0x40, 
  0xF0, 0x90, 0xF0, 0x90, 0xF0, 
  0xF0, 0x90, 0xF0, 0x10, 0xF0, 
  0xF0, 0x90, 0xF0, 0x90, 0x90, 
  0xE0, 0x90, 0xE0, 0x90, 0xE0, 
  0xF0, 0x80, 0x80, 0x80, 0xF0, 
  0xE0, 0x90, 0x90, 0x90, 0xE0, 
  0xF0, 0x80, 0xF0, 0x80, 0xF0, 
  0xF0, 0x80, 0xF0, 0x80, 0x80  
};

typedef struct memory_s
{
  byte* addrspace;
  ptr first_byte;
  ptr last_byte;
  ptr program_start;
  ptr font_start;
} memory_s;

static int system_is_little_endian(void);
static void swap_bytes(instr* instruction);

memory_s* memory_init()
{
  memory_s* memory;

  memory = Malloc(sizeof(*memory));
  memory->addrspace = Calloc(MAX_RAM, sizeof(byte));
  memory->first_byte = 0;
  memory->last_byte = MAX_RAM - 1;
  memory->program_start = PROGRAM_START_ADDR;
  memory->font_start = FONT_START_ADDR;
  memcpy(memory->addrspace + memory->font_start, font_data, sizeof(font_data)); // load font data
  
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

  fetch = (instr*) &memory->addrspace[addr]; // Fetch two bytes instead of one

  // Make sure that bytes are in proper order
  if (system_is_little_endian())
    swap_bytes(fetch);
  
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

void memory_load_file(memory_s* memory, FILE* bin)
{
  byte val;
  ptr addr;

  addr = memory->program_start;
  while ((char)(val = getc(bin)) != EOF && addr <= memory->last_byte)
    memory->addrspace[addr++] = val;

  if (addr > memory->last_byte)
    error("error: input file too large.", 1);
}

static int system_is_little_endian(void)
{
  int test;

  test = 1;
  return *(char*)&test;
}

static void swap_bytes(instr* instruction)
{
  byte b1, b2;

  b1 = (byte) *instruction;
  b2 = *((byte*)instruction + 1);
  *instruction = (instr) b2;
  *((byte*)instruction + 1) = b1;
}
