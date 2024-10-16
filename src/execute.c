#include "execute.h"

// Instructions 0-F
static void z(instruction_s*, object_s*);
static void o(instruction_s*, object_s*);
static void tw(instruction_s*, object_s*);
static void th(instruction_s*, object_s*);
static void fo(instruction_s*, object_s*);
static void fi(instruction_s*, object_s*);
static void si(instruction_s*, object_s*);
static void se(instruction_s*, object_s*);
static void e(instruction_s*, object_s*);
static void n(instruction_s*, object_s*);
static void A(instruction_s*, object_s*);
static void B(instruction_s*, object_s*);
static void C(instruction_s*, object_s*);
static void D(instruction_s*, object_s*);
static void E(instruction_s*, object_s*);
static void F(instruction_s*, object_s*);

static void invalid_instruction(instr instruction);
static int key_map_index(int key_press);

execute_f exec[] =
{
  z, o, tw, th, fo, fi, si, se,
  e, n, A, B, C, D, E, F
};

static int keypad_map[] =
{
  KEY_X, KEY_ONE, KEY_TWO, KEY_THREE,
  KEY_Q, KEY_W, KEY_E, KEY_A,
  KEY_S, KEY_D, KEY_Z, KEY_C,
  KEY_FOUR, KEY_R, KEY_F, KEY_V
};

static void z(instruction_s* inst, object_s* o)
{
  switch(inst->original)
  {
  case 0x00E0: // CLS
    display_clear();
    break;
  case 0x00EE: // RET
    rfile_pc_write(o->rfile, stack_pop(o->stack));
    break;
  default:
    invalid_instruction(inst->original);
  }  
}

static void o(instruction_s* inst, object_s* o)
{
  ptr addr;

  addr = inst->original & ~0xF000;

  if (!addr || addr & 0xF000 || addr % 2)
    error("error: attempted to jump to invalid address.", 1);

  rfile_pc_write(o->rfile, addr); // JP addr
}

static void tw(instruction_s* inst, object_s* o)
{
  ptr addr;

  addr = inst->original & 0x0FFF;

  if (!addr)
    invalid_instruction(inst->original);

  stack_push(o->stack, rfile_pc_read(o->rfile)); // CALL addr
  rfile_pc_write(o->rfile, addr);
}

static void th(instruction_s* inst, object_s* o)
{
  byte val;

  val = inst->original & 0x00FF;

  if (rfile_read(o->rfile, inst->n2) == val) 
    rfile_pc_write(o->rfile, rfile_pc_read(o->rfile) + sizeof(ptr)); // SE Vx, byte
}

static void fo(instruction_s* inst, object_s* o)
{
  byte val;

  val = inst->original & 0x00FF;

  if (rfile_read(o->rfile, inst->n2) != val)
    rfile_pc_write(o->rfile, rfile_pc_read(o->rfile) + sizeof(ptr)); // SNE Vx, byte
}

static void fi(instruction_s* inst, object_s* o)
{
  if (inst->n4)
    invalid_instruction(inst->original);

  if (rfile_read(o->rfile, inst->n2) == rfile_read(o->rfile, inst->n3))
    rfile_pc_write(o->rfile, rfile_pc_read(o->rfile) + sizeof(ptr)); // SE Vx, Vy
}

static void si(instruction_s* inst, object_s* o)
{
  byte val;

  val = inst->original & 0x00FF;
  rfile_write(o->rfile, val, inst->n2); // LD Vx, byte
}

static void se(instruction_s* inst, object_s* o)
{
  byte val;

  val = inst->original & 0x00FF;
  rfile_write(o->rfile, rfile_read(o->rfile, inst->n2) + val, inst->n2); // ADD Vx, byte
}

static void e(instruction_s* inst, object_s* o)
{
  byte vx, vy;

  vx = rfile_read(o->rfile, inst->n2);
  vy = rfile_read(o->rfile, inst->n3);
  
  switch(inst->n4)
  {
  case 0x0: // LD Vx, Vy
    vx = vy;
    break;
  case 0x1: // OR Vx, Vy
    vx |= vy;
    break;
  case 0x2: // AND Vx, Vy
    vx &= vy;
    break;
  case 0x3: // XOR Vx, Vy
    vx ^= vy;
    break;
  case 0x4: // ADD Vx, Vy
    vx += vy;
    rfile_write(o->rfile, vx < vy ? 1 : 0, 0xF); // Check for overflow, set carry flag
    break;
  case 0x5: // SUB Vx, Vy
    rfile_write(o->rfile, vx > vy ? 1 : 0, 0xF); // Check for underflow, set borrow flag
    vx -= vy;
    break;
  case 0x6: // SHR Vx {, Vy}
    rfile_write(o->rfile, vx & 0x1 ? 1 : 0, 0xF);
    vx >>= 1;
    break;
  case 0x7: // SUBN Vx, Vy
    rfile_write(o->rfile, vy > vx ? 1 : 0, 0xF); // Check for underflow, set borrow flag
    vx = vy - vx;
    break;
  case 0xE: // SHL Vx {, Vy}
    rfile_write(o->rfile, vx & 0x80 ? 1 : 0, 0xF);
    vx <<= 1;
    break;
  default:
    invalid_instruction(inst->original);
  }

  rfile_write(o->rfile, vx, inst->n2);
}

