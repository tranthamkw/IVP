/*
   notes and comments 
   useful information
   to follow

*/

#include <stdio.h>
#include <stdlib.h>
#include "kenBoard.h"

//#define CN_RESERVE 5
#define CN_TARGET 8

int getPVCN7500(unsigned short chan, float* temperature);
int getSVCN7500(unsigned short chan, float* temperature);
int setSVCN7500(unsigned short chan, float temperature);
// chan is the rs485 channel
