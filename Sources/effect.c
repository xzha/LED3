#include "effect.h"       

//EFFECT: LOADING ANIMATION
void effect_startup_animation(void)
{
  int k;
  int x, y, z;

  fill(0x00);

  for(z = 0; z < 8; z++)
  {
    for(y = 0; y < 8; y++)
    {
      for(x = 0; x < 8; x++)
      {
        turnOn(x, y, z);
        delay_ms(5);
      }
    }
  }

  delay_ms(400);

  for(z = 0; z < 8; z++)
  {
    for(y = 0; y < 8; y++)
    {
      for(x = 0; x < 8; x++)
      {
        turnOff(x, y, z);
        delay_ms(5);
      }
    }
  }

  delay_ms(500);

  for(k = 0; k < 3; k++)
  {
    fill(0xFF);
    delay_ms(200);
    fill(0x00);
    delay_ms(200);
  }
}


//EFFECT: LEAVE TRAIL OF LEDS BEHIND
void effect_holupp(char axis, int direction, int delay)
{
  int ii, jj, k;
  int start = 0;
  int end = 8;

  int fakek = 0;
  fill(0x00);

  if(axis == 'x')
  {
    if(direction == 1)
    {
      draw_planeX(0);
    }
    else if(direction == -1)
    {
      draw_planeX(7);
    }
  }
  else if(axis == 'y')
  {
    if(direction == 1)
    {
      draw_planeY(0);
    }
    else if(direction == -1)
    {
      draw_planeY(7);
    }
  }
  else if(axis == 'z')
  {
    if(direction == 1)
    {
      draw_planeZ(0);
    }
    else if(direction == -1)
    {
      draw_planeZ(7);
    }
  }

  delay_ms(delay);
  fill_mask();
  carve_mask(3);

  for(fakek = start; fakek < end - 1; fakek++)
  {
    if(direction == -1)
    {
      k = 7 - fakek; 
    }
    else
    {
      k = fakek; 
    }
    for(ii = 0; ii < 8; ii++)
    {
      for(jj = 0; jj < 8; jj++)
      {
        if(shift_mask[ii][jj] == 1)
        {
          if(axis == 'x')
          {
            moveX(k, ii, jj, direction);
          }
          else if(axis == 'y')
          {
            moveY(ii, k, jj, direction);
          }
          else if(axis == 'z')
          {
            moveZ(ii, jj, k, direction);
          }
        }
      }
    }

    carve_mask(ii);
    delay_ms(18);
  }  

  delay_ms(delay);

  if(axis == 'x')
  {
    effect_sweepX(direction, delay/25); 
  }
  else if(axis == 'z')
  {
    effect_sweepZ(direction, delay/25);
  }
  else if(axis == 'y')
  {
    effect_sweepY(direction, delay/25);
  }
}


//EFFECT: MOVE LEDS BETWEEN TWO PLATES
void effect_capacitor(int delay)
{
  int randx, randy, rand_time;
  int x, y, z;
  int k;

  int l;

  for(k = 0; k < 32; k++)
  {
    l = 63 - k;

    while(l)
    {
      randx = rand() % 8;
      randy = rand() % 8;

      if(!getState(randx, randy, 7))
      {
        turnOn(randx, randy, 7);
        break;
      }

      l--;
    }
  }

  for(x = 0; x < 8; x++)
  {
    for(y = 0; y < 8; y++)
    {
      if(!getState(x, y, 7))
      {
        turnOn(x, y, 0);
      }
    }
  }

  while(effect_count == 1)
  {
    randx = rand() % 8;
    randy = rand() % 8;

    if(getState(randx, randy, 0))
    {
      for(z = 0; z < 8; z++)
      {
        moveZ(randx, randy, z, 1);
        rand_time = rand() % 4;
        delay_ms(delay);
      }
    }
    else
    {
      for(z = 0; z < 8; z++)
      {
        moveZ(randx,randy, 7 - z, -1);
        rand_time = rand() % 7;
        delay_ms(delay);
      }
    }

    rand_time = rand() % 40;
    delay_ms(rand_time);
  }
}    


//EFFECT: COLLECT EVERYTHING AND MOVE X
void effect_sweepX(int direction, int delay)
{  
  int sweepi, sweepj, sweepk;

  if(direction == 1)
  {
    for(sweepk = 0; sweepk < 7; sweepk++)
    {
      for(sweepi = 0; sweepi < 8; sweepi++)
      {
        for(sweepj = 0; sweepj < 8; sweepj++)
        {
          if(getState(sweepk, sweepi, sweepj))
          {
            turnOff(sweepk, sweepi, sweepj);
            turnOn(sweepk + 1, sweepi, sweepj);
          }
        }
      }

      delay_ms(delay);
    }
  }
  else if(direction == -1)
  {
    for(sweepk = 7; sweepk > 0; sweepk--)
    {
      for(sweepi = 0; sweepi < 8; sweepi++)
      {
        for(sweepj = 0; sweepj < 8; sweepj++)
        {
          if(getState(sweepk, sweepi, sweepj))
          {
            turnOff(sweepk, sweepi, sweepj);
            turnOn(sweepk - 1, sweepi, sweepj);
          }
        }
      }

      delay_ms(delay);
    }
  }
}


