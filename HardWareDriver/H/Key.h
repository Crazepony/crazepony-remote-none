#ifndef _Key_H_
#define _Key_H_

#include "sys_fun.h"

//#define Key1     PBin(2)//PB2是BOOT1引脚，不用
#define kSet_Mode_Choose         0x12
#define kValueinc                0x13
#define kValuedec                0x14




void KeyInit(void);   //Key初始化函数外部声明


#endif

