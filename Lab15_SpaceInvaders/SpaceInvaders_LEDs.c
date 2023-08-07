// Switches module for Space Invaders
// LED on PB4 - Fire
// LED on PB5 - Cool down ready

#include "tm4c123gh6pm.h"
#include "SpaceInvaders_LEDs.h"

/* LEDs init */
void SpaceInvaders_LEDs_PortB_Init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000002;     // 1) Port B clock
    delay = SYSCTL_RCGC2_R;           // delay
    // 2) no need to unlock port B
    GPIO_PORTB_AMSEL_R &= ~0x30;      // 3) disable analog function on PB4 and PB5
    GPIO_PORTB_PCTL_R &= ~0x00FF0000; // 4) GPIO clear bit PCTL on PB4 and PB5
    GPIO_PORTB_DIR_R |= 0x30;         // 5) PB4, PB5 output
    GPIO_PORTB_AFSEL_R &= ~0x30;      // 6) no alternate function on PB4 and PB5
    GPIO_PORTB_DEN_R |= 0x30;         // 7) enable digital pins on PB4 and PB5
}

/* Turn on LED PB4 */
void SpaceInvaders_LED_Fire_On(void)
{
    GPIO_PORTB_DATA_R |= 0x10;
}
/* Turn off LED PB4 */
void SpaceInvaders_LED_Fire_Off(void)
{
    GPIO_PORTB_DATA_R &= ~0x10;
}

/* Turn on LED PB5 */
void SpaceInvaders_LED_CoolDown_On(void)
{
    GPIO_PORTB_DATA_R |= 0x20;
}
/* Turn off LED PB5 */
void SpaceInvaders_LED_CoolDown_Off(void)
{
    GPIO_PORTB_DATA_R &= ~0x20;
}
