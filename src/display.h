#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <assert.h>
#include <inttypes.h>
#include <raylib.h>

#include "macros.h"
#include "error.h"
#include "memory.h"
#include "register.h"

void display_draw(void);
void display_draw_sprite(ptr start, byte n, byte x, byte y,
			 memory_s* memory, rfile_s* rfile);
void display_clear(void);
void display_print_debug_info(void);

#endif
