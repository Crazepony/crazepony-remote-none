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
led.c file
编写者：小马  (Camel)
作者E-mail：375836945@qq.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2014-01-28
------------------------------------
*/

#include "Led.h"
#include "UART1.h"
#include "config.h"

/********************************************
              Led初始化函数
功能：
1.配置Led接口IO输出方向
2.关闭所有Led(开机默认方式)
描述：
对应IO=1，灯亮
********************************************/
void LedInit(void)
{
	  static u8 i;
    RCC->APB2ENR|=1<<2;    //使能PORTA时钟	
    RCC->APB2ENR|=1<<3;    //使能PORTB时钟
  	RCC->APB2ENR|=1<<0;    //使能复用时钟	   
	
	  GPIOA->CRH&=0XFFF0FFFF;  //PA12推挽输出
    GPIOA->CRH|=0X00030000;
    GPIOA->ODR|=1<<12;        //PA12上拉
  
    GPIOB->CRL&=0X0000FFFF;  //PB4,5,6,7推挽输出
    GPIOB->CRL|=0X33330000;
    GPIOB->ODR|=15<<4;        //PB4,5,6,7上拉
    AFIO->MAPR|=2<<24;      //关闭JATG,千万不能讲SWD也关闭，否则芯片作废，亲测!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //PB4 默认是JNTRST接口,属于调试接口，复用功能为PB4 IO口
	
	  for(i=1;i<=5;i++)LedSet(i,0);//初始化关闭所以LED
	
	  
}

void LedSet(u8 ch,char sta)
{
	switch(ch)
	{
		case signalLED:
			if(sta == 1) GPIO_SetBits(GPIOA, GPIO_Pin_12);
				else  GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			break;
				
		case led2:
			if(sta == 1) GPIO_SetBits(GPIOB, GPIO_Pin_4);
				else  GPIO_ResetBits(GPIOB, GPIO_Pin_4);
			break;
		
		case led3:
			if(sta == 1) GPIO_SetBits(GPIOB, GPIO_Pin_5);
				else  GPIO_ResetBits(GPIOB, GPIO_Pin_5);
			break;
		
		case led4:
			if(sta == 1) GPIO_SetBits(GPIOB, GPIO_Pin_6);
				else  GPIO_ResetBits(GPIOB, GPIO_Pin_6);
			break;
		
		case led5:
			if(sta) GPIO_SetBits(GPIOB, GPIO_Pin_7);
				else  GPIO_ResetBits(GPIOB, GPIO_Pin_7);
			break;
	}
		
}

