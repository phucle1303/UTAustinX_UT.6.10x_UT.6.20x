// Switches module for Space Invaders
// fire button connected to PE0
// special weapon fire button connected to PE1

#ifndef _SPACEINVADERS_SWITCHES_H_
#define _SPACEINVADERS_SWITCHES_H_

#define SWITCH_PRESSED      1
#define SWITCH_NOT_PRESSED  0

/* Swiches init */
void SpaceInvaders_Switches_PortE_Init(void);
/* Read switch state PE0 (fire button) */
unsigned char SpaceInvaders_GetSwitchState_PE0(void);
/* Read switch state PE1 (special weapon fire button) */
unsigned char SpaceInvaders_GetSwitchState_PE1(void);

#endif /* _SPACEINVADERS_SWITCHES_H_ */
