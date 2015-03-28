/* */
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* includes */
#include <stdlib.h>

#include "cube.h"
#include "base.h"
#include "effect.h"

/* Functions Declarations */
void shiftout(char);
void setDelayConstant();

/* Variables Declarations */
// Clocking signals
int tencnt = 0;
int onecnt = 0;    

// Counters 
int i = 0;
int j = 0;

#define EFFECT_NUM 6
void effect_pick(void);
char old_effect_count = -2;

/* Output signals */

// Signals to shift out
unsigned char line1 = 0;
unsigned char line2 = 0;
unsigned char line3 = 0;
unsigned char line4 = 0;
unsigned char line5 = 0;
unsigned char line6 = 0;
unsigned char line7 = 0;
unsigned char line8 = 0;
unsigned char layer = 0x00;
unsigned char cur_layer = 0;


// Push buttons              
char leftpb = 0;  // left pushbutton flag
char rghtpb = 0;  // right pushbutton flag
char prevpb = 0;  // previous pushbutton state
char runstp = 0;  // run/stop flag         

// NEWLY ADDED
// Delay Multiplier
int DELAY_MULTIPLIER = 0;

/*          
 ***********************************************************************
 Initializations
 ***********************************************************************
 */

void  initializations(void) {

  /* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
  PLLCTL = PLLCTL | 0x40; //; turn on PLL
  SYNR = 0x02;            //; set PLL multiplier
  REFDV = 0;              //; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; //; engage PLL

  /* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40   ; //COP off; RTI and COP stopped in BDM-mode

  /* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
  SCIBDH =  0x00; //set baud rate to 9600
  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; //$9C = 156
  SCICR2 =  0x0C; //initialize SCI for program-driven operation
  DDRB   =  0x10; //set PB4 for output mode
  PORTB  =  0x10; //assert DTR pin on COM port

  /* Initialize peripherals */

  /*
     Initialize SPI for baud rate of 6 Mbs, MSB first
     (note that R/S, R/W', and LCD clk are on different PTT pins)
     */  
  SPIBR_SPR0 = 1; 
  SPIBR_SPR1 = 0; //baud rate pre-selection bits = 1
  SPIBR_SPR2 = 0;
  SPIBR_SPPR0 = 0;
  SPIBR_SPPR1 = 0;//baud rate selection bits = 0
  SPIBR_SPPR2 = 0; // (SPPR + 1) * 2^(SPR+1) = 4 = Baud rate divisor 

  SPICR1 = 0x50; //master mode, active high clock, data sampling at odd edges of SCK, 
  //data transferred most significant bit first.
  SPICR2 = 0x00; //non-bidrectional mode   

  /* Initialize interrupts */

  /* 
     Initialize TIM Ch 7 (TC7) for periodic interrupts every 10.0 ms  
     - Enable timer subsystem                         
     - Set channel 7 for output compare
     - Set appropriate pre-scale factor and enable counter reset after OC7
     - Set up channel 7 to generate 10 ms interrupt rate
     - Initially disable TIM Ch 7 interrupts                                 
     */         
  TSCR1_TEN = 1; //enable timer subsystem
  TIOS_IOS7 = 1; //set channel 7 for output compare
  TSCR2_PR0 = 1;
  TSCR2_PR1 = 1; //24Mhz / (2^4) = 1.5Mhz -> TCNT increments every 0.667uS
  TSCR2_PR2 = 1;
  TSCR2_TCRE = 1; //counter reset by successful output compare 7
  TC7 = 188; //0.667uS * 15000 = 1mS; interrupts 1000 times per second, 250fps
  TIE_C7I = 1; //enable TIM Ch 7 interrupts   


  /*
     Initialize the RTI for an 2.048 ms interrupt rate
     */                                                
  RTICTL = 0x1F; 
  CRGINT = 0x80; //enable interrrupt

  /* Initialize digital I/O port pins */  
  DDRT = 0xFF; //program port T for output mode
  DDRM = 0xFF; //program port M for output mode

  // ATD
  ATDCTL2 = 0x80;
  ATDCTL3 = 0x18;
  ATDCTL4 = 0x85;
  DDRAD = 0;
  ATDDIEN = 0xC0; //program PAD7 and PAD6 pins as digital inputs

  delay_count = -1;
  effect_count = -1;
}

/*                        
 ***********************************************************************
 Main
 ***********************************************************************
 */
void main(void) {
  DisableInterrupts;
  initializations();                      
  EnableInterrupts;
  effect_startup_animation();

  for(;;) 
  {
    /* < start of your main loop > */ 

    if (leftpb) {
      leftpb=0;
    }


    if (rghtpb) {
      rghtpb = 0;
    }

    effect_pick();

  }/* loop forever */

}   /* do not leave main */




/*
 ***********************************************************************                       
 RTI interrupt service routine: RTI_ISR
 ************************************************************************
 */

