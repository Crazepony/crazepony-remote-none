#include "tim.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "NRF24L01.h"
#include "Led.h"
#include "misc.h" 
#include "Display.h"
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
1.初始化定时器4，发送遥控数据
------------------------------------
*/



void TIM4_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //打开时钟
    
    TIM_DeInit(TIM4);

    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_Prescaler = 36-1;//定时1ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    

   
    /* NVIC_PriorityGroup 1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //TIM4
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM4,ENABLE);
    
}		

int Ledcounter;


//定时器控制入口函数
//定时周期为1ms
//该逻辑可以用来显示遥控信号的强度，遥控上灯闪得越快，信号越弱
//因为发送函数里面有个最大重发次数，当收不到应答信号时，在发送函数处会耗时，重发次数越多
//耗时越多，LED就会出现闪动越厉害的现象

void TIM4_IRQHandler(void)		//1ms中断一次,用于程序读取6050等
{
    if( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
    {     
     Ledcounter++;
      
     if(Ledcounter==59)Led1=1;
     if(Ledcounter==60){Led1=0;Ledcounter=0;}
     

      TxBuf[0]=(Throttle*10)&0xff;
      TxBuf[1]=((Throttle*10)>>8);     //油门量装入待发送数组的前两个字节
      TxBuf[2]=Pitch;                  //俯仰角装入第3个字节
      TxBuf[3]=Roll;                   //横滚角转入第4个字节
      TxBuf[4]=Yaw;                    //偏航角装入第5个字节
      TxBuf[31]=(Throttle<=5) ? 0:0xA5;//紧急熄火标志字节控制     
      NRF24L01_TxPacket(TxBuf);	//数据包发送，最大重发次数15次
     
     
      TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);   //清除中断标志  
      
        
        
    } 
}

