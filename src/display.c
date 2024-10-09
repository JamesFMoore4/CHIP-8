#include "display.h"

static uint8_t display[DISP_BYTES_H][DISP_BYTES_W] = {0};

static uint8_t bit_table[8] =
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

static int display_is_bit_set(uint8_t byte, int index);

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
      for (k = 0; k < sizeof(uint8_t); k++)
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

  assert(x >= 0 && x <= C8_SCR_WIDTH &&
	 "Invalid display x coordinate.");
  assert(y >= 0 && y <= C8_SCR_HEIGHT &&
	 "Invalid display y coordinate.");

  indexh = x / sizeof(uint8_t);
  indexv = y / sizeof(uint8_t);
  bit_index = x - (indexh * sizeof(uint8_t));

  if (val)
    display[indexv][indexh] |= bit_table[bit_index];
  else
    display[indexv][indexh] &= ~bit_table[bit_index];
}

static int display_is_bit_set(uint8_t byte, int index)
{
  assert(index >= 0 && index <= 7 && "Invalid bit index.");
  return byte & bit_table[index];
}
