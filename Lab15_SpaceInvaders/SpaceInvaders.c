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

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long Convert(unsigned long sample);

static unsigned long ADCValue_Measure;   
static unsigned long ADCValue;     
static unsigned char ADCFlag = 0;  

typedef enum
{
    SPACEINVADERS_WELCOME,
    SPACEINVADERS_START,
    SPACEINVADERS_INGAME,
    SPACEINVADERS_GAMEOVER
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
    TExaS_Init(SSI0_Real_Nokia5110_Scope); // set system clock to 80 MHz
    Random_Init(1);
    Nokia5110_Init();
    Nokia5110_ClearBuffer();
    Nokia5110_DisplayBuffer(); // draw buffer

    // Nokia5110_Clear();
    // Nokia5110_SetCursor(0, 0);
    // Nokia5110_OutString("HELLO PLAYER");
    // Nokia5110_SetCursor(2, 1);
    // Nokia5110_OutString("Welcome!");
    // Nokia5110_SetCursor(1, 3);
    // Nokia5110_OutString("Press Fire");
    // Nokia5110_SetCursor(2, 4);
    // Nokia5110_OutString("to play!");

    // Nokia5110_PrintBMP(32, 47, PlayerShip0, 0); // player ship middle bottom
    // Nokia5110_PrintBMP(33, 47 - PLAYERH, Bunker0, 0);

    // Nokia5110_PrintBMP(0, ENEMY10H - 1, SmallEnemy10PointA, 0);
    // Nokia5110_PrintBMP(16, ENEMY10H - 1, SmallEnemy20PointA, 0);
    // Nokia5110_PrintBMP(32, ENEMY10H - 1, SmallEnemy20PointA, 0);
    // Nokia5110_PrintBMP(48, ENEMY10H - 1, SmallEnemy30PointA, 0);
    // Nokia5110_PrintBMP(64, ENEMY10H - 1, SmallEnemy30PointA, 0);
    // Nokia5110_DisplayBuffer(); // draw buffer

    // Delay100ms(50); // delay 5 sec at 50 MHz

    ADC0_Init();
    SpaceInvaders_SysTick_Init(2666666); //30 Hz
    SpaceInvaders_Switches_PortE_Init();
    SpaceInvaders_LEDs_PortB_Init();
    SpaceInvaders_Sound_Init();
    SpaceInvaders_Sprite_Init();
    SpaceInvaders_PlayerShip_Init();
    SpaceInvaders_Bunker_Init();

    EnableInterrupts();
    while (1)
    {
        switch (gameState)
        {
        case SPACEINVADERS_WELCOME:
            Nokia5110_Clear();
            Nokia5110_SetCursor(0, 0);
            Nokia5110_OutString("HELLO PLAYER");
            Nokia5110_SetCursor(2, 1);
            Nokia5110_OutString("Welcome!");
            Nokia5110_SetCursor(1, 3);
            Nokia5110_OutString("Press Fire");
            Nokia5110_SetCursor(2, 4);
            Nokia5110_OutString("to play!");
            break;
        
        case SPACEINVADERS_START:
            ADCValue_Measure = ADC0_In();
            ADCValue = ADCValue_Measure;
            SpaceInvaders_PlayerShip_Move(ADCValue);
            SpaceInvaders_PlayerShip_Draw();
            SpaceInvaders_Sprite_Move();
            SpaceInvaders_Sprite_Draw();
            SpaceInvaders_Bunker_Draw(BUNKER_UNDAMAGED);
            gameState = SPACEINVADERS_INGAME;
            break;

        case SPACEINVADERS_INGAME:
            SpaceInvaders_Sprite_Move();
            SpaceInvaders_Sprite_Draw();
            SpaceInvaders_Bunker_Draw(BUNKER_UNDAMAGED);
            /* Move player ship */
            if (ADCFlag == 1)
            {
                SpaceInvaders_PlayerShip_Move(ADCValue);
                SpaceInvaders_PlayerShip_Draw();
                ADCFlag = 0;
            }
            break;

        default:
            break;
        }

        // if (gameState == 1)
        // {
        //     ADCValue_Measure = ADC0_In();
        //     if (Semaphore == 1)
        //     {
        //         SpaceInvaders_Sprite_Move();
        //         SpaceInvaders_Sprite_Draw();
        //         /* Move player ship */
        //         if (ADCFlag == 1)
        //         {
        //             SpaceInvaders_PlayerShip_Move(ADCValue);
        //             SpaceInvaders_PlayerShip_Draw();
        //             ADCFlag = 0;
        //         }
        //         SpaceInvaders_Bunker_Draw(BUNKER_UNDAMAGED);
        //         Semaphore = 0;
        //     }
        // }
        
    }
}


void SysTick_Handler(void)
{
    if ((gameState == SPACEINVADERS_WELCOME) && (SpaceInvaders_GetSwitchState_PE0() == SWITCH_PRESSED))
    {
        gameState = SPACEINVADERS_START;
        Nokia5110_ClearBuffer();
        Nokia5110_DisplayBuffer(); // draw buffer
    }
    else if (gameState == SPACEINVADERS_INGAME)
    {
        /* ADC Slidepot */
        if (ADCValue_Measure != ADCValue)
        {
            ADCValue = ADCValue_Measure;
            ADCFlag = 1;
        }

        /* check if fire button is pressed */
        if (SpaceInvaders_GetSwitchState_PE0() == SWITCH_PRESSED)
        {
            /* play shoot sound */
            SpaceInvaders_Sound_Shoot();
        }
        
        /* Trigger semaphore */
        //Semaphore = 1;
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
