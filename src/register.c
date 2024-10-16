#include "register.h"

static void rfile_check_index(int index);

typedef struct rfile_s
{
  byte registers[NUM_REGISTERS];
  ptr pc;
  ptr index;
} rfile_s;

rfile_s* rfile_init(void)
{
  rfile_s* rfile;

  rfile = Calloc(1, sizeof(*rfile));
  rfile->pc = (ptr) PROGRAM_START_ADDR;

  return rfile;
}

void rfile_free(rfile_s* rfile)
{
  free(rfile);
}

byte rfile_read(rfile_s* rfile, int index)
{
  rfile_check_index(index);
  return rfile->registers[index];
}

void rfile_write(rfile_s* rfile, byte val, int index)
{
  rfile_check_index(index);
  rfile->registers[index] = val;
}

ptr rfile_pc_read(rfile_s* rfile)
{
  return rfile->pc;
}

void rfile_pc_write(rfile_s* rfile, ptr addr)
{
  check_addr(addr, "error: attempted to write invalid address to program counter.");
  rfile->pc = addr;
}

ptr rfile_index_read(rfile_s* rfile)
{
  return rfile->index;
}

void rfile_index_write(rfile_s* rfile, ptr addr)
{
  check_addr(addr, "error: attempted to write invalid address to index register.");
  rfile->index = addr;
}

void rfile_print(rfile_s* rfile)
{
  int i;

  for (i = 0; i <= 0xF; i++)
    printf("V%x: %d %x\n", i, rfile->registers[i], rfile->registers[i]);
  printf("PC: %d %x\n", rfile->pc, rfile->pc);
  printf("I: %d %x\n", rfile->index, rfile->index);
}

static void rfile_check_index(int index)
{
  if (index < 0 || index > NUM_REGISTERS - 1)
    error("error: invalid register index.", 1);
}
