#ifndef Cube_effect_h
#define Cube_effect_h

#include "cube.h"
#include "base.h"
#include "font.h"

void effect_startup_animation(void);
void effect_rain(int delay);	
void effect_box_shrink_grow(int iterations, int rot, int flip, int delay);	
void effect_box_center(int delay, int grow);	
void effect_pathmove(unsigned char *path, int length); 
void effect_path_text(int delay, char *str);  

void effect_sweepX(int direction, int delay);  
void effect_sweepY(int direction, int delay);
void effect_sweepZ(int direction, int delay);
void effect_capacitor(int delay);
void effect_holupp(char axis, int direction, int delay);

#endif
