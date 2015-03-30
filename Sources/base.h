#ifndef Cube_base_h
#define Cube_base_h

#include "cube.h"
#include <string.h>

void fill(unsigned char pattern);
void delay_ms(unsigned int x);
void turnOn(int x,int y, int z);
void turnOff(int x, int y, int z);
unsigned char inBound(int x, int y, int z);
void draw_planeZ(int z);
void clear_planeZ(int z);
void draw_planeY(int y);
void clear_planeY(int y);
void draw_planeX(int x);
void clear_planeX(int x);

unsigned char getState(int x, int y, int z);
void alterState(int x, int y, int z, int state);

void reorder(int * x1, int * x2);
unsigned char make_lineX(int x1, int x2);
void draw_filledBox(int x1, int x2, int y1, int y2, int z1, int z2);
void draw_emptyBox(int x1, int x2, int y1, int y2, int z1, int z2);
void draw_lineBox(int x1, int x2, int y1, int y2, int z1, int z2);

void mirror_x(void);
void mirror_y(void);
void mirror_z(void);

char flipbyte(char b);

void moveX(int x, int y, int z, int units);
void moveY(int x, int y, int z, int units);
void moveZ(int x, int y, int z, int units);
void carve_mask(int x);
void fill_mask(void);

void shiftCubeNoWrapAround(char axis, int units);

#endif
