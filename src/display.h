#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <assert.h>
#include <inttypes.h>
#include <raylib.h>

#include "macros.h"
#include "error.h"

void display_draw(void);
void display_set_bit(int x, int y, int val);
void display_clear(void);

#endif
