#include "base.h"       

// Fill the cube with a given pattern.
// fill(0x00); clears the cube
// fill(0xff); lights all leds

void fill(unsigned char pattern)
{
  int z, y;

  for (z = 0; z < 8; z++)
  {
    for (y = 0; y < 8; y++)
    {
      cube[z][y] = pattern;
    }
  }
}


// Delay function used in graphical effects.
void delay_ms(unsigned int x)
{
  delay_count = 0;

  while(delay_count < x);

  delay_count = -1;
}


// Turn on specific LED
void turnOn(int x, int y, int z)
{
  if(inBound(x, y, z))
  {
    cube[z][y] |= (1 << x); 
  }
}


// Turn off specific LED
void turnOff(int x, int y, int z)
{
  if(inBound(x, y, z))
  {
    cube[z][y] &= ~(1 << x);
  }
}


// Get the state of an LED
unsigned char getState(int x, int y, int z)
{
  if(inBound(x, y, z))
  { 
    return((cube[z][y] >> x) & 0x01);
  }

  return(0);
}


// Set LED to given state
void alterState(int x, int y, int z, int state)
{
  if (state == 1)
  {
    turnOn(x, y, z);
  }
  else
  {
    turnOff(x, y, z);
  }
}


// Check if given coordinates are in bound
unsigned char inBound(int x, int y, int z)
{
  if(x < 8 && x >= 0 && y < 8 && y >= 0 && z < 8 && z >= 0)
  {
    return(1);
  }

  return(0);
}


void draw_planeZ(int z)
{
  int y;

  if(z < 8 && z >= 0)
  {
    for(y = 0; y < 8; y ++)
    {
      cube[z][y] = 0xFF;
    }
  } 
}


void clear_planeZ(int z)
{
  int y;

  if(z < 8 && z >= 0)
  {
    for(y = 0; y < 8; y ++)
    {
      cube[z][y] = 0x00;
    }
  }
}


void draw_planeY(int y)
{
  int z;

  if(y < 8 && y >= 0)
  {
    for(z = 0; z < 8; z ++)
    {
      cube[z][y] = 0xFF;
    }
  }
}


void clear_planeY(int y)
{
  int z;

  if(y < 8 && y >= 0)
  {
    for(z = 0; z < 8; z ++)
    {
      cube[z][y] = 0x00;
    }
  }
}


void draw_planeX(int x)
{
  int y, z;

  if(x < 8 && x >= 0)
  {
    for(z = 0; z < 8; z ++)
    {
      for(y = 0; y < 8; y ++)
      {
        cube[z][y] |= 1 << x;
      }
    }
  } 
}


void clear_planeX(int x)
{
  int y, z;

  if(x < 8 && x >= 0)
  {
    for(z = 0; z < 8; z ++)
    {
      for(y = 0; y < 8; y ++)
      {
        cube[z][y] &= ~(1 << x);
      }
    }
  }
}


unsigned char make_lineX(int x1, int x2)
{
  unsigned char x;

  x = ((0xFF << x1) & ~(0xFF << (x2 + 1)));

  return(x);
}

void reorder(int * x1, int * x2)
{
  int temp;

  if(*x1 > *x2)
  {
    temp = *x1;
    *x1 = *x2;
    *x2 = temp;
  } 
}


void draw_filledBox(int x1, int x2, int y1, int y2, int z1, int z2)
{
  int x, y, z;      

  reorder(&x1, &x2);
  reorder(&y1, &y2);
  reorder(&z1, &z2);

  x = make_lineX(x1, x2);

  if(inBound(x1, y1, z1) && inBound(x2, y2, z2))
  {
    for(z = z1; z < (z2 + 1); z++)
    {
      for(y = y1; y < (y2 + 1); y++)
      {
        cube[z][y] |= x;
      }
    }
  }
}


void draw_emptyBox(int x1, int x2, int y1, int y2, int z1, int z2)
{
  int x, y, z;

  reorder(&x1, &x2);
  reorder(&y1, &y2);
  reorder(&z1, &z2);

  x = make_lineX(x1, x2);


  if(inBound(x1, y1, z1) && inBound(x2, y2, z2))
  {
    for(z = z1; z < (z2+1); z++)
    {
      for(y = y1; y < (y2+1); y++)
      {
        if((z == z1 || z == z2) || (y == y1 || y == y2))
        {
          cube[z][y] |= x;
        }
        else
        {
          cube[z][y] |= ((0x01 << x2) | (0x01 << x1));
        }
      }
    } 
  }
}


void draw_lineBox(int x1, int x2, int y1, int y2, int z1, int z2)
{
  int x, y, z;

  reorder(&x1, &x2);
  reorder(&y1, &y2);
  reorder(&z1, &z2);

  x = make_lineX(x1, x2);

  if(inBound(x1, y1, z1) && inBound(x2, y2, z2))
  {
    for(z = z1; z < (z2 + 1); z++)
    {
      for(y = y1; y < (y2 + 1); y++)
      {
        if((z == z1 || z == z2) && (y == y1 || y == y2))
        {
          cube[z][y] |= x;
        } 
        else if((z == z1 || z == z2) || (y == y1 || y == y2))
        {
          cube[z][y] |= (0x01 << x2) | (0x01 << x1);
        }
      }
    } 
  }
}


