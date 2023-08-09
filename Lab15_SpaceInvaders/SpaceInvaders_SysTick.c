/* 30Hz SysTick ISR */

#include "tm4c123gh6pm.h"
#include "SpaceInvaders_SysTick.h"

// Initialize SysTick interrupts to trigger at 30 Hz, 33 ms
void SpaceInvaders_SysTick_Init(unsigned long period)
{                                  
    NVIC_ST_CTRL_R = 0;            // disable SysTick during setup
    NVIC_ST_RELOAD_R = period - 1; // reload value
    NVIC_ST_CURRENT_R = 0;         // any write to current clears it
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000; // priority 2
    NVIC_ST_CTRL_R = 0x07; // enable SysTick with core clock and interrupts
                           // finish all initializations and then enable interrupts
}
