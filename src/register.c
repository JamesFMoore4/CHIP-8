#include "register.h"

typedef struct rfile_s
{
  uint8_t registers[NUM_REGISTERS];
  uint16_t pc;
  uint16_t index;
} rfile_s;

rfile_s* rfile_init(void)
{
  rfile_s* rfile;

  rfile = Calloc(1, sizeof(*rfile));
  rfile->pc = (uint16_t) PROGRAM_START_ADDR;

  return rfile;
}

void rfile_free(rfile_s* rfile)
{
  free(rfile);
}

uint8_t rfile_read(rfile_s* rfile, int x)
{
  assert(x >= 0x0 && x <= 0xF && "Invalid register index.");
  return rfile->registers[x];
}

void rfile_write(rfile_s* rfile, uint8_t val, int x)
{
  assert(x >= 0x0 && x <= 0xF && "Invalid register index.");
  rfile->registers[x] = val;
}

uint16_t rfile_pc_read(rfile_s* rfile)
{
  return rfile->pc;
}

void rfile_pc_write(rfile_s* rfile, uint16_t val)
{
  assert(!(val & 0xF000) && "Attempted to write invalid address to program counter.");
  rfile->pc = val;
}

uint16_t rfile_index_read(rfile_s* rfile)
{
  return rfile->index;
}

void rfile_index_write(rfile_s* rfile, uint16_t val)
{
  assert(!(val & 0xF000) && "Attempted to write invalid address to index register.");
  rfile->index = val;
}
