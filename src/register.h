#ifndef _REGISTER_H
#define _REGISTER_H

#include <assert.h>
#include <inttypes.h>

#include "error.h"
#include "macros.h"

typedef struct rfile_s rfile_s;

rfile_s* rfile_init(void);
void rfile_free(rfile_s* rfile);

uint8_t rfile_read(rfile_s* rfile, int x);
void rfile_write(rfile_s* rfile, uint8_t val, int x);

uint16_t rfile_pc_read(rfile_s* rfile);
void rfile_pc_write(rfile_s* rfile, uint16_t val);

uint16_t rfile_index_read(rfile_s* rfile);
void rfile_index_write(rfile_s* rfile, uint16_t val);

#endif
