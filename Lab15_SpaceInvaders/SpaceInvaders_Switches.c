// Switches module for Space Invaders
// fire button connected to PE0
// special weapon fire button connected to PE1

#include "tm4c123gh6pm.h"
#include "SpaceInvaders_Switches.h"

/* Switches Init */
void SpaceInvaders_Switches_PortE_Init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000010;     // 1) Port E clock
    delay = SYSCTL_RCGC2_R;           // delay
    // 2) no need to unlock port E
    GPIO_PORTE_AMSEL_R &= ~0x03;      // 3) disable analog function on PE0 and PE1
    GPIO_PORTE_PCTL_R &= ~0x000000FF; // 4) GPIO clear bit PCTL on PE0 and PE1
    GPIO_PORTE_DIR_R &= ~0x03;        // 5) PE1,PE0 input
    GPIO_PORTE_AFSEL_R &= ~0x03;      // 6) no alternate function on PE0 and PE1
    GPIO_PORTE_PDR_R |= 0x03;         // enable pull-down resistors on PE0 and PE1
    GPIO_PORTE_DEN_R |= 0x03;         // 7) enable digital pins on PE0 and PE1
}

/* Read switch state PE0 (fire button) */
unsigned char SpaceInvaders_GetSwitchState_PE0(void)
{
    unsigned char returnVal = SWITCH_NOT_PRESSED;
    if (GPIO_PORTE_DATA_R&0x01)
    {
        returnVal = SWITCH_PRESSED;
    }
    return returnVal;
}

/* Read switch state PE1 (special weapon fire button) */
unsigned char SpaceInvaders_GetSwitchState_PE1(void)
{
    unsigned char returnVal = SWITCH_NOT_PRESSED;
    if (GPIO_PORTE_DATA_R&0x02)
    {
        returnVal = SWITCH_PRESSED;
    }
    return returnVal;
}
