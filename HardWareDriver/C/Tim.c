#include "tim.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "NRF24L01.h"
#include "Led.h"
#include "misc.h" 
#include "Display.h"
#include "stdio.h"
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
2.初始化定时器3，打印信息
------------------------------------
*/



	

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
      NRF24L01_TxPacket(TxBuf);	       //数据包发送，最大重发次数15次
      TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);   //清除中断标志  
    } 
}



int DebugCounter;             //打印信息输出时间间隔计数值

#define Debug

void TIM3_IRQHandler(void)		//打印中断服务程序
{
    if( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
    {     
#ifdef Debug
      DebugCounter++;
      if( DebugCounter==200)
            {
            DebugCounter=0;
//             printf(" ******************************************************************\r\n");
//             printf(" *       ____                      _____                  +---+   *\r\n");
//             printf(" *      / ___\\                     / __ \\                 | R |   *\r\n");
//             printf(" *     / /                        / /_/ /                 +---+   *\r\n");
//             printf(" *    / /   ________  ____  ___  / ____/___  ____  __   __        *\r\n");
//             printf(" *   / /  / ___/ __ `/_  / / _ \\/ /   / __ \\/ _  \\/ /  / /        *\r\n");
//             printf(" *  / /__/ /  / /_/ / / /_/  __/ /   / /_/ / / / / /__/ /         *\r\n");
//             printf(" *  \\___/_/   \\__,_/ /___/\\___/_/    \\___ /_/ /_/____  /          *\r\n");
//             printf(" *                                                  / /           *\r\n");
//             printf(" *                                             ____/ /            *\r\n");
//             printf(" *                                            /_____/             *\r\n");
//             printf(" ******************************************************************\r\n");
           // printf("\r\n");
            printf(" Crazepony-II报告：系统正在运行...\r\n"); 
            printf("\r\n--->遥控实时杆量<---\r\n");
            printf("油门当前值---> %d\r\n",(int) Throttle);
            printf("俯仰当前值---> %d\r\n",(int) Pitch);
            printf("横滚当前值---> %d\r\n",(int) Roll);
            printf("偏航当前值---> %d\r\n",(int) Yaw);
            


        }
#else      
             
#endif
        
        TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);   //清除中断标志   
    }
}








//定时器4初始化：用来中断处理PID
void TIM4_Init(char clock,int Preiod)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //打开时钟
    
    TIM_DeInit(TIM4);

    TIM_TimeBaseStructure.TIM_Period = Preiod;
    TIM_TimeBaseStructure.TIM_Prescaler = clock-1;//定时1ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);

    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM4,ENABLE);
    //printf("定时器4初始化完成...\r\n");
    
}	


//定时器3初始化
void TIM3_Init(char clock,int Preiod)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //打开时钟
    
    TIM_DeInit(TIM3);

    TIM_TimeBaseStructure.TIM_Period = Preiod;
    TIM_TimeBaseStructure.TIM_Prescaler = clock-1;//定时1ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3,TIM_FLAG_Update);

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3,ENABLE);
  
    //printf("定时器3初始化完成...\r\n");
}		


void TimerNVIC_Configuration()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* NVIC_PriorityGroup 2 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //TIM3
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//定时器3作为串口打印定时器
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //TIM4
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

} 




