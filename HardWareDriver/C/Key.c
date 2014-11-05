#include "Key.h"
/*    
      ____                      _____                  +---+
     / ___\                     / __ \                 | R |
    / /                        / /_/ /                 +---+
   / /   ________  ____  ___  / ____/___  ____  __   __
  / /  / ___/ __ `/_  / / _ \/ /   / __ \/ _  \/ /  / /
 / /__/ /  / /_/ / / /_/  __/ /   / /_/ / / / / /__/ /
 \___/_/   \__,_/ /___/\___/_/    \___ /_/ /_/____  /
                                                 / /
                                            ____/ /
                                           /_____/
Tim.c file
编写者：小马  (Camel)
作者E-mail：375836945@qq.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2014-01-28
功能：
1.按键IO口初始化
2.这部分按键的功能是对外开放的，我没有定义 其功能，大家自由发挥吧
------------------------------------
*/

/********************************************
              Key初始化函数
功能：
1.配置Key接口IO输入方向

描述：
Key接口：
Key1-->PB2
Key2-->PB1
Key3-->PB3
********************************************/
void KeyInit(void)
{
    RCC->APB2ENR|=1<<3;    //使能PORTB时钟
    GPIOB->CRL&=0XFFFF0F0F;  //PB1,2,3输入
    GPIOB->CRL|=0X00008080;
    GPIOB->ODR|=7<<1;        //PB1,2,3上拉
    
    RCC->APB2ENR|=1<<2;    //使能PORTA时钟
    GPIOB->CRH&=0XFFFFFFF0;  //PA8输入
    GPIOB->CRH|=0X00000008;
    GPIOB->ODR|=1<<8;        //PA8上拉
}


char KeyScan(void)
{
 if(Key2 == kPress) return kSet_Mode_Choose;
 if(Key3 == kPress) return kValueinc;
 if(Key4 == kPress) return kValuedec;
 
 return 0x15;
}





