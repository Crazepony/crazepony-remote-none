#ifndef _Led_H_
#define _Led_H_

#include "sys.h"


#define Led1       PAout(12)
#define Led2       PBout(4)
#define Led3       PBout(5)
#define Led4       PBout(6)
#define Led5       PBout(7)





void LedInit(void);   //Led初始化函数外部声明



#endif

