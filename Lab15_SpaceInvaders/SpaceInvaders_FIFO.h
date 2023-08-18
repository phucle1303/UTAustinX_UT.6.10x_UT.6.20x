#ifndef _SPACEINVADERS_FIFO_H_
#define _SPACEINVADERS_FIFO_H_

#define FIFO_SIZE 20

// typedef struct
// {
//     unsigned char selectedData;
//     unsigned char flag;
// }data_t;


void SpaceInvaders_Fifo_Init(void);
unsigned char SpaceInvaders_Fifo_Put(unsigned long data);
unsigned char SpaceInvaders_Fifo_Get(unsigned long *datapt);

#endif /* _SPACEINVADERS_FIFO_H_ */
