#ifndef _Key_H_
#define _Key_H_

#include "sys.h"



//#define Key1     PBin(2)//PB2是BOOT1引脚，不用
#define kSet_Mode_Choose         0x12
#define kValueinc                0x13
#define kValuedec                0x14

#define Key2         PBin(1)
#define Key3         PBin(3)
#define Key4         PAin(8)



#define kPress       0
#define kRelease     !kPress




void KeyInit(void);   //Key初始化函数外部声明
char KeyScan(void);   //按键扫描函数


#endif

