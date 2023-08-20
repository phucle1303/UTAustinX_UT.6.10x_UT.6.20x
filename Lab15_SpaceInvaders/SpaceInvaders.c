// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

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

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "SpaceInvaders_Timer2A.h"
#include "SpaceInvaders_LEDs.h"
#include "SpaceInvaders_Switches.h"
#include "SpaceInvaders_SysTick.h"
#include "ADC.h"
#include "SpaceInvaders_Sound.h"
#include "SpaceInvaders_Animation.h"
#include "SpaceInvaders_FIFO.h"

#define LEVEL_SLOW      3
#define LEVEL_MEDIUM    2
#define LEVEL_FAST      1
#define LEVEL_SUPERFAST 0

#define SHOOT_SPECIAL_COUNTDOWN 50

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long Convert(unsigned long sample);

static unsigned long ADCValue_Measure;   
static unsigned long ADCValue; 

static unsigned char shootFlag = 0;
static unsigned char shootSpecialFlag = 1;
static unsigned char shootSpecialCount = 1;
static unsigned char shootSpecialCountFlag = 1;

static unsigned char countSemaphore = 0;

static unsigned long scoreCount = 0;
static unsigned long scoreFinal = 0;

static unsigned char levelTrack = LEVEL_SLOW;

static unsigned long spriteToShoot = 0;
static unsigned long selectedSpriteToShoot = 0;
static unsigned char EnemyShot[NUM_SPRITE] = {0,0,0,0,0};
static unsigned char countRandom = 0;
static unsigned char spriteShootInit[NUM_SPRITE] = {0,0,0,0,0};
typedef enum
{
    SPACEINVADERS_WELCOME,
    SPACEINVADERS_START,
    SPACEINVADERS_SHOWLEVEL,
    SPACEINVADERS_INGAME,
    SPACEINVADERS_GAMEOVER,
    SPACEINVADERS_LEVELWIN,
    SPACEINVADERS_GAMEWIN,
    SPACEINVADERS_WAITSTATE,
    SPACEINVADERS_END
}gameState_t;
static gameState_t gameState = SPACEINVADERS_WELCOME;
static unsigned char Semaphore = 0;

// *************************** Capture image dimensions out of BMP**********
// #define BUNKERW ((unsigned char)Bunker0[18])
// #define BUNKERH ((unsigned char)Bunker0[22])
// #define ENEMY30W ((unsigned char)SmallEnemy30PointA[18])
// #define ENEMY30H ((unsigned char)SmallEnemy30PointA[22])
// #define ENEMY20W ((unsigned char)SmallEnemy20PointA[18])
// #define ENEMY20H ((unsigned char)SmallEnemy20PointA[22])
// #define ENEMY10W ((unsigned char)SmallEnemy10PointA[18])
// #define ENEMY10H ((unsigned char)SmallEnemy10PointA[22])
// #define ENEMYBONUSW ((unsigned char)SmallEnemyBonus0[18])
// #define ENEMYBONUSH ((unsigned char)SmallEnemyBonus0[22])
// #define LASERW ((unsigned char)Laser0[18])
// #define LASERH ((unsigned char)Laser0[22])
// #define MISSILEW ((unsigned char)Missile0[18])
// #define MISSILEH ((unsigned char)Missile0[22])
// #define PLAYERW ((unsigned char)PlayerShip0[18])
// #define PLAYERH ((unsigned char)PlayerShip0[22])

