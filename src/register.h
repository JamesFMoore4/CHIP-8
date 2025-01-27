#ifndef _REGISTER_H
#define _REGISTER_H

#include <assert.h>
#include <inttypes.h>

#include "error.h"
#include "macros.h"

typedef struct rfile_s rfile_s;

rfile_s* rfile_init(void);
void rfile_free(rfile_s* rfile);

byte rfile_read(rfile_s* rfile, int index);
void rfile_write(rfile_s* rfile, byte val, int index);

ptr rfile_pc_read(rfile_s* rfile);
void rfile_pc_write(rfile_s* rfile, ptr addr);

ptr rfile_index_read(rfile_s* rfile);
void rfile_index_write(rfile_s* rfile, ptr addr);

void rfile_print(rfile_s* rfile);

#endif
