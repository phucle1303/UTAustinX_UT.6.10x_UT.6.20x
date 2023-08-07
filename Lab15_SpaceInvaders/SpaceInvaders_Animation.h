// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#ifndef _SPACEINVADERS_SPRITE_H_
#define _SPACEINVADERS_SPRITE_H_

#define NUM_SPRITE 5

#define BUNKER_DEAD 0
#define BUNKER_HEAVY_DAMAGED 1
#define BUNKER_MODERATE_DAMAGED 2
#define BUNKER_UNDAMAGED 3

extern bunker_t playerBunker;
extern playerShip_t playerShip;
extern STyp Enemy[NUM_SPRITE];

void SpaceInvaders_Sprite_Init(void);
void SpaceInvaders_Sprite_Move(void);
void SpaceInvaders_Sprite_Draw(void);

void SpaceInvaders_PlayerShip_Init(void);
void SpaceInvaders_PlayerShip_Move(unsigned long Slidepot_Distance);
void SpaceInvaders_PlayerShip_Draw(void);

void SpaceInvaders_Bunker_Init(void);
void SpaceInvaders_Bunker_Defense(void);
void SpaceInvaders_Bunker_Draw(unsigned char bunkerLife);

#endif /* _SPACEINVADERS_SPRITE_H_ */