int main(void)
{
    unsigned char i, j;
	TExaS_Init(SSI0_Real_Nokia5110_Scope); // set system clock to 80 MHz
    // Random_Init(1);
    Nokia5110_Init();
    Nokia5110_ClearBuffer();
    Nokia5110_DisplayBuffer(); // draw buffer

    ADC0_Init();
    SpaceInvaders_SysTick_Init(2639999); //30 Hz 
    SpaceInvaders_Switches_PortE_Init();
    SpaceInvaders_LEDs_PortB_Init();
    SpaceInvaders_Sound_Init();
    
    spriteToShoot = Random32()%NUM_SPRITE;
    (void)SpaceInvaders_Fifo_Put(spriteToShoot);

    SpaceInvaders_LED_CoolDown_Off();

    Nokia5110_Clear();

    EnableInterrupts();
    Random_Init(NVIC_ST_CURRENT_R);
    while (1)
    {
        switch (gameState)
        {
        case SPACEINVADERS_WELCOME:
            Nokia5110_ClearBuffer();
            Nokia5110_DisplayBuffer();
            Nokia5110_SetCursor(0, 0);
            Nokia5110_OutString("HELLO PLAYER");
            Nokia5110_SetCursor(2, 1);
            Nokia5110_OutString("Welcome!");
            Nokia5110_SetCursor(1, 3);
            Nokia5110_OutString("Press Fire");
            Nokia5110_SetCursor(2, 4);
            Nokia5110_OutString("to play!");
            Delay100ms(1);
            gameState = SPACEINVADERS_WAITSTATE;
            break;

        case SPACEINVADERS_SHOWLEVEL:
            Nokia5110_Clear();
            Nokia5110_ClearBuffer();
            Nokia5110_DisplayBuffer();
            Nokia5110_SetCursor(2, 1);
            switch (levelTrack)
            {
            case LEVEL_SLOW:
                Nokia5110_OutString("Level 1");
                break;
            
            case LEVEL_MEDIUM:
                Nokia5110_OutString("Level 2");
                break;

            case LEVEL_FAST:
                Nokia5110_OutString("Level 3");
                break;

            case LEVEL_SUPERFAST:
                Nokia5110_OutString("Level 4");
                break;
            default:
                break;
            }
            
            Delay100ms(5);
            gameState = SPACEINVADERS_START;
            break;
        
        case SPACEINVADERS_WAITSTATE:
            /* do nothing */
            break;

        case SPACEINVADERS_START:
            Nokia5110_Clear();
            Nokia5110_ClearBuffer();
            Nokia5110_DisplayBuffer(); // draw buffer
            SpaceInvaders_Sprite_Init();
            SpaceInvaders_PlayerShip_Init();
            SpaceInvaders_Bunker_Init();
            SpaceInvaders_ShipMissile_Init();
            SpaceInvaders_ShipMissileSpecial_Init();
            for (j=0; j<NUM_SPRITE; j++)
            {
                SpaceInvaders_SpriteLaser_Init(j);
            }
            // SpaceInvaders_SpriteLaser_Init();
            SpaceInvaders_Fifo_Init();
            shootFlag = 0;
            shootSpecialFlag = 1;
            shootSpecialCount = 1;
            shootSpecialCountFlag = 1;
            scoreCount = 0;
            countSemaphore = 0;
            ADCValue_Measure = ADC0_In();
            ADCValue = ADCValue_Measure;
            SpaceInvaders_PlayerShip_Move(ADCValue);
            SpaceInvaders_PlayerShip_Draw();
            // SpaceInvaders_Sprite_Move();
            SpaceInvaders_Sprite_DrawAlive();
            SpaceInvaders_Bunker_Draw(BUNKER_UNDAMAGED);
            Delay100ms(1);
            gameState = SPACEINVADERS_INGAME;
            Nokia5110_ClearBuffer();
            Nokia5110_DisplayBuffer(); // draw buffer
            break;

        case SPACEINVADERS_INGAME:
            while (Semaphore == 0);
            if (SpaceInvaders_Sprite_GetY() == SpaceInvaders_Bunker_GetY()-BUNKER_HEIGHT)
            {
                gameState = SPACEINVADERS_GAMEOVER;
            }
            else
            {
                Nokia5110_ClearBuffer();
                Nokia5110_DisplayBuffer(); // draw buffer
                if (countSemaphore == levelTrack)
                {
                    SpaceInvaders_Sprite_Move();
                    countSemaphore = 0;
                }
                if (shootFlag == 1)
                {
                    SpaceInvaders_ShipMissile_Move();
                    SpaceInvaders_ShipMissile_Draw();
                    if (SpaceInvaders_ShipMissile_Sprites_Hit() == 1)
                    {
                        SpaceInvaders_Sprite_DrawDead();
                        scoreCount++;
                    }
                    if (SpaceInvaders_ShipMissile_GetY() - SHIPMISSILE_HEIGHT == 1)
                    {
                        shootFlag = 0;
                    }
                }
                else if (shootSpecialCount >= SHOOT_SPECIAL_COUNTDOWN)
                {
                    shootSpecialFlag = 0;
                    SpaceInvaders_LED_CoolDown_On();
                    shootSpecialCount = 0;
                    shootSpecialCountFlag = 0;
                }
                else if (shootSpecialFlag == 1 && shootSpecialCount == 0 && shootSpecialCountFlag == 0)
                {
                    SpaceInvaders_ShipMissileSpecial_Move();
                    SpaceInvaders_ShipMissileSpecial_1_Draw();
                    if (SpaceInvaders_ShipMissileSpecial_1_Sprites_Hit() == 1)
                    {
                        SpaceInvaders_Sprite_DrawDead();
                        scoreCount++;
                    }
                    SpaceInvaders_ShipMissileSpecial_2_Draw();
                    if (SpaceInvaders_ShipMissileSpecial_2_Sprites_Hit() == 1)
                    {
                        SpaceInvaders_Sprite_DrawDead();
                        scoreCount++;
                    }

                    if (SpaceInvaders_ShipMissileSpecial_1_GetY() - SHIPMISSILE_HEIGHT == 1 || \
                        SpaceInvaders_ShipMissileSpecial_2_GetY() - SHIPMISSILE_HEIGHT == 1 )
                    {
                        shootSpecialCountFlag = 1;
                    }

                }
                SpaceInvaders_Sprite_DrawAlive();
        
                if (countRandom == 20 && SpaceInvaders_Fifo_Get(&selectedSpriteToShoot)==1)
                {
                    if (EnemyShot[spriteToShoot] != 1)
                    {
                        EnemyShot[spriteToShoot] = 1;
                    }
                    countRandom = 0;
                }
                
                for (i=0;i<NUM_SPRITE;i++)
                {
                    if (EnemyShot[i] == 1)
                    {
                        if (spriteShootInit[i] == 0)
                        {
                            SpaceInvaders_SpriteLaser_Init(i);
                            spriteShootInit[i] = 1;
                        }
                        SpaceInvaders_SpriteLaser_Move(i);
                        SpaceInvaders_SpriteLaser_Draw(i);
                        if (SpaceInvaders_SpriteLaser_GetY(i) == NOKIA_HEIGHT)
                        {
                            EnemyShot[i] = 0;
                            spriteShootInit[i] = 0;
                        }
                    }
                }
                if (SpaceInvaders_SpriteLaser_Bunker_Hit() != BUNKER_DEAD)
                {
                    SpaceInvaders_Bunker_Draw(SpaceInvaders_SpriteLaser_Bunker_Hit());
                }
                if (SpaceInvaders_SpriteLaser_PlayerShip_Hit() == 1)
                {
                    SpaceInvaders_PlayerShip_SetLife(0);
                    SpaceInvaders_PlayerShip_Draw();
                    Delay100ms(2);
                    gameState = SPACEINVADERS_GAMEOVER;
                }
                else
                {
                    SpaceInvaders_PlayerShip_Draw();
                }
                ADCValue = ADC0_In();
                Semaphore = 0;
            }
            if (scoreCount == 5)
            {
                scoreFinal = scoreFinal + scoreCount;
                gameState = SPACEINVADERS_LEVELWIN;
            }
            
            break;

        case SPACEINVADERS_GAMEOVER:
            Nokia5110_Clear();
            Nokia5110_ClearBuffer();
            Nokia5110_DisplayBuffer();
            Nokia5110_SetCursor(1, 0);
            Nokia5110_OutString("GAME OVER!");
            Nokia5110_SetCursor(1, 1);
            Nokia5110_OutString("Try again?");
            Nokia5110_SetCursor(1, 3);
            Nokia5110_OutString("Press Fire");
            Nokia5110_SetCursor(2, 4);
            Nokia5110_OutString("button!");
            Delay100ms(1);
            gameState = SPACEINVADERS_END;
            break;

        case SPACEINVADERS_LEVELWIN:
            Nokia5110_Clear();
            Nokia5110_ClearBuffer();
            Nokia5110_DisplayBuffer();
            Nokia5110_SetCursor(1, 0);
            Nokia5110_OutString("Score:");
            Nokia5110_SetCursor(1, 1);
            Nokia5110_OutUDec(scoreFinal);
            Nokia5110_SetCursor(1, 2);
            if (levelTrack != 0)
            {
                levelTrack--;
            }
            else
            {
                gameState = SPACEINVADERS_GAMEWIN;
            }
            Delay100ms(5);
            gameState = SPACEINVADERS_SHOWLEVEL;
            break;

        case SPACEINVADERS_END:
            /* do nothing */
            break;

        case SPACEINVADERS_GAMEWIN:
            Nokia5110_Clear();
            Nokia5110_ClearBuffer();
            Nokia5110_DisplayBuffer();
            Nokia5110_SetCursor(1, 0);
            Nokia5110_OutString("Score:");
            Nokia5110_SetCursor(1, 1);
            Nokia5110_OutUDec(scoreFinal);
            Nokia5110_SetCursor(1, 3);
            Nokia5110_OutString("Victory!");
            break;

        default:
            /* do nothing */
            break;
        }
        
    }
}