// Flip the cube 180 degrees along the y axis.
void mirror_y(void)
{
  unsigned char buffer[8][8];
  unsigned char x,y,z;

  memcpy(buffer, cube, 64); // copy the current cube into a buffer.

  fill(0x00);

  for(z = 0; z < 8; z++)
  {
    for(y = 0; y < 8; y++)
    {
      for(x = 0; x < 8; x++)
      {
        if(buffer[z][y] & (0x01 << x))
        {
          turnOn(x, (8 - 1 - y), z); 
        }
      }
    }
  }
}

// Flip the cube 180 degrees along the x axis
void mirror_x(void)
{
  unsigned char buffer[8][8];
  unsigned char y,z;

  memcpy(buffer, cube, 64); // copy the current cube into a buffer.

  fill(0x00);

  for(z = 0; z < 8; z++)
  {
    for(y = 0; y < 8; y++)
    {
      // This will break with different buffer sizes..
      cube[z][y] = flipbyte(buffer[z][y]);
    }
  }
}


// Flip the cube 180 degrees along the z axis
void mirror_z(void)
{
  unsigned char buffer[8][8];
  unsigned char z, y;

  memcpy(buffer, cube, 64); // copy the current cube into a buffer.

  for(y = 0; y < 8; y++)
  {
    for(z = 0; z < 8; z++)
    {
      cube[(8 - 1 - z)][y] = buffer[z][y];
    }
  }
}


char flipbyte (char b)
{
  char flop = 0x00;

  flop = (flop & 0b11111110) | (0b00000001 & (b >> 7));
  flop = (flop & 0b11111101) | (0b00000010 & (b >> 5));
  flop = (flop & 0b11111011) | (0b00000100 & (b >> 3));
  flop = (flop & 0b11110111) | (0b00001000 & (b >> 1));
  flop = (flop & 0b11101111) | (0b00010000 & (b << 1));
  flop = (flop & 0b11011111) | (0b00100000 & (b << 3));
  flop = (flop & 0b10111111) | (0b01000000 & (b << 5));
  flop = (flop & 0b01111111) | (0b10000000 & (b << 7));

  return(flop);
}

void shiftCubeNoWrapAround(char axis, int units)
{
  int shift_index = 7;
  unsigned char shift_temp[8];
  int shift_layer;
  int shift_row;

  if (axis == 'x')
  {
    if(units == 1)
    {
      for(shift_layer = 0; shift_layer < 8; shift_layer++)
      {
        for(shift_row = 0; shift_row < 8; shift_row++)
        {
          if(shift_mask[shift_layer][shift_row] == 1)
          {
            cube[shift_layer][shift_row] >>= 1;
            turnOff(7, shift_row, shift_layer);
          }
        }
      }
    }
    else if(units == -1)
    {
      for(shift_layer = 0; shift_layer < 8; shift_layer++)
      {
        for(shift_row = 0; shift_row < 8; shift_row++)
        {
          cube[shift_layer][shift_row] <<= 1;
          turnOff(1, shift_row, shift_layer);
        }
      }
    }
  }
  else if (axis == 'y')
  {
    if(units == 1)
    {
      for(shift_layer = 0; shift_layer < 8; shift_layer++)
      {
        shift_index = 7;

        while(shift_index > 0)
        {
          cube[shift_layer][(shift_index + units) % 8] = cube[shift_layer][shift_index];
          shift_index--;
        }

        cube[shift_layer][1] = 0;
      }
    }
    else if(units == -1)
    {
      units = 1;

      for(shift_layer = 0; shift_layer < 8; shift_layer++)
      {
        shift_index = 0;

        while(shift_index < 7)
        {
          cube[shift_layer][shift_index] = cube[shift_layer][(shift_index + units) % 8];
          shift_index++;
        }

        cube[shift_layer][7] = 0;
      }
    }
  }
  else if(axis == 'z')
  {
    if(units == 1)
    {
      while(shift_index > 0)
      {
        memcpy(cube[(shift_index + units) % 8], cube[shift_index], 8);
        shift_index--;
      }

      memcpy(cube[0], blank,8);
    }
    else if(units == -1)
    {
      shift_index = 0;
      units = 1;

      memcpy(shift_temp,cube[0],8);

      while(shift_index < 7)
      {
        memcpy(cube[shift_index], cube[(shift_index + units) % 8], 8);
        shift_index++;
      }

      memcpy(cube[7], blank, 8);
    }
  }
}


void moveX(int x, int y, int z, int units)
{
  int newx = x + units;

  if(newx >= 0 && newx < 8)
  {
    turnOff(x, y, z);
    turnOn(newx, y, z);
  }
}


void moveY(int x, int y, int z, int units)
{
  int newy = y + units;

  if(newy >= 0 && newy < 8)
  {
    turnOff(x, y, z);
    turnOn(x, newy, z);
  }
}


void moveZ(int x, int y, int z, int units)
{
  int newz = z + units;

  if(newz >= 0 && newz < 8)
  {
    turnOff(x, y, z);
    turnOn(x, y, newz);
  }
}


void fill_mask(void)
{
  int y, z;

  for(z = 0; z < 8; z++)
  {
    for(y = 0; y < 8; y++)
    {
      shift_mask[z][y] = 1;
    }
  }
}


void carve_mask(int x)
{
  int randx, randy;
  int rand_num = rand() % 10;

  while(rand_num >= 0)
  {
    randx = rand() % 9;
    randy = rand() % 9;

    if(shift_mask[randx][randy] == 1)
    {
      rand_num--;
      shift_mask[randx][randy] = 0;
    }
  }
}