static void n(instruction_s* inst, object_s* o)
{
  if (inst->n4)
    invalid_instruction(inst->original);

  if (rfile_read(o->rfile, inst->n2) != rfile_read(o->rfile, inst->n3))
    rfile_pc_write(o->rfile, rfile_pc_read(o->rfile) + sizeof(ptr)); // SNE Vx, Vy
}

static void A(instruction_s* inst, object_s* o)
{
  ptr addr;

  addr = inst->original & 0x0FFF;
  rfile_index_write(o->rfile, addr); // LD I, addr
}

static void B(instruction_s* inst, object_s* o)
{
  ptr addr;

  addr = inst->original & 0x0FFF;
  addr += rfile_read(o->rfile, 0x0);

  if (!addr || addr & 0xF000 || addr % 2)
    error("error: attempted to jump to invalid address.", 1);

  rfile_pc_write(o->rfile, addr); // JP V0, addr
}

static void C(instruction_s* inst, object_s* o)
{
  byte random, val;

  random = (byte)(rand() % 256);
  val = inst->original & 0x00FF;

  rfile_write(o->rfile, random & val, inst->n2); // RND Vx, byte
}

static void D(instruction_s* inst, object_s* o)
{
  display_draw_sprite(rfile_index_read(o->rfile), inst->n4, rfile_read(o->rfile, inst->n2),
		      rfile_read(o->rfile, inst->n3), o->memory);
}

static void E(instruction_s* inst, object_s* o)
{
  byte vx;

  vx = rfile_read(o->rfile, inst->n2);
  if (vx < 0x0 || vx > 0xF)
    error("error: invalid key.", 1);
  
  switch (inst->original & 0x00FF)
  {
  case 0x9E: // SKP Vx
    if (IsKeyDown(keypad_map[vx]))
      rfile_pc_write(o->rfile, rfile_pc_read(o->rfile) + sizeof(ptr));
    break;
  case 0xA1: // SKNP Vx
    if (IsKeyUp(keypad_map[vx]))
      rfile_pc_write(o->rfile, rfile_pc_read(o->rfile) + sizeof(ptr));
    break;
  default:
    invalid_instruction(inst->original);
  }
}

static void F(instruction_s* inst, object_s* o)
{
  int key_press, index, offset, vx, i, val;
  const int font_width = 5;
  
  switch(inst->original & 0x00FF)
  {
  case 0x07: // LD Vx, DT
    rfile_write(o->rfile, timer_get(o->timer_delay), inst->n2);
    break;
  case 0x0A: // LD Vx, K
    while (!(key_press = GetKeyPressed()) || (index = key_map_index(key_press)) < 0);
    rfile_write(o->rfile, (byte) index, inst->n2);
    break;
  case 0x15: // LD DT, Vx
    timer_set(o->timer_delay, rfile_read(o->rfile, inst->n2));
    break;
  case 0x18: // LD ST, Vx
    timer_set(o->timer_sound, rfile_read(o->rfile, inst->n2));
    break;
  case 0x1E: // ADD I, Vx
    rfile_index_write(o->rfile, rfile_index_read(o->rfile) + rfile_read(o->rfile, inst->n2));
    break;
  case 0x29: // LD F, Vx
    rfile_index_write(o->rfile, FONT_START_ADDR + inst->n2 * font_width);
    break;
  case 0x33: // LD B, Vx
    index = rfile_index_read(o->rfile);
    vx = rfile_read(o->rfile, inst->n2);
    offset = 2;
    while (offset >= 0)
    {
      memory_write(o->memory, index + offset, vx % 10);
      offset--;
      vx /= 10;
    }
    break;
  case 0x55: // LD [I], Vx
    index = rfile_index_read(o->rfile);
    for (i = 0; i <= inst->n2; i++)
    {
      vx = rfile_read(o->rfile, i);
      memory_write(o->memory, index + i, vx);
    }
    break;
  case 0x65: // LD Vx, [I]
    index = rfile_index_read(o->rfile);
    for (i = 0; i <= inst->n2; i++)
    {
      val = memory_read_byte(o->memory, index + i);
      rfile_write(o->rfile, val, i);
    }
    break;
  default:
    invalid_instruction(inst->original);
  }
}

static void invalid_instruction(instr instruction)
{
  fprintf(stderr, "error: invalid instruction '%.2x'.\n",
	  instruction);
  exit(1);
}

static int key_map_index(int key_press)
{
  int i, size;

  size = sizeof(keypad_map) / sizeof(int);
  for (i = 0; i < size; i++)
    if (keypad_map[i] == key_press)
      return i;

  return -1;
}
