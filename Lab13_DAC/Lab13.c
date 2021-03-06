// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);
int main(void){ // Real Lab13 
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
// PortE used for piano keys, PortB used for DAC        
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
  EnableInterrupts();  // enable after all initialization are done
  while(1){                
// input from keys to select tone
		switch (Piano_In())
		{
			case 0:
				Sound_Tone(4777); // C note //1/(32*523.251)/12.5/10^-9 (32 is number of samples)
				break;
			case 1:
				Sound_Tone(4256); // D note //1/(32*587.330)/12.5/10^-9 (32 is number of samples)
				break;
			case 2:
				Sound_Tone(3792); // E note //1/(32*659.255)/12.5/10^-9 (32 is number of samples)
				break;
			case 3:
				Sound_Tone(3188); // G note //1/(32*783.991)/12.5/10^-9 (32 is number of samples)
				break;
			default:
				Sound_Off(); // no button pressed
				break;
		}
		delay(10);
  }
            
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


