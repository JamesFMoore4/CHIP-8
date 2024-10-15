#ifndef _TIMER_H
#define _TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <raylib.h>

#include "macros.h"

typedef struct timer_s timer_s;
typedef enum {DELAY, SOUND} type_e;
typedef void (*update_f)(timer_s*);

timer_s* timer_init(type_e type);
void timer_free(timer_s* timer);
void timer_set(timer_s* timer, byte val);
byte timer_get(timer_s* timer);
void timer_update(timer_s* timer);

#endif
