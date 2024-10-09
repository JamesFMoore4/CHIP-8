#ifndef _STACK_H
#define _STACK_H

#include <inttypes.h>

#include "error.h"
#include "macros.h"

typedef struct stack_s stack_s;

stack_s* stack_init();
uint16_t stack_pop(stack_s* stack);
uint16_t stack_peek(stack_s* stack);
void stack_push(stack_s* stack, uint16_t addr);
void stack_free(stack_s* stack);

#endif
