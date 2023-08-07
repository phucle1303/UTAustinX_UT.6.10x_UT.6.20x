// Switches module for Space Invaders
// LED on PB4 - Fire
// LED on PB5 - Cool down ready

#ifndef _SPACEINVADERS_LEDS_H_
#define _SPACEINVADERS_LEDS_H_

/* LEDs init */
void SpaceInvaders_LEDs_PortB_Init(void);

/* Turn on LED PB4 */
void SpaceInvaders_LED_Fire_On(void);
/* Turn off LED PB4 */
void SpaceInvaders_LED_Fire_Off(void);

/* Turn on LED PB5 */
void SpaceInvaders_LED_CoolDown_On(void);
/* Turn off LED PB5 */
void SpaceInvaders_LED_CoolDown_Off(void);


#endif /* _SPACEINVADERS_LEDS_H_ */
