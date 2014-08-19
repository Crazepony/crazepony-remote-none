#ifndef __Display_H__
#define __Display_H__
#include "stm32f10x.h"




extern    int Throttle;
extern    int Roll;
extern    int Pitch;
extern    int Yaw;
extern    int Battery;


void DisplayInit(void);
void ShowNumber(char x,char y,int Num);
void GetAD(unsigned char RomoteMode);
#endif