void SysTick_Handler(void)
{
    if ((gameState == SPACEINVADERS_WAITSTATE) && (SpaceInvaders_GetSwitchState_PE0() == SWITCH_PRESSED))
    {
        gameState = SPACEINVADERS_SHOWLEVEL;
    }
    else if (gameState == SPACEINVADERS_INGAME)
    {
        SpaceInvaders_PlayerShip_Move(ADCValue);
        /* check if fire button is pressed */
        if (shootFlag == 0 && SpaceInvaders_GetSwitchState_PE0() == SWITCH_PRESSED)
        {
            SpaceInvaders_ShipMissile_SetPosMissile(SpaceInvaders_PlayerShip_GetX(), SpaceInvaders_PlayerShip_GetY());
            shootFlag = 1;
            /* play shoot sound */
            //SpaceInvaders_Sound_Shoot();
        }
        else if (shootSpecialFlag == 0 && SpaceInvaders_GetSwitchState_PE1() == SWITCH_PRESSED)
        {
            SpaceInvaders_ShipMissileSpecial_SetPosMissile(SpaceInvaders_PlayerShip_GetX(), SpaceInvaders_PlayerShip_GetY());
            SpaceInvaders_LED_CoolDown_Off();
            shootSpecialFlag = 1;
        }
        countSemaphore++;
        countRandom++;
        if (shootSpecialCountFlag == 1)
        {
            shootSpecialCount++;
        }
        spriteToShoot = Random32()%NUM_SPRITE;
        (void)SpaceInvaders_Fifo_Put(spriteToShoot);
        /* Trigger semaphore */
        Semaphore = 1;
    }
    else if ((gameState == SPACEINVADERS_END) && (SpaceInvaders_GetSwitchState_PE0() == SWITCH_PRESSED))
    {
        gameState = SPACEINVADERS_WELCOME;
    }
}

void Delay100ms(unsigned long count)
{
    unsigned long volatile time;
    while (count > 0)
    {
        time = 727240; // 0.1sec at 80 MHz
        while (time)
        {
            time--;
        }
        count--;
    }
}
