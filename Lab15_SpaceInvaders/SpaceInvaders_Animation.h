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

#define NUM_SPRITE              5

#define SPRITES_WIDTH           16
#define SPRITES_HEIGHT          10

#define BUNKER_DEAD             0
#define BUNKER_HEAVY_DAMAGED    1
#define BUNKER_MODERATE_DAMAGED 2
#define BUNKER_UNDAMAGED        3
#define BUNKER_WIDTH            18
#define BUNKER_HEIGHT           5

#define PLAYERSHIP_WIDTH        18
#define PLAYERSHIP_HEIGHT       8

#define SHIPMISSILE_WIDTH       4
#define SHIPMISSILE_HEIGHT      9

#define NOKIA_HEIGHT            48-1
#define NOKIA_WIDTH             84-1

typedef struct 
{
    unsigned char x;      // x coordinate
    unsigned char y;      // y coordinate
    const unsigned char *image[2]; // ptr->image
    unsigned char life;            // 0=dead, 1=alive
}Enemy_t;          

typedef struct 
{
    unsigned char x;      // x coordinate
    unsigned char y;      // y coordinate
    const unsigned char *image[4]; // ptr->image
    unsigned char life;            // 0=dead, 1=heavy damaged, 2=moderate damaged, 3=undamaged
} bunker_t;   

typedef struct
{
    unsigned char x;      // x coordinate
    unsigned char y;      // y coordinate
    const unsigned char *image[2];
    unsigned char hit; //0=no hit, 1=hit
}missile_t;


void SpaceInvaders_Sprite_Init(void);
void SpaceInvaders_Sprite_Move(void);
void SpaceInvaders_Sprite_DrawAlive(void);
void SpaceInvaders_Sprite_DrawDead(void);
unsigned char SpaceInvaders_Sprite_GetX(Enemy_t Enemy);
unsigned char SpaceInvaders_Sprite_GetY(void);
unsigned char SpaceInvaders_Sprite_GetLife(Enemy_t);
// void SpaceInvaders_Sprite_Kill(Enemy_t);

void SpaceInvaders_PlayerShip_Init(void);
void SpaceInvaders_PlayerShip_Move(unsigned long Slidepot_Distance);
void SpaceInvaders_PlayerShip_Draw(void);
unsigned char SpaceInvaders_PlayerShip_GetX(void);
unsigned char SpaceInvaders_PlayerShip_GetY(void);

void SpaceInvaders_Bunker_Init(void);
void SpaceInvaders_Bunker_Defense(void);
void SpaceInvaders_Bunker_Draw(unsigned char bunkerLife);
unsigned char SpaceInvaders_Bunker_GetY(void);

void SpaceInvaders_ShipMissile_Init(void);
void SpaceInvaders_ShipMissile_SetPosMissile(unsigned char playerShipX, unsigned char playerShipY);
void SpaceInvaders_ShipMissile_Move(void);
void SpaceInvaders_ShipMissile_Draw(void);
unsigned char SpaceInvaders_ShipMissile_GetX(void);
unsigned char SpaceInvaders_ShipMissile_GetY(void);
unsigned char SpaceInvaders_ShipMissile_Sprites_Hit(void);

#endif /* _SPACEINVADERS_SPRITE_H_ */
