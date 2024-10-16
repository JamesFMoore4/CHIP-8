#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "error.h"
#include "stack.h"
#include "macros.h"
#include "display.h"
#include "register.h"
#include "memory.h"
#include "timer.h"
#include "execute.h"

extern execute_f exec[];

static void objects_init(object_s* objects, FILE* bin);
static void objects_free(object_s* objects);
static instr fetch(memory_s* memory, rfile_s* rfile);
static void decode(instruction_s* decoded_instr, instr instruction);
static void execute(instruction_s* decoded_instr, object_s* objects);

int main(int argc, char** argv)
{
  FILE* bin;
  instr instruction;
  instruction_s decoded_instr;
  object_s objects;

  if (argc != 2)
  {
    printf("usage: %s <CHIP-8 file>\n", argv[0]);
    exit(0);
  }

  if (!(bin = fopen(argv[1], "rb")))
    error("error: file not found.", 1);

  // Must be called before initialization of sound timer
  InitAudioDevice();
  objects_init(&objects, bin);
  
  fclose(bin);
  
  InitWindow(SCR_WIDTH, SCR_HEIGHT, "CHIP-8");
  SetTargetFPS(60);

  srand(time(NULL));
  
  while (!WindowShouldClose())
  {
    timer_update(objects.timer_sound);
    timer_update(objects.timer_delay);
    /* instruction = fetch(objects.memory, objects.rfile); */
    /* decode(&decoded_instr, instruction); */
    /* execute(&decoded_instr, &objects); */
    display_draw();
  }
  
  objects_free(&objects);

  CloseAudioDevice();
  CloseWindow();

  return 0;
}

static void objects_init(object_s* objects, FILE* bin)
{
  objects->memory = memory_init();
  objects->stack = stack_init();
  objects->rfile = rfile_init();
  objects->timer_delay = timer_init(DELAY);
  objects->timer_sound = timer_init(SOUND);
  memory_load_file(objects->memory, bin);
}

static void objects_free(object_s* objects)
{
  stack_free(objects->stack);
  rfile_free(objects->rfile);
  memory_free(objects->memory);
  timer_free(objects->timer_delay);
  timer_free(objects->timer_sound);
}

static instr fetch(memory_s* memory, rfile_s* rfile)
{
  instr instruction;
  ptr addr;

  addr = rfile_pc_read(rfile);
  instruction = memory_read_instruction(memory, addr);
  rfile_pc_write(rfile, addr + sizeof(addr));
  return instruction;
}

static void decode(instruction_s* decoded_instr, instr instruction)
{
  byte b1, b2;

  b1 = *((byte*)&instruction + 1);
  b2 = (byte)instruction;

  // These must be logical shifts, or bugs will occur
  decoded_instr->original = instruction;
  decoded_instr->n1 = (b1 & 0xF0) >> 4;
  decoded_instr->n2 = b1 & 0xF;
  decoded_instr->n3 = (b2 & 0xF0) >> 4;
  decoded_instr->n4 = b2 & 0xF;
}

static void execute(instruction_s* decoded_instr, object_s* objects)
{
  assert(decoded_instr->n1 >= 0x0 && decoded_instr->n1 <= 0xF && "Invalid instruction code.");
  exec[decoded_instr->n1](decoded_instr, objects);
}
