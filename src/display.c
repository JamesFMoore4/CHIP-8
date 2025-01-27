#include "display.h"

static byte display[C8_SCR_HEIGHT][C8_SCR_WIDTH] = {0};

static void display_convert_to_array(byte* dest, byte val);

void display_draw(void)
{
  const int width = HOFFSET;
  const int height = VOFFSET;
  int posx, posy, i, j, k;
  int white;

  BeginDrawing();
  ClearBackground(BLACK);

  posx = posy = 0;
  for (i = 0; i < C8_SCR_HEIGHT; i++)
  {
    for (j = 0; j < C8_SCR_WIDTH; j++)
    {
      DrawRectangle(posx, posy, width, height, display[i][j] ? WHITE : BLACK);

      if (DEBUG_MODE)
	DrawRectangleLines(posx, posy, width, height, GRAY);

      posx += HOFFSET;
    }
    posx = 0;
    posy += VOFFSET;
  }
  
  EndDrawing();
}

void display_draw_sprite(ptr start, byte n, byte x, byte y,
			 memory_s* memory, rfile_s* rfile)
{ 
  int i, j;
  byte arr[8], val, temp;
  
  x %= C8_SCR_WIDTH;
  y %= C8_SCR_HEIGHT;

  for (i = 0; i < n; i++)
  {
    val = memory_read_byte(memory, start + i);
    display_convert_to_array(arr, val);
    temp = x;
    
    for (j = 7; j >= 0; j--)
    {
      if (display[y][x] && arr[j])
	rfile_write(rfile, 1, 0xF);
      
      display[y][x] ^= arr[j];
      if (++x >= C8_SCR_WIDTH)
	break;
    }
    
    x = temp;
    if (++y >= C8_SCR_HEIGHT)
      break;
  }
}

void display_clear(void)
{
  memset(display, 0, sizeof(display));
}

void display_print_debug_info(void)
{
  int i, j;

  for (i = 0; i < C8_SCR_HEIGHT; i++)
  {
    for (j = 0; j < C8_SCR_WIDTH; j++)
    {
      printf("%d ", !!display[i][j]);
    }
    printf("\n");
  }
}

static void display_convert_to_array(byte* dest, byte val)
{
  int i;
  byte mask;

  for (i = 0, mask = 1; i < 8; i++, mask <<= 1)
    dest[i] = val & mask ? -1 : 0;
}
