#include "SpaceInvaders_FIFO.h"

static unsigned long PutI;  // index to put new
static unsigned long GetI;  // index of oldest
static unsigned long Fifo[FIFO_SIZE];

void SpaceInvaders_Fifo_Init(void)
{
    PutI = GetI = 0; //empty
}

unsigned char SpaceInvaders_Fifo_Put(unsigned long data)
{
    if(((PutI+1)%FIFO_SIZE) == GetI)
    {
        return 0; // fail if full
    }
    Fifo[PutI] = data;        // save in Fifo
    PutI = (PutI+1)%FIFO_SIZE; // next place to put
    return 1;
}

unsigned char SpaceInvaders_Fifo_Get(unsigned long *datapt)
{
    if(GetI == PutI) 
    {
        return 0; // fail if empty
    }
    *datapt = Fifo[GetI];       // retrieve data
    GetI = (GetI+1)%FIFO_SIZE; // next place to get
    return 1;
}

