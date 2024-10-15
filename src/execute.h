#ifndef _EXECUTE_H
#define _EXECUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "macros.h"
#include "display.h"
#include "error.h"
#include "timer.h"
#include "memory.h"
#include "register.h"
#include "stack.h"

typedef struct instruction_s
{
  byte n1, n2, n3, n4; // nibbles 1, 2, 3, 4, most sig -> least
} instruction_s;

typedef struct object_s
{
  stack_s* stack;
  rfile_s* rfile;
  memory_s* memory;
  timer_s* timer_delay, *timer_sound;
} object_s;

typedef void (*execute_f)(instruction_s*, object_s*);

#endif
