#include "display.h"

static byte display[DISP_BYTES_H][DISP_BYTES_W] = {0};

static byte bit_table[8] =
{
  0x1,
  0x2,
  0x4,
  0x8,
  0x10,
  0x20,
  0x40,
  0x80
};

static int display_is_bit_set(byte byte, int index);
static void display_check_bit_index(int index);
static void display_check_pixel_index(int x, int y);

void display_draw(void)
{
  const int width = HOFFSET;
  const int height = VOFFSET;
  int posx, posy, i, j, k;
  int white;

  BeginDrawing();
  ClearBackground(BLACK);

  posx = posy = 0;
  for (i = 0; i < DISP_BYTES_H; i++)
  {
    for (j = 0; j < DISP_BYTES_W; j++)
    {
      for (k = 0; k < sizeof(byte); k++)
      {
	white = display_is_bit_set(display[i][j], k);
	DrawRectangle(posx, posy, width, height, white ? WHITE : BLACK);

	if (DEBUG_MODE)
	  DrawRectangleLines(posx, posy, width, height, GRAY);
	
	posx += HOFFSET;
      }
    }
    posx = 0;
    posy += VOFFSET;
  }
  
  EndDrawing();
}

void display_set_bit(int x, int y, int val)
{
  int indexh, indexv;
  int bit_index;

  display_check_pixel_index(x, y);

  indexh = x / sizeof(byte);
  indexv = y / sizeof(byte);
  bit_index = x - (indexh * sizeof(byte));

  if (val)
    display[indexv][indexh] |= bit_table[bit_index];
  else
    display[indexv][indexh] &= ~bit_table[bit_index];
}

void display_draw_sprite(ptr start, byte n, byte x, byte y,
			 memory_s* memory)
{
  
}

void display_clear(void)
{
  memset(display, 0, sizeof(display));
}

static int display_is_bit_set(byte val, int index)
{
  display_check_bit_index(index);
  return val & bit_table[index];
}

static void display_check_bit_index(int index)
{
  if (index < 0 || index > 7)
    error("Invalid bit index.", 1);
}

static void display_check_pixel_index(int x, int y)
{
  if (x < 0 || x > C8_SCR_WIDTH || y < 0 || y > C8_SCR_HEIGHT)
    error("Invalid display index.", 1);
}
