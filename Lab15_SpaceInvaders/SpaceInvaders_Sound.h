// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)

#ifndef _SPACEINVADERS_SOUND_H_
#define _SPACEINVADERS_SOUND_H_

void SpaceInvaders_Sound_Init(void);
void SpaceInvaders_Sound_Play(const unsigned char *pt, unsigned long count);
void SpaceInvaders_Sound_Shoot(void);
void SpaceInvaders_Sound_Killed(void);
void SpaceInvaders_Sound_Explosion(void);
void SpaceInvaders_Sound_Fastinvader1(void);
void SpaceInvaders_Sound_Fastinvader2(void);
void SpaceInvaders_Sound_Fastinvader3(void);
void SpaceInvaders_Sound_Fastinvader4(void);
void SpaceInvaders_Sound_Highpitch(void);

#endif /* _SPACEINVADERS_SOUND_H_ */
