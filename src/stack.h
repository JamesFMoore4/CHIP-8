#ifndef _STACK_H
#define _STACK_H

#include <inttypes.h>

#include "error.h"
#include "macros.h"

typedef struct stack_s stack_s;

stack_s* stack_init();
ptr stack_pop(stack_s* stack);
void stack_push(stack_s* stack, ptr addr);
void stack_free(stack_s* stack);

#endif
