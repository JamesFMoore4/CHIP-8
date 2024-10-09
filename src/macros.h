#ifndef _MACROS_H
#define _MACROS_H

#include <inttypes.h>

#define SCR_WIDTH 1280
#define SCR_HEIGHT 640
#define C8_SCR_WIDTH 64
#define C8_SCR_HEIGHT 32
#define HOFFSET (SCR_WIDTH / C8_SCR_WIDTH)
#define VOFFSET (SCR_HEIGHT / C8_SCR_HEIGHT)
#define DISP_BYTES_W (C8_SCR_WIDTH / sizeof(uint8_t))
#define DISP_BYTES_H (C8_SCR_HEIGHT / sizeof(uint8_t))

#define MAX_RAM 4096
#define PROGRAM_START_ADDR 0x200
#define MAX_STACK_SIZE 16
#define NUM_REGISTERS 16

#define DEBUG_MODE 1

typedef uint8_t byte;
typedef uint16_t ptr;
typedef uint16_t instr;

#endif
