#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "stack.h"
#include "macros.h"
#include "display.h"
#include "register.h"
#include "memory.h"
#include "timer.h"

int main(int argc, char** argv)
{
  FILE* file;
  stack_s* stack;
  rfile_s* rfile;
  memory_s* memory;
  timer_s* timer_delay;
  timer_s* timer_sound;

  if (argc != 2)
  {
    printf("usage: %s <CHIP-8 file>\n", argv[0]);
    exit(0);
  }

  if (!(file = fopen(argv[1], "rb")))
    error("error: file not found.", 1);

  stack = stack_init();
  rfile = rfile_init();
  memory = memory_init();
  timer_delay = timer_init(DELAY);
  timer_sound = timer_init(SOUND);
  
  memory_load_file(memory, file);
  fclose(file);
  
  InitWindow(SCR_WIDTH, SCR_HEIGHT, "CHIP-8");
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    // input()
    // timer()
    // fetch()
    // decode()
    // execute()
    display_draw();
  }
  
  CloseWindow();
  stack_free(stack);
  rfile_free(rfile);
  memory_free(memory);

  return 0;
}
