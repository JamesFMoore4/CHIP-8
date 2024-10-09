#ifndef _ERROR_H
#define _ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void sys_error(char* msg, int status);
void error(char* msg, int status);
void* Malloc(size_t size);
void* Calloc(size_t nmemb, size_t size);

#endif
