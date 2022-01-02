// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))

#define goW    			  0
#define waitW         1
#define goS           2
#define waitS         3
#define walk          4
#define noWalk1       5
#define walkLightOff1 6
#define noWalk2       7
#define walkLightOff2 8

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// Linked data structure
struct State 
{
	unsigned long OutWalk; // 2-bit pattern to output walk light
  unsigned long Out;  // 6-bit pattern to road lights
  unsigned long Time; // delay in 10ms units
  unsigned long Next[8]; // next state for inputs 0,1,2,3,4,5,6,7
}; 
typedef const struct State STyp;
	
STyp FSM[9]=
{
 {0x02,0x0C, 9,{goW,goW,waitW,waitW,waitW,goW,waitW,waitW}},
 {0x02,0x14, 5,{goS,goS,goS,goS,walk,walk,walk,goS}},
 {0x02,0x21, 9,{goS,waitS,goS,waitS,waitS,waitS,goS,waitS}},
 {0x02,0x22, 5,{goW,goW,goW,goW,walk,walk,walk,walk}},
 {0x08,0x24, 9,{walk,noWalk1,noWalk1,noWalk1,noWalk1,noWalk1,noWalk1,noWalk1}},
 {0x02,0x24, 5,{noWalk1,walkLightOff1,walkLightOff1,noWalk1,walkLightOff1,walkLightOff1,walkLightOff1,walkLightOff1}},
 {0x00,0x24, 5,{walkLightOff1,noWalk2,noWalk2,noWalk2,noWalk2,noWalk2,noWalk2,noWalk2}},
 {0x02,0x24, 5,{noWalk2,walkLightOff2,walkLightOff2,noWalk2,walkLightOff2,walkLightOff2,walkLightOff2,walkLightOff2}},
 {0x00,0x24, 5,{walkLightOff2,goW,goS,goW,walk,goW,goS,goW}}
};
unsigned long S;  // index to the current state
unsigned long Input;

// ***** 3. Subroutines Section *****

void SysTick_Init(void)
{
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}

// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay)
{
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}

// 8000000*12.5ns equals 100ms
void SysTick_Wait100ms(unsigned long delay)
{
  unsigned long i;
  for(i=0; i<delay; i++)
	{
    SysTick_Wait(8000000);  // wait 100ms
  }
}

void PortE_Init(void)
{ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x10;      			  // 1) E clock
  delay = SYSCTL_RCGC2_R;             // delay to allow clock to stabilize     
  GPIO_PORTE_AMSEL_R &= ~0x07;        // 2) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;     // 3) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R &= ~0x07;          // 4) PE0, PE1, PE2 input
  GPIO_PORTE_AFSEL_R &= ~0x07;        // 5) no alternate function    
  GPIO_PORTE_DEN_R |= 0x07;           // 6) enable digital pins PE2-PE0
}

void PortB_Init(void)
{ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x02;      			  // 1) B clock
  delay = SYSCTL_RCGC2_R;             // delay to allow clock to stabilize     
  GPIO_PORTB_AMSEL_R &= ~0x3F;        // 2) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;     // 3) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R |= 0x3F;           // 4) PB5-PB0 output  
  GPIO_PORTB_AFSEL_R &= ~0x3F;        // 5) no alternate function    
  GPIO_PORTB_DEN_R |= 0x3F;           // 6) enable digital pins PB5-PB0
}

void PortF_Init(void)
{ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x20;      			  // 1) F clock
  delay = SYSCTL_RCGC2_R;             // delay to allow clock to stabilize     
  GPIO_PORTF_AMSEL_R &= ~0x0A;        // 2) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;     // 3) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R |= 0x0A;           // 4) PF1, PF3 output
  GPIO_PORTF_AFSEL_R &= ~0x0A;        // 5) no alternate function    
  GPIO_PORTF_DEN_R |= 0x0A;           // 6) enable digital pins PE2-PE0
}

int main(void)
{ 
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
	SysTick_Init();
	PortE_Init();
	PortB_Init();
	PortF_Init();
  S=goW;
  EnableInterrupts();
  while(1)
	{
    GPIO_PORTB_DATA_R = FSM[S].Out;
    GPIO_PORTF_DATA_R = FSM[S].OutWalk;
		SysTick_Wait100ms(FSM[S].Time);
		Input = GPIO_PORTE_DATA_R&0x07;
		S = FSM[S].Next[Input];
  }
}




