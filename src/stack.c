#include "stack.h"

typedef struct stack_s
{
  uint16_t buf[MAX_STACK_SIZE];
  int top, num_items;
} stack_s;

stack_s* stack_init()
{
  stack_s* stack;

  stack = Calloc(1, sizeof(*stack));
  stack->top = -1;
  
  return stack;
}

uint16_t stack_pop(stack_s* stack)
{
  uint16_t addr;
  
  if (!stack->num_items)
    error("error: stack underflow.\n", 1);

  addr = stack->buf[stack->top--];
  stack->num_items--;

  return addr;
}

uint16_t stack_peek(stack_s* stack)
{
  if (!stack->num_items)
    error("error: stack underflow.\n", 1);

  return stack->buf[stack->top];
}

void stack_push(stack_s* stack, uint16_t addr)
{
  if (stack->num_items == MAX_STACK_SIZE)
    error("error: stack overflow.\n", 1);

  stack->buf[++stack->top] = addr;
  stack->num_items++;
}

void stack_free(stack_s* stack)
{
  free(stack);
}