//EFFECT: COLLECT EVERYTHING AND MOVE Y
void effect_sweepY(int direction, int delay) 
{     
  int sweepi, sweepj, sweepk;

  if(direction == 1)
  {
    for(sweepk = 0; sweepk < 7; sweepk++)
    {
      for(sweepi = 0; sweepi < 8; sweepi++)
      {
        for(sweepj = 0; sweepj < 8; sweepj++)
        {
          if(getState(sweepi, sweepk, sweepj))
          {
            turnOff(sweepi, sweepk, sweepj);
            turnOn(sweepi, sweepk + 1, sweepj);
          }
        }
      }

      delay_ms(delay);
    }
  }
  else if(direction == -1)
  {
    for(sweepk = 7; sweepk > 0; sweepk--)
    {
      for(sweepi = 0; sweepi < 8; sweepi++)
      {
        for(sweepj = 0; sweepj < 8; sweepj++)
        {
          if(getState(sweepi, sweepk, sweepj))
          {
            turnOff(sweepi, sweepk, sweepj);
            turnOn(sweepi, sweepk - 1, sweepj);
          }
        }
      }

      delay_ms(delay);
    }
  }
}


//EFFECT: COLLECT EVERYTHING AND MOVE Z
void effect_sweepZ(int direction, int delay)
{
  int sweepi, sweepj, sweepk;

  if(direction == 1)
  {
    for(sweepk = 0; sweepk < 7; sweepk++)
    {
      for(sweepi = 0; sweepi < 8; sweepi++)
      {
        for(sweepj = 0; sweepj < 8; sweepj++)
        {
          if(getState(sweepi, sweepj, sweepk))
          {
            turnOff(sweepi, sweepj, sweepk);
            turnOn(sweepi, sweepj, sweepk + 1);
          }
        }
      }

      delay_ms(delay);
    }
  }
  else if(direction == -1)
  {
    for(sweepk = 7; sweepk > 0; sweepk--)
    {
      for(sweepi = 0; sweepi < 8; sweepi++)
      {
        for(sweepj = 0; sweepj < 8; sweepj++)
        {
          if(getState(sweepi, sweepj, sweepk))
          {
            turnOff(sweepi, sweepj, sweepk);
            turnOn(sweepi, sweepj, sweepk - 1);
          }
        }
      }

      delay_ms(delay);
    }
  }
}


//EFFECT: RAIN
void effect_rain(int delay)
{
  int i_rain;
  int rnd_x, rnd_y, rnd_num;

  rnd_num = rand() % 4;

  for(i_rain = 0; i_rain < rnd_num; i_rain++)
  {
    rnd_x = rand() % 8;
    rnd_y = rand() % 8;
    turnOn(rnd_x, rnd_y, 7);
  }

  delay_ms(delay);
  shiftCubeNoWrapAround('z', -1);
}


//EFFECT: GROW SHRINK CUBE FROM CORNER
void effect_box_shrink_grow(int iterations, int rot, int flip, int delay)
{
  int x, k, xyz;

  for(x = 0; x < iterations && effect_count == 2; x++)
  {
    for(k = 0; k < 16 && effect_count == 2; k++)
    {
      xyz = 7-k; // This reverses counter i between 0 and 7.

      if(k > 7)
      {
        xyz = k - 8; // at i > 7, i 8-15 becomes xyz 0-7.
      }

      fill(0x00); 
      delay_ms(1);
      DisableInterrupts;

      draw_lineBox(0, xyz, 0, xyz, 0, xyz);

      if(flip > 0)
      {
        mirror_z();
      }

      if(rot == 1 || rot == 3)
      {
        mirror_y();
      }

      if(rot == 2 || rot == 3) 
      {
        mirror_x();
      }

      EnableInterrupts;      
      delay_ms(delay);
      fill(0x00);
    }
  }
}


//EFFECT: GROW SHRINK CUBE FROM CENTER OF CUBE
void effect_box_center(int delay, int grow)
{
  int k, kk;

  fill(0x00);

  for(k = 0; k < 4 && effect_count == 3; k++)
  {
    kk = k;

    if(grow > 0)
    {
      kk = 3 - k;
    }

    draw_lineBox(4 + kk, 3 - kk, 4 + kk, 3 - kk, 4 + kk, 3 - kk);
    delay_ms(delay);
    fill(0x00);
  }
}


void effect_pathmove(unsigned char *path, int length)
{
  int k, z;
  unsigned char state;

  for(k = (length - 1); k >= 1; k--)
  {
    for(z = 0; z < 8; z++)
    {
      state = getState(((path[(k - 1)] >> 4) & 0x0f), (path[(k - 1)] & 0x0f), z);
      alterState(((path[k] >> 4) & 0x0f), (path[k] & 0x0f), z, state);
    }
  }

  for(k = 0; k < 8; k++)
  {
    turnOff(((path[0] >> 4) & 0x0f), (path[0] & 0x0f),k);
  }
}


//EFFECT: SCROLLING TEXT
void effect_path_text (int delay, char *str)
{
  unsigned char path[28];
  unsigned char chr[5]; 
  unsigned char stripe;
  int z, k, ii;

  z = 4;
  font_getpath(0, path, 28);

  while(*str && effect_count == 4)
  {		
    font_getchar(*str++, chr);

    for(ii = 0; ii < 5 && effect_count == 4; ii++)
    {
      stripe = chr[ii];

      for(z = 0; z < 8 && effect_count == 4; z++)
      {
        if((stripe>>(7 - z)) & 0x01)
        {
          turnOn(0, 7, z);
        }
        else
        {
          turnOff(0, 7, z);
        }
      }

      effect_pathmove(path, 28);
      delay_ms(delay);
    }

    effect_pathmove(path, 28);
    delay_ms(delay);
  }

  for(k = 0; k < 28 && effect_count == 4; k++)
  {
    effect_pathmove(path, 28);
    delay_ms(delay);
  }
}
