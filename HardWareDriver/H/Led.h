#ifndef _Led_H_
#define _Led_H_

#include "stm32f10x.h"

#define signalLED 1
#define led2      2
#define led3      3
#define led4      4
#define led5      5

void LedInit(void);   //Led初始化函数外部声明
void LedSet(u8 ch,char sta);//LED点亮关闭


#endif