interrupt 7 void RTI_ISR(void)
{
  // clear RTI interrupt flag
  CRGFLG = CRGFLG | 0x80;    

  if(prevpb && !PORTAD0_PTAD6){
    prevpb = 0;
    rghtpb = 1;

    effect_count++;
    effect_count %= EFFECT_NUM;
    fill(0x00);

  } else if(prevpb && !PORTAD0_PTAD7) {
    prevpb = 0;
    leftpb = 1;

    effect_count = -1;
  } else {
    prevpb = PORTAD0_PTAD6 && PORTAD0_PTAD7;
  }

  // NEWLY ADDED
  if(effect_count >= 0)
  {
    setDelayConstant();
  }
}

/*
 ***********************************************************************                       
 TIM interrupt service routine       
 ***********************************************************************
 */

interrupt 15 void TIM_ISR(void)
{
  // clear TIM CH 7 interrupt flag 
  TFLG1 = TFLG1 | 0x80; 

  // Set up one sec
  tencnt++;
  if(tencnt == 100) {
    tencnt = 0;
    onecnt++;
    if(onecnt == 10) {
      onecnt = 0; 
      PTT_PTT1 = 1 - PTT_PTT1;
    }
  }

  if (delay_count != -1) {
    delay_count++;
  }

  layer = 0x00; // clear layer
  line1 = cube[cur_layer][0]; 
  line2 = cube[cur_layer][1];    
  line3 = cube[cur_layer][2];
  line4 = cube[cur_layer][3];
  line5 = cube[cur_layer][4];
  line6 = cube[cur_layer][5];
  line7 = cube[cur_layer][6];
  line8 = cube[cur_layer][7];  
  layer = layer | (0x01 << (cur_layer)); // shift to the right layer

  // paint by layer, shift out right rect, left rect, then layer (opposite of gal set up)
  shiftout(line8);
  shiftout(line7);
  shiftout(line6); 
  shiftout(line5);  
  shiftout(line4);
  shiftout(line3); 
  shiftout(line2);
  shiftout(line1);   
  shiftout(layer);     

  PTT_PTT4 = 1;
  PTT_PTT4 = 0;

  cur_layer ++;
  if (cur_layer == 8) {
    cur_layer = 0; // reset to bottom layer
  } 

}      

/*
 ***********************************************************************                       
 SCI interrupt service routine             
 ***********************************************************************
 */

interrupt 20 void SCI_ISR(void)
{


}

/*
 ***********************************************************************
shiftout: Transmits the character x to external shift 
register using the SPI.  It should shift MSB first.  
MISO = PM[4]
SCK  = PM[5]
 ***********************************************************************
 */

void shiftout(char x)
{
  // test the SPTEF bit: wait if 0; else, continue
  // write data x to SPI data register
  // wait for 30 cycles for SPI data to shift out 
  while (!SPISR_SPTEF);
  SPIDR = x;
  asm {
    nop //1
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop //15
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop //30
  }

  return;
} 


void effect_pick(void) {
  if (effect_count == 0) {
    effect_rain(10 * DELAY_MULTIPLIER);
  } else if (effect_count == 1) { 
    effect_capacitor(2 * DELAY_MULTIPLIER);
  } else if (effect_count == 2) { 
    for (i=0;i<8 && effect_count == 2;i++){
      effect_box_shrink_grow (1, i%4, i & 0x04, 5 * DELAY_MULTIPLIER);
    }
  } else if (effect_count == 3) {
    effect_box_center(5 * DELAY_MULTIPLIER,0);
    effect_box_center(5 * DELAY_MULTIPLIE0,1);
    effect_box_center(5 * DELAY_MULTIPLIE0,0);
    effect_box_center(5 * DELAY_MULTIPLIE0,1);
  } else if (effect_count == 4) {
    effect_path_text (12 * DELAY_MULTIPLIER, "MERRY XMAS");
  } else if (effect_count == 5) {
    if (effect_count == 5) {
      effect_holupp('z',1,50 * DELAY_MULTIPLIER);
      delay_ms(400);
    }
    if (effect_count == 5) {
      effect_holupp('z',-1,50 * DELAY_MULTIPLIER);
      delay_ms(400);
    }
    if (effect_count == 5) {
      effect_holupp('x',1,50 * DELAY_MULTIPLIER);
      delay_ms(400);
    }
    if (effect_count == 5) {
      effect_holupp('x',-1,50 * DELAY_MULTIPLIER);
      delay_ms(400);
    }
    if (effect_count == 5) {
      effect_holupp('y',1,50 * DELAY_MULTIPLIER);
      delay_ms(400);
    }
    if (effect_count == 5) {
      effect_holupp('y',-1,50 * DELAY_MULTIPLIER);
      delay_ms(400);
    }
  }
}


// NEWLY ADDED
void setDelayConstant()
{

  ATDCTL5 = 0x00;

  DELAY_MULTIPLIER = ATDDR0H / 5;

  if (DELAY_MULTIPLIER < 3)
  {
    DELAY_MULTIPLIER = 3;
  }
}
