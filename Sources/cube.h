#ifndef Cube_cube_h
#define Cube_cube_h

extern int delay_count; 

// Cube buffer cube[z][y] -> stores x;
extern unsigned char cube[8][8];

/* Cube layout
  2D from front

     ______  ______
    |      ||      |
    |      ||      |
  Z |LINE 1||LINE 2| .... to LINE 8
    |      ||      |
    |      ||      |
    |______||______|

        Y

  2D from top (bit values)
    
     _____________
    |7   7   7 
    |6   6   6    
  X |5   5   5  
    |4   4   4    ........ 5 bytes
    |3   3   3    
    |2   2   2
    |1   1   1
    |0   0   0
    |_____________
        
        Y
*/

/* Axis layout
        Z
        |
        |    X
        |   / 
        |  /
        | /
________|/_________ Y
       /|
      / |
     /  |
    /   |
*/


extern unsigned char shift_mask[8][8];
extern unsigned char blank[8];
extern char effect_count;

#endif