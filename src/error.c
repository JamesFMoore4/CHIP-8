#include "error.h"

void sys_error(char* msg, int status)
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(status);
}

void error(char* msg, int status)
{
  fprintf(stderr, "%s\n", msg);
  exit(status);
}

void* Malloc(size_t size)
{
  void* mem;

  if (!(mem = malloc(size)))
    sys_error("malloc error", 1);
  return mem;
}

void* Calloc(size_t nmemb, size_t size)
{
  void* mem;

  if (!(mem = calloc(nmemb, size)))
    sys_error("calloc error", 1);
  return mem;
}

void check_addr(ptr addr, char* msg)
{
  if (addr & 0xF000)
    error(msg, 1);
}
