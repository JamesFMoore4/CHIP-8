#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "stack.h"
#include "macros.h"
#include "display.h"

int main(int argc, char** argv)
{
  FILE* file;
  stack_s* stack;

  if (argc != 2)
  {
    printf("usage: %s <CHIP-8 file>\n", argv[0]);
    exit(0);
  }

  if (!(file = fopen(argv[1], "rb")))
    error("error: file not found.", 1);

  // read file contents into CHIP-8 RAM
  
  fclose(file);
  
  InitWindow(SCR_WIDTH, SCR_HEIGHT, "CHIP-8");
  SetTargetFPS(60);

  stack = stack_init();

  while (!WindowShouldClose())
  {
    display_draw();
  }
  
  CloseWindow();
  stack_free(stack);

  return 0;
}